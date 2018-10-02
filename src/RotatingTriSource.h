#pragma once

#include "ofMain.h"
#include "FboSource.h"

class RotatingTriSource : public ofx::piMapper::FboSource {
	public:
        void setup();
		void update();
		void draw();
        void reset();
        void drawMovingRect(int, int, int, int, float);
        void setName(string _name);
    
        void drawBoundaryTri();
        void movTri(int ox, int oy, int r);
        void boundarySetup(int ox, int oy, int r);
        void decreaseTriDensity();
        void increaseTriDensity();
        void steadyGrowing();
        void pshychGrowing();
        void fastGrowing();
    
        ofPoint movTri1Loc;
        ofPoint movTri2Loc;
        ofPoint movTri3Loc;
        ofPoint a;
        ofPoint b;
        ofPoint c;
    
        int startTime;
        float scaleVal;
        int TriDensity;
        int rotAngle;
        float angleStep;
        int minSz, maxSz;
        float sinStep;
        bool isGrowing;
        bool noisedRotateScene;
        static const int xSeeds = 100;
        vector<float> noiseSeeds;
        vector<float> posX, posY;
    
    
};
