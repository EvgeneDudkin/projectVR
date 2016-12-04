#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	camWidth = 320;
	camHeight = 240;
	colorDiffPosX = 360;
	colorDiffPosY = 20;

	vidGrabber.setVerbose(true);
	vidGrabber.setup(camWidth, camHeight);

	colorImg.allocate(camWidth, camHeight);
	colorDiff.allocate(camWidth, camHeight);
	bitMask = new int[camWidth * camHeight];
	bitMaskStep = new int[camWidth / step * camHeight / step];
	//backgroundImage.allocate(camWidth, camHeight);
	//for (int i = 0; i < MAX_CAM_CADR; i++) {
		//backgroundImages[i].allocate(camWidth, camHeight);
	//}

	bLearnBackground = true; // allow learn background
}

void ofApp::countMiddleValues() {

	float r = 0, g = 0, b = 0;
	for (int w = 0; w < backgroundImages[0].getWidth(); w++) {
		for (int h = 0; h < backgroundImages[0].getHeight(); h++) {
			for (int i = 0; i < MAX_CAM_CADR; i++) {
				r += backgroundImages[i].getColor(w, h).r;
				g += backgroundImages[i].getColor(w, h).g;
				b += backgroundImages[i].getColor(w, h).b;
			}
			r = r / MAX_CAM_CADR;
			g = g / MAX_CAM_CADR;
			b = b / MAX_CAM_CADR;
			middleValues.setColor(w, h, ofColor(r, g, b));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(100, 100, 100);
	vidGrabber.update();

	if (!vidGrabber.isFrameNew())
		return;

	ofPixels pixels = vidGrabber.getPixels(); //get pixels from image
	colorImg.setFromPixels(pixels); //set first image

	if (bLearnBackground && cadrsCount < MAX_CAM_CADR) {
		backgroundImages[cadrsCount] = pixels;
		cadrsCount++;
	}
	
	if (bLearnBackground && cadrsCount == MAX_CAM_CADR) {
		bLearnBackground = false;
		middleValues = pixels;
		countMiddleValues();
		cadrsCount = 0;
		backgroundImage.setFromPixels(middleValues);

	}

	//if (!bLearnBackground) {
	//	bufferArr = pixels;
	//	for (int i = 0; i < camHeight; i++)
	//		for (int j = 0; j < camWidth; j++) {
	//			int k = 0;
	//			k += abs(middleValues.getColor(j, i).r - bufferArr.getColor(j, i).r) +
	//				abs(middleValues.getColor(j, i).g - bufferArr.getColor(j, i).g) +
	//				abs(middleValues.getColor(j, i).b - bufferArr.getColor(j, i).b);
	//			if (k <= threshold) {
	//				bufferArr.setColor(j, i, ofColor(255, 255, 255)); //set color of pixel to white
	//			}
	//		}

	//	colorDiff.setFromPixels(bufferArr);
	//}

	//Считаем матрицу отличных от фона пикселей
	if (!bLearnBackground) {
		bufferArr = pixels;
		for (int i = 1; i < camHeight - 1; i++)
			for (int j = 1; j < camWidth - 1; j++) {
				float k = 0;
				for (int j1 = 0; j1 < 3; j1++) {
					float dd = 0;
					for (int k1 = 0; k1 < 9; k1++) {
						if (j1 == 0)	dd += bufferArr.getColor((j - 1 + k1 % 3), (i - 1 + k1 / 3)).r;
						if (j1 == 1)	dd += bufferArr.getColor((j - 1 + k1 % 3), (i - 1 + k1 / 3)).g;
						if (j1 == 2)	dd += bufferArr.getColor((j - 1 + k1 % 3), (i - 1 + k1 / 3)).b;
					}
					dd /= 9;
					if (j1 == 0)	k += abs(middleValues.getColor(j, i).r - dd);
					if (j1 == 1)	k += abs(middleValues.getColor(j, i).g - dd);
					if (j1 == 2)	k += abs(middleValues.getColor(j, i).b - dd);
				}
				if (k <= threshold)
					bitMask[j + i * camWidth] = 1;
				else bitMask[j + i * camWidth] = 0;
			}

		//Обнуляем матрицу квадратов
		for (int i = 0; i < camHeight / step; i++)
			for (int j = 0; j < camWidth / step; j++)
				bitMaskStep[j + camWidth * i] = 0;
		
		//считаем заполненность квадратов
		for (int i = 1; i < camHeight - 1; i++)
			for (int j = 1; j < camWidth - 1; j++) {
				if (bitMask[j + i * camWidth]) {
					bufferArr.setColor(j, i, ofColor(255, 255, 255));
				} else {
					int k11 = j / step;
					int k12 = i / step;
					bitMaskStep[k11 + k12 * camWidth]++; //количество единиц в 1 квадрате
				}
			}

		//Рисуем квадраты
		for (int i = 0; i < camHeight / step; i++)
			for (int j = 0; j < camWidth / step; j++)
				if (bitMaskStep[j + camWidth * i] >= step*step / 2) {
					ofRect(colorDiffPosX + j * step, colorDiffPosY + i * step,
						colorDiffPosX + (j + 1) * step, colorDiffPosY + (i + 1) * step);
				}
		
		//Определяем зону кончика вверх
		int k11 = 0;
		for (int i = 0 ; i < camHeight / step; i++) {
			for (int j = 0; j < camWidth / step; j++)
				if (bitMaskStep[j + camWidth * i] >= step*step / 2 && bitMaskStep[j + camWidth * (i + 1)] >= step * step / 2
					&& bitMaskStep[j + camWidth * (i + 2)] >= step * step / 2 &&
					bitMaskStep[j + camWidth * (i - 1)] < step * step / 2 && bitMaskStep[j - 1 + camWidth * (i -
						1)] < step * step / 2 && bitMaskStep[j + 1 + camWidth * (i - 1)] < step * step / 2 &&
					bitMaskStep[j - 2 + camWidth * i] < step * step / 2 && bitMaskStep[j + 2 + camWidth * (i -
						1)] < step * step / 2)
				{
					//Нашли кончик
					ofRect(colorDiffPosX + j * step, colorDiffPosY + i * step,
						colorDiffPosX + (j + 1) * step, colorDiffPosY + (i + 1) * step);
					
					k11 = 1;
					//x = j * step + step / 2;
					//y = i * step + step / 2;
					break;
				}
			if (k11 == 1) break;
		}
		colorDiff.setFromPixels(bufferArr);	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetHexColor(0xffffff);
	colorImg.draw(20, 20);
	colorDiff.draw(colorDiffPosX, colorDiffPosY);
	backgroundImage.draw(20, 280);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case ' ':
		bLearnBackground = true;
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
