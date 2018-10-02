#pragma once

#include "ofMain.h"
#include "FboSource.h"

class SlidingTriSource : public ofx::piMapper::FboSource {
	public:
        void setup();
		void update();
		void draw();
        void reset();
        void setName(string _name);

        void movTri(float x, float y, float sz);
        void drawBoundaryTriangle();
        void snappyMovement();
        void flowingMovement();
    
        float boundarySize;
        float minSz, maxSz;
        float x1, y1, x2, y2, x3, y3;
        float tx1, ty1, tx2, ty2, tx3, ty3;
        float sinStep;
    
        int startTime;
        int step;
    
        vector <int> posX, posY;
        ofPoint location;
        ofPoint dist;
        ofPoint p1, p2, p3;
        ofPoint t1, t2, t3;
        ofPoint c;
        ofPoint newT3;
    
};
