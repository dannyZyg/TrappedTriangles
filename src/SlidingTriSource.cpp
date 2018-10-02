// Danny Keig 2018

// An animation experimenting with triangles jumping from corner to corner of the boundary.

// Help with calculating triangles, given one corner vector:
// https://www.processing.org/discourse/beta/num_1252373090.html
// thanks to ben_hem!

// inspiration and help taken from the Fiery Comet Trail example from Theo. Thanks!

#include "SlidingTriSource.h"

//--------------------------------------------------------------
void SlidingTriSource::setup(){
	// Give our source a decent name
    name = "Sliding Triangle";
	// Allocate our FBO source, decide how big it should be
    allocate(700, 700);
    
    ofPushStyle();
    ofBackground(0);
    int initialSize = 10;
    x1 = 20;
    y1 = 400;
    boundarySize = 400;
    minSz = 10;
    maxSz = 200;
    sinStep = 0;
    
    location.set(x1, y1);
    
    for(int i=0; i<initialSize; i++){
        posX.push_back(x1);
        posY.push_back(y1);
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void SlidingTriSource::reset(){
    //reset is called optionally. if you leave it empty nothing is happening
    startTime = ofGetElapsedTimeMillis();
    
    ofPushStyle();
    minSz = 10;
    maxSz = 200;
    ofPopStyle();
    
}
//--------------------------------------------------------------
void SlidingTriSource::setName(string _name){
    name = _name;
}

//--------------------------------------------------------------

// Don't do any drawing here
void SlidingTriSource::update(){
    
    // update the vector so that each position shifts up one.
    
    for(int i = 0; i<posX.size() -1; i++){
        posX[i] = posX[i + 1];
        posY[i] = posY[i + 1];
    }
    
    // update the leading triangle with the location coordinates
    posX[posX.size() -1] = location.x;
    posY[posY.size() -1] = location.y;
    
    sinStep += 0.01;
    
}
//--------------------------------------------------------------

// No need to take care of fbo.begin() and fbo.end() here.
// All within draw() is being rendered into fbo;
void SlidingTriSource::draw(){
    ofClear(0); //clear the buffer

    drawBoundaryTriangle();
    ofPushStyle();
    
    // 1. snappy movement to start
    if(ofGetElapsedTimeMillis() - startTime <25000){
        snappyMovement();
    }
    
    //2. move to flowing movement
    if(ofGetElapsedTimeMillis() - startTime >25000){
        flowingMovement();
    }
    
    // draw lots of triangles!
    
    for( int i = 0; i < posX.size(); i++){
        
        float frame = ofGetFrameNum()%60;
        float thickness;
        
        // random thickness for glitchiness
        if(ofGetElapsedTimeMillis() - startTime < 40000){
            thickness = ofRandom(3, 20);
        }
        
        if(ofGetElapsedTimeMillis() - startTime > 40000){
        thickness = ofMap(frame, 0, 60, 0.1, 10 );
        }
        float colorMov1;
        float colorMov2;
        float colorMov3;
        
        
        // colours are effected by both lerp and sin
        if(ofGetElapsedTimeMillis() - startTime > 10000){
            colorMov1 = ofMap(sin(sinStep), -1, 1, 70, 255);
            colorMov2 = ofMap(cos(sinStep), -1, 1, 150, 200);
            colorMov3 = ofMap(sin(sinStep * 0.5), -1, 1, 0, 125);
        }
        else{
            colorMov1 = 150;
            colorMov2 = 220;
            colorMov3 = 220;
        }
        
        ofSetLineWidth(thickness + (i*0.2));
        ofColor color1 = ofColor(0, 255, 100, 200);
        ofColor color2 = ofColor(ofColor(colorMov1, colorMov2, colorMov3, 200));
        float colorPC = ofMap(i, 0, posX.size() -1, 0, 1);
        ofSetColor(color1.getLerped(color2, colorPC));
        
        // size changes with sin after a certain time
        if(ofGetElapsedTimeMillis() - startTime > 25000){
        maxSz = ofMap(sin(sinStep), -1 , 1, 50, 300);
        }
        
        // scale the triangles by min/max sz
        float scaledSz = ofMap(i, 0, posX.size(), minSz, maxSz);
        ofNoFill();
        movTri(posX[i], posY[i], scaledSz);
    }
    ofPopStyle();

}

//--------------------------------------------------------------

void SlidingTriSource::drawBoundaryTriangle(){
    
    // draw the boundary/edge traingle
    
    ofPushStyle();
    
    x2 = x1 + boundarySize ;
    y2 = y1;
    x3 = x1 + (cos(atan2(y2-y1,x2-x1)-PI/3) * ofDist(x1,y1,x2,y2));
    y3 = y1 + (sin(atan2(y2-y1,x2-x1)-PI/3) * ofDist(x1,y1,x2,y2));
    
    p1.set(x1, y1);
    p2.set(x2 , y2);
    p3.set(x3 , y3);
    
    // calculate new coordinated for the top point, for use in my flowingMovement function.
    
    newT3.x = x1 + (cos(atan2(y2-y1,x2-x1)-PI/3) * ofDist(x1,y1,x2 - maxSz,y2));
    newT3.y = y1 + (sin(atan2(y2-y1,x2-x1)-PI/3) * ofDist(x1,y1,x2 - maxSz,y2));
    
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(1);
    ofNoFill();
    ofDrawTriangle(x1,y1,x2,y2,x3,y3);
    ofPopStyle();
    ofPopStyle();
    
}

//--------------------------------------------------------------


void SlidingTriSource::movTri(float x, float y, float sz){
    
    // draw the moving triangles
    
    tx1 = x;
    ty1 = y;
    tx2 = tx1 + sz ;
    ty2 = ty1;
    tx3 = tx1 + (cos(atan2(ty2-ty1,tx2-tx1)-PI/3) * ofDist(tx1,ty1,tx2,ty2));
    ty3 = ty1 + (sin(atan2(ty2-ty1,tx2-tx1)-PI/3) * ofDist(tx1,ty1,tx2,ty2));
    
    ofDrawTriangle(tx1,ty1,tx2,ty2,tx3,ty3);
    
    //create vectors for movement calculations.
    t1.set(tx1, ty1);
    t2.set(tx2, ty2);
    t3.set(tx3, ty3);
}

//--------------------------------------------------------------

void SlidingTriSource::snappyMovement(){
    
    // with a low movSpeed, the triangles snap to the corners when the next step is called.
    
    float movSpeed = 2;
    if (step > 3){
        step = 1;
    }
    if(step == 1){
        dist = p2 - t2;
        location += dist/movSpeed;
        maxSz = ofRandom(50, 300);
    }
    if(step == 2){
        dist = p3 - t3;
        location += dist/movSpeed;
        maxSz = ofRandom(50, 300);
    }
    if(step == 3){
        dist = p1 - location;
        location += dist/movSpeed;
        maxSz = ofRandom(50, 300);
    }
    if(ofGetFrameNum()%120 == 0){
        step++;
    }
}

//--------------------------------------------------------------
void SlidingTriSource::flowingMovement(){
    
    // with the movSpeed controlled by sin, with another sin offset, the speed changes somewhat unpredicatably.
    
    float movSpeed;
    float offset = 1;
    if(ofGetElapsedTimeMillis() > 40000){
        offset = ofMap(sin(sinStep), -1, 1, -2, 2);
    }
    movSpeed = ofMap(cos(sinStep * offset), -1, 1, 5, 50);
    if (step > 3){
        step = 1;
    }
    if(step == 1){
        dist = p2 - t2;
        location += dist/movSpeed ;
    }
    
    // use the new location for the top corner, to attempt to keep the moving triangles in the frame.
    
    if(step == 2){
        dist = newT3 - location;
        location += dist/movSpeed;
    }
    if(step == 3){
        dist = p1 - location;
        location += dist/movSpeed;
    }
    if(ofGetFrameNum()%120 == 0){
        step++;
    }
}

