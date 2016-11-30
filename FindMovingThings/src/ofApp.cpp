#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 320;
	camHeight = 240;
	
	vidGrabber.setVerbose(true);
	vidGrabber.setup(camWidth, camHeight);

	colorImg.allocate(camWidth, camHeight);
	colorDiff.allocate(camWidth, camHeight);
	for (int i = 0; i < MAX_CAM_CADR; i++) {
		backgroundImages[i].allocate(camWidth, camHeight);
	}

	middleValues = new float[camWidth * camHeight * 3];
	//bufferArr = new ofColor[camWidth * camHeight];

	bLearnBakground = true; // allow learn background
	currentCount = 0;
}


void ofApp::countMiddleValues() {

	// Надо посчитать middleValues для каждых.
	// indexes are everywhere...
	
	for (int i = 0; i < camHeight * camWidth; i++)
		middleValues[i] = 0;

	for (int i1 = 0; i1 < MAX_CAM_CADR; i1++) {
		bufferArr = backgroundImages[i1].getPixels();
		for (int i = 0; i < camHeight; i++)
			for (int j = 0; j < camWidth; j++) {
				int ind = (i * camWidth + j) * 3;
				middleValues[(i * camWidth + j) * 3 + 0] += bufferArr.getColor(j, i).r;
				middleValues[(i * camWidth + j) * 3 + 1] += bufferArr.getColor(j, i).g;
				middleValues[(i * camWidth + j) * 3 + 2] += bufferArr.getColor(j, i).b;
			}
	}
	for (int i = 0; i < camHeight * camWidth * 3; i++)
		middleValues[i] = middleValues[i] / MAX_CAM_CADR;
}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(100, 100, 100);
	vidGrabber.update();

	if (!vidGrabber.isFrameNew())
		return;

	ofPixels pixels = vidGrabber.getPixels(); //get pixels from image
	colorImg.setFromPixels(pixels); //set first image

	if (bLearnBakground && currentCount < MAX_CAM_CADR) { //if buffer is not full
		backgroundImages[currentCount] = colorImg;
		currentCount++;
		return;
	}

	//if buffer is full, we are ready...
	if (bLearnBakground && currentCount == MAX_CAM_CADR) { 
		bLearnBakground = false;
		currentCount = 0;
		countMiddleValues();
		return;
	}

	//...for this
	bufferArr = pixels;
	for (int i = 0; i < camHeight; i++)
		for (int j = 0; j < camWidth; j++) {
			int k = 0;
			int ind = i * camWidth + j; //current pixel
			k += abs(middleValues[ind * 3 + 0] - bufferArr.getColor(j, i).r) +
				 abs(middleValues[ind * 3 + 1] - bufferArr.getColor(j, i).g) +
				 abs(middleValues[ind * 3 + 2] - bufferArr.getColor(j, i).b);
			if (k <= threshold) {
				bufferArr.setColor(j, i, ofColor(255, 255, 255)); //set color of pixel to white
			}
		}

	colorDiff.setFromPixels(bufferArr);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetHexColor(0xffffff);
	colorImg.draw(20, 20);
	colorDiff.draw(360, 20);
	backgroundImages[0].draw(20, 280);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case ' ':
		bLearnBakground = true;
		break;
	case 'y':
		for (int i = 0; i < camWidth; i++)
			for (int j = 0; j < camHeight; j++) {
				std::string str1 = ("i = ");
				str1 += i;
				str1 += ", j = ";
				str1 += j;
				ofLogWarning("TAG", str1);
			}
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
