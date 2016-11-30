#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 320;
	camHeight = 240;
	
	vidGrabber.setVerbose(true);
	vidGrabber.setup(camWidth, camHeight);

	colorImg.allocate(camWidth, camHeight);
	colorDiff.allocate(camWidth, camHeight);
	backgroundImage.allocate(camWidth, camHeight);

	bLearnBakground = true; // allow learn background
}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(100, 100, 100);
	vidGrabber.update();

	if (!vidGrabber.isFrameNew())
		return;

	ofPixels pixels = vidGrabber.getPixels(); //get pixels from image
	colorImg.setFromPixels(pixels); //set first image

	if (bLearnBakground) {
		middleValues = pixels;
		bLearnBakground = false;
		backgroundImage.setFromPixels(middleValues);
	}

	bufferArr = pixels;
	for (int i = 0; i < camHeight; i++)
		for (int j = 0; j < camWidth; j++) {
			int k = 0;
			k += abs(middleValues.getColor(j, i).r - bufferArr.getColor(j, i).r) +
				abs(middleValues.getColor(j, i).g - bufferArr.getColor(j, i).g) +
				abs(middleValues.getColor(j, i).b - bufferArr.getColor(j, i).b);
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
	backgroundImage.draw(20, 280);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case ' ':
		bLearnBakground = true;
		break;
	case 'p':
		std::string str1 = " ";
		str1 += std::to_string(middleValues.getBytesPerPixel());
		str1 += ", ";
		str1 += std::to_string(middleValues.getBytesStride());
		ofLogWarning("TAG", str1);
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
