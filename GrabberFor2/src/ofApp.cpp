#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 320;
	camHeight = 240;

	vector<ofVideoDevice> devices = vidGrabber[0].listDevices();

	for (int i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	for (int i = 0; i < numOfCams; i++) {
		vidGrabber[i].setDeviceID(i);
		vidGrabber[i].setDesiredFrameRate(60);
		vidGrabber[i].initGrabber(camWidth, camHeight);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100, 100, 100);
	for (int i = 0; i < numOfCams; i++) {
		vidGrabber[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xffffff);
	int x = 20;
	int y = 20;
	for (int i = 0; i < numOfCams; i++) {
		vidGrabber[i].draw(x, y);
		x += camWidth;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
