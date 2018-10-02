#pragma once

#include "ofMain.h"
#include "FboSource.h"

class GrowingTriSource : public ofx::piMapper::FboSource {
	public:
        void setup();
		void update();
		void draw();
        void reset();
        void setName(string _name);

        void growingTri(float x, float y, float Sz, float offset, int triNum);
        void movTri(float x, float y, float sz);
        void drawBoundaryTriangle();
        void lineThickness();
        void increaseNumOfTris();
        void decreaseNumOfTris();
    
        int startTime;
    
        float x1, y1, x2, y2, x3, y3;
        float tx1, ty1, tx2, ty2, tx3, ty3;
        float boundarySize;
        float triSz;
        float angle1, angle2, angle3;
        float angleStep;
        float sinStep;
        float angleOffset;
        float maxSz;
        float growth;
    
        ofPoint tri1Loc;
        ofPoint tri2Loc;
        ofPoint tri3Loc;
    
        bool grow;
        bool rotateOn;
        vector<float> posX, posY;
    
    
    
};
