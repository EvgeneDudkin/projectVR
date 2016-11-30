#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofVideoGrabber vidGrabber;

		ofxCvColorImage			colorImg;

		static const int		MAX_CAM_CADR = 15;
		int						currentCount;
		ofxCvColorImage			backgroundImages[MAX_CAM_CADR];
		float					*middleValues;
		ofxCvColorImage			colorDiff;
		ofPixels				bufferArr; 

		void transformArray(ofPixels& pixels);
		void countMiddleValues();

		ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

		ofxCvContourFinder 	contourFinder;

		static const int threshold = 30;
		static const int step = 5;
		bool bLearnBakground;

		int camWidth;
		int camHeight;
		
};
