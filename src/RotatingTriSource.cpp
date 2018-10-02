// Danny Keig 2018


// An animation based on equilateral triangles, calculated from a center point.
// Features growing and scaled triangles, and then rotating triangles.

// Noise helped by noise code examples week 5.
// formula for an equilateral triangle (given the center), taken from user 'Xantix' at Stack Overflow:
// https://stackoverflow.com/questions/11449856/draw-a-equilateral-triangle-given-the-center


#include "RotatingTriSource.h"

//--------------------------------------------------------------
void RotatingTriSource::setup(){
	// Give our source a decent name
    name = "Rotating Triangle";
	// Allocate our FBO source, decide how big it should be
    allocate(1000, 800);
    
    ofPushStyle();
    
    boundarySetup(0, 0, 400);
    ofNoFill();
    movTri1Loc.set (b.x, b.y);
    TriDensity = 10; // a variable different to posX.size(), used for scaling.
    rotAngle = 0;
    angleStep = 0.3;
    minSz = 10;
    maxSz = 250;
    sinStep = 0.1;
    isGrowing = true;
    noisedRotateScene = false;
    int initialSize = 1;
    
    for (int i = 0; i < initialSize; i++){
        posX.push_back(0);
        posY.push_back(0);
    }
    for (int i = 0; i < xSeeds; i++){
        noiseSeeds.push_back( ofRandom(1000));
    }

    ofPopStyle();
}

//--------------------------------------------------------------

void RotatingTriSource::reset(){
    //reset is called optionally. if you leave it empty nothing is happening
    startTime = ofGetElapsedTimeMillis();
    if(posX.size() > 1){
        decreaseTriDensity();
    }
}

//--------------------------------------------------------------

void RotatingTriSource::setName(string _name){
    name = _name;
}

//--------------------------------------------------------------
// Don't do any drawing here
void RotatingTriSource::update(){

    sinStep += 0.01;
}
//--------------------------------------------------------------

// No need to take care of fbo.begin() and fbo.end() here.
// All within draw() is being rendered into fbo;
void RotatingTriSource::draw(){
    
    ofPushStyle();
    ofNoFill();
    ofClear(0); //clear the buffer
    
    //set color and draw boundary triangle
    ofPushMatrix();
    ofTranslate(fbo->getWidth()/2, fbo->getHeight()/2);
    ofRotate(180);
    ofSetColor(255);
    
    drawBoundaryTri();
    
    // begin drawing animation triangles
    // lerp the colour based on the size of the triangle
    
    ofPushMatrix();
    ofSetColor(255, 0, 0);;
    for(int i = 0; i < posX.size() -1 ; i++){
        float scaledSz = ofMap(i, 0, TriDensity, minSz, maxSz);
        ofSetLineWidth(3);
        ofColor r = ofColor(0, 100, 274);
        ofColor b = ofColor(42,143,245);
        ofSetColor(b.lerp(r, scaledSz));
       
        
        if(noisedRotateScene == true){
            float noiseAngleLim = ofMap(sin(sinStep), -1, 1, 30, 360);
            ofRotate(ofNoise(noiseSeeds[i]) * noiseAngleLim);
        }
        movTri(posX[i], posY[i], scaledSz);
        noiseSeeds[i] += 0.01;
    }
    
    ofPopMatrix();
    ofPopMatrix();
    
    
//     The following statements determine the order and timeline of events

    // 1. steady growing
    
    if(ofGetElapsedTimeMillis() - startTime < 12000){
    steadyGrowing();
    }
    
    // 2. faster growing
    if(ofGetElapsedTimeMillis() - startTime > 12000 && ofGetElapsedTimeMillis() - startTime < 22000){
    fastGrowing();
    }
    
    // 3. psychedelic growing!
    if(ofGetElapsedTimeMillis() - startTime > 22000 && ofGetElapsedTimeMillis() - startTime < 40000){
    pshychGrowing();
    }
    
    // 4. noised rotation
    if(ofGetElapsedTimeMillis() - startTime > 40000){
    noisedRotateScene = true;
    }
    if(ofGetElapsedTimeMillis() - startTime < 40000){
        noisedRotateScene = false;
    }
    
    // make sure noised rotation scene has right number of triangles, so they don't exceed the edges of the boundary
    if(noisedRotateScene == true){
        TriDensity = 10;
        if(posX.size() < 10){
            increaseTriDensity();
        }
        if(posX.size() > 10){
            decreaseTriDensity();
        }
    }
    

    ofPopStyle();
}

//--------------------------------------------------------------

void RotatingTriSource::boundarySetup(int ox, int oy, int r){
    
    // calculate all of the geometry for displaying the boundary triangle
    
    int full_angle = 360;
    c.set ( ox, oy + r );
    b.x = (c.x * cos(ofDegToRad(full_angle / 3))) - (c.y * sin(ofDegToRad(full_angle / 3)));
    b.y = (c.x * sin(ofDegToRad(full_angle / 3))) + (c.y * cos(ofDegToRad(full_angle / 3)));
    a.x = (c.x * cos(ofDegToRad(full_angle * 2 / 3))) - (c.y * sin(ofDegToRad(full_angle * 2 / 3)));
    a.y = (c.x * sin(ofDegToRad(full_angle * 2 / 3))) + (c.y * cos(ofDegToRad(full_angle * 2 / 3)));
}

//--------------------------------------------------------------


void RotatingTriSource::drawBoundaryTri(){
    
    //display the boundary triangle
    ofPushStyle();
    ofSetLineWidth(1);
    ofDrawTriangle(a.x, b.y, b.x, b.y, c.x, c.y);
    ofPopStyle();
}

//--------------------------------------------------------------


void RotatingTriSource::movTri(int ox, int oy, int r){
    
    // calculate the geometry for themoving triangles
    ofPushStyle();
    int full_angle = 360;
    ofPoint a;
    ofPoint b;
    ofPoint c;
    c.set (ox, oy + r);
    b.x = (c.x * cos(ofDegToRad(full_angle / 3))) - (c.y * sin(ofDegToRad(full_angle / 3)));
    b.y = (c.x * sin(ofDegToRad(full_angle / 3))) + (c.y * cos(ofDegToRad(full_angle / 3)));
    a.x = (c.x * cos(ofDegToRad(full_angle * 2 / 3))) - (c.y * sin(ofDegToRad(full_angle * 2 / 3)));
    a.y = (c.x * sin(ofDegToRad(full_angle * 2 / 3))) + (c.y * cos(ofDegToRad(full_angle * 2 / 3)));
    
    ofSetLineWidth(2);
    ofDrawTriangle(a.x, b.y, b.x, b.y, c.x, c.y);
    ofPopStyle();
}

//--------------------------------------------------------------
void RotatingTriSource::increaseTriDensity(){
    
    // add more triangles
    
    posX.push_back(0);
    posY.push_back(0);
}

//--------------------------------------------------------------
void RotatingTriSource::decreaseTriDensity(){
    
    // take away triangles, but stop at a certain point
    
    if(TriDensity > 1){
        posX.pop_back();
        posY.pop_back();
    }
}

//--------------------------------------------------------------

void RotatingTriSource::steadyGrowing(){
    
    // conditionals for controlling a steady growth of triangles, stopping at the boundaries.
    
    if(ofGetFrameNum()%20 == 0){
        if(isGrowing) {
            posX.push_back(0);
            posY.push_back(0);
        }
        if(!isGrowing){
            posX.pop_back();
            posY.pop_back();
        }
    }
    if(posX.size() > 16){
        isGrowing = false;
    }
    if(posX.size() <2){
        isGrowing = true;
    }
}

//--------------------------------------------------------------

void RotatingTriSource::fastGrowing(){
    
    //conditionals for a faster growth, stopping at the boundaries.
    
    if(ofGetFrameNum()%5 == 0){
        if(isGrowing) {
            posX.push_back(0);
            posY.push_back(0);
        }
        if(!isGrowing){
            posX.pop_back();
            posY.pop_back();
        }
    }
    if(posX.size() > 16){
        isGrowing = false;
    }
    if(posX.size() <2){
        isGrowing = true;
    }
}

//--------------------------------------------------------------

void RotatingTriSource::pshychGrowing(){
    
    // conditionals for a fast growth, with triangle density controlled by sin. Creates a psychedelic effect!
    
    TriDensity = ofMap(cos(sinStep), -1, 1, 5, 30);
    
    if(ofGetFrameNum()%5 == 0){
        if(isGrowing) {
            posX.push_back(0);
            posY.push_back(0);
        }
        if(!isGrowing){
            posX.pop_back();
            posY.pop_back();
        }
    }
    if(posX.size() > 30){
        isGrowing = false;
    }
    if(posX.size() <2){
        isGrowing = true;
    }
}
    

    

