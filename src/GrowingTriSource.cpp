// Danny Keig 2018

// A sketch which plays with triangles growing out from the corners of the bigger 'boundary' triangles.
// It progresses from growing triangles, to growing and rotating triangles, with variation through line width and rotation offset.
// Help with calculating triangles, given one corner vector:
// https://www.processing.org/discourse/beta/num_1252373090.html
// thanks to ben_hem!


#include "GrowingTriSource.h"
//--------------------------------------------------------------
void GrowingTriSource::setup(){
	// Give our source a decent name
    name = "Growing Triangle";
	// Allocate our FBO source, decide how big it should be
    allocate(700, 600);
    
    ofPushStyle();
    ofNoFill();
    int initialSize = 10;
    x1 = 20;
    y1 = 580;
    angle1 = 0;
    angleStep = 0;
    boundarySize = 660;
    triSz = 10;
    maxSz = 50;
    grow = true;
    rotateOn = false;
    growth = 1;
    sinStep = 0;
    drawBoundaryTriangle();
    tri1Loc.set (x1, y1);
    tri2Loc.set (x2, y2);
    tri3Loc.set (x3, y3);
    angleOffset = 1;
    ofSetLineWidth(20);
    
    for (int i = 0; i < initialSize; i++){
        posX.push_back(100);
        posY.push_back(100);
    }
    ofPopStyle();
}

//--------------------------------------------------------------

void GrowingTriSource::reset(){
    //reset is called optionally. if you leave it empty nothing is happening    
    startTime = ofGetElapsedTimeMillis();
    
    ofPushStyle();
    ofNoFill();
    boundarySize = 660;
    triSz = 10;
    maxSz = 50;
    grow = true;
    rotateOn = false;
    angleOffset = 1;
    ofSetLineWidth(20);
    ofPopStyle();
}

//--------------------------------------------------------------

void GrowingTriSource::setName(string _name){
    name = _name;
}

//--------------------------------------------------------------

// Don't do any drawing here
void GrowingTriSource::update(){
    
    ofPushStyle();
    
    //moving value for the growth of the triangles
    growth = ofMap(cos(sinStep), -1, 1, 1, 3.5);
    
    //conditions under which to grow the triangles, or shrink the triangles.
    if (grow) triSz += growth;
    if (!grow) triSz -= growth;
    
    if (tx2 > x2 - 20){
        grow = false;
    }
    if (tx2 < x1){
        grow = true;
    }
    
    // step through sin and angle slowly
    angleStep += 0.01;
    sinStep += 0.01;
    ofPopStyle();
}
//--------------------------------------------------------------

// No need to take care of fbo.begin() and fbo.end() here.
// All within draw() is being rendered into fbo;
void GrowingTriSource::draw(){
    
    ofPushStyle();
    ofNoFill();
    ofClear(0); //clear the buffer
    
    // draw an empty boundary triangle for the shapes to move within;
    drawBoundaryTriangle();
    
    // moving alpha value for one of the lerp colours.
    float alpha1 = ofMap(sin(sinStep), -1, 1, 80, 200);
    
    ofSetBackgroundColor(0);
    
    // create a set of colours to lerp through, and a colourScale variable to create variation as the triangles grow!
    
    float colorScale = ofMap(triSz, 0, maxSz, 0., 1.);
    ofColor r = ofColor(0, 100, 274, alpha1);
    ofColor b = ofColor(42,143,245, 100);
    ofSetColor(b.lerp(r, colorScale));
    
    // sequence of events!
    // 1. standard growing triangles
    
    if(ofGetElapsedTimeMillis() - startTime < 22000){
        ofSetLineWidth(20);
        angle1 = 0;
        angle2 = 0;
        angle3 = 0;
        rotateOn = false;
        angleStep = 0;
    }
    
    // 2. growing and rotating triangles
    
    if(ofGetElapsedTimeMillis() - startTime > 22000) {
        rotateOn = true;
    }
    
    if(ofGetElapsedTimeMillis() - startTime > 22000 && rotateOn == true){
        float sameAngle = ofMap(sin(angleStep), -1, 1, -60, 60);
        angle1 = sameAngle;
        angle2 = sameAngle;
        angle3 = sameAngle;
    }
    
    // 3. growing and rotating triangles, with rotation of each corner offset by an amount. The offset amount is controlled by sin.
    
    // Time to draw all the triangles!!
    
    if(ofGetElapsedTimeMillis() - startTime > 43000){
        angleOffset = ofMap(sin(angleStep * 0.5), -1, 1, -0.7, 0.7);
        angle1 = ofMap(sin(angleStep * angleOffset), -1, 1, -60, 60);
        angle2 = ofMap(sin(angleStep * angleOffset * 2), -1, 1, -60, 60);
        angle3 = ofMap(sin(angleStep * angleOffset * 3), -1, 1, -60, 60);
    }
    
    // Draw triangle in corner 1.
    
    ofPushMatrix();
    ofTranslate(tri1Loc.x, tri1Loc.y);
    ofRotate(angle1);
    growingTri(0, 0, triSz, 0, 1);
    ofPopMatrix();
    
    // offset colour for tri 2.
    ofSetColor(b.lerp(r, (colorScale * 0.3)));
    
    // Draw triangle in corner 2, by translating and rotating -120 degrees.
    ofPushMatrix();
    ofTranslate(tri2Loc.x, tri2Loc.y);
    ofRotate(-120);
    ofRotate(angle2);
    growingTri(0, 0, triSz, 0, 2);
    ofPopMatrix();
    
    // offset colour for tri 3
    ofSetColor(b.lerp(r, (colorScale * 0.6)));
    
    // Draw triangle in corner 3, by translating and rotating 120 degrees.
    ofPushMatrix();
    ofTranslate(tri3Loc.x, tri3Loc.y);
    ofRotate(120);
    ofRotate(angle3);
    growingTri(0, 0, triSz, 0, 3);
    ofPopMatrix();
    
    ofPopStyle();
}

//--------------------------------------------------------------

void GrowingTriSource::drawBoundaryTriangle(){
    
    // calculations for boundary/edge triangle
    
    x2 = x1 + boundarySize ;
    y2 = y1;
    x3 = x1 + (cos(atan2(y2-y1,x2-x1)-PI/3) * ofDist(x1,y1,x2,y2));
    y3 = y1 + (sin(atan2(y2-y1,x2-x1)-PI/3) * ofDist(x1,y1,x2,y2));
    
    ofPushStyle();
    ofSetLineWidth(1);
    ofDrawTriangle(x1,y1,x2,y2,x3,y3);
    ofPopStyle();
}


//--------------------------------------------------------------

void GrowingTriSource::movTri(float x, float y, float sz){
    
    // calculations for animation triangles, prividing inputs for location and size.
    
    tx1 = x;
    ty1 = y;
    tx2 = tx1 + sz ;
    ty2 = ty1;
    tx3 = tx1 + (cos(atan2(ty2-ty1,tx2-tx1)-PI/3) * ofDist(tx1,ty1,tx2,ty2));
    ty3 = ty1 + (sin(atan2(ty2-ty1,tx2-tx1)-PI/3) * ofDist(tx1,ty1,tx2,ty2));
    
    ofDrawTriangle(tx1,ty1,tx2,ty2,tx3,ty3);
}

//--------------------------------------------------------------

void GrowingTriSource::growingTri(float x, float y, float Sz, float offset, int triNum){
    
    // for each of our corners, draw many triangles, scaled smoothly in size.
    
    for(int i = 0; i < (posX.size() - offset) ; i++){
        
        if(ofGetElapsedTimeMillis() < 11500){
            ofSetLineWidth(10);
        }
        
        if(ofGetElapsedTimeMillis() - startTime > 10000){
            lineThickness();
        }
        
        if(ofGetElapsedTimeMillis() - startTime > 43000 && ofGetElapsedTimeMillis() - startTime < 60000){
            ofSetLineWidth(ofRandom(3, 20));
        }
        
        float scaleSize = Sz * (i*0.1);
        movTri(x, y, scaleSize);
    }
}


//--------------------------------------------------------------
void GrowingTriSource::lineThickness(){
    
    // when called, this function will map line width to sin
    
    float thickness = ofMap(sin(sinStep), -1, 1, 8, 25, true);
    ofSetLineWidth(thickness);
}

