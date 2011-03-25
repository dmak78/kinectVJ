#include "DepthVisualizerInput.h"
#include "testApp.h"

const int useKinect = true;

bool DepthVisualizerInput::usingKinect(){
	return useKinect;
}

void DepthVisualizerInput::setup(ofxControlPanel& panel){
	this->panel = &panel;
	
	camWidth = 640;
	camHeight = 480;
	
	depthImage.allocate(camWidth, camHeight);
	animationImage.allocate(camWidth,camHeight);
	contourImage.allocate(camWidth,camHeight);
	tempImage.allocate(camWidth,camHeight);
	colorImage.allocate(camWidth,camHeight);
	
	bLearnBakground = true;
	
	animation.load("kinectScan");
	
	if(useKinect) {
		//kinect.init(true);  //shows infrared image
		kinect.init();
		kinect.setVerbose(true);
		kinect.open();
		// this is setting the raw data conversion range
		// we assume that 100 to 300 CM (1 - 3 meters) is good for tracking a person in space
		rawNearThreshold = 0;
		rawFarThreshold = 350;
		kinect.getCalibration().setClippingInCentimeters(rawNearThreshold, rawFarThreshold);
		
		
	} 
	//Setup the grids
	for(int y = 0; y < camHeight; y++) {
		for(int x = 0; x < camWidth; x++) {
			pointGrid[y][x].set(0, 0, 0);
			trackGrid[y][x].set(0, 0, 0);
			blobGrid[y][x].set(0, 0, 0);
			animationPointGrid[y][x].set(0,0,0);
			//colorGrid[y][x].set(0,0,0);
		}
	}
}

bool DepthVisualizerInput::update(){
	bool isFrameNew = false;

	stepSize = panel->getValueI("stepSize");

	//animation.load("kinectScan");
	cout << "Animation length is " << animation.size() << " frames." << endl;

		kinect.update();
		if(kinect.isFrameNew()) {
			isFrameNew = true;
			
			depthImage.setFromPixels(kinect.getDepthPixels(), camWidth, camHeight);
			contourImage.setFromPixels(kinect.getDepthPixels(), camWidth, camHeight);
			colorImage.setFromPixels(kinect.getCalibratedRGBPixels(), camWidth,camHeight);
			rawNearThreshold = panel->getValueI("nearClipping");
			rawFarThreshold = panel->getValueI("farClipping");
			kinect.getCalibration().setClippingInCentimeters(rawNearThreshold, rawFarThreshold);
			kinect.setCameraTiltAngle(panel->getValueI("camAngle"));
			
			depthImage.flagImageChanged();
			contourImage.flagImageChanged();
			
			if(animation.isFrameNew()){
			animation.setPlaySpeed(panel->getValueF("playSpeed"));
			
			
			ofImage& cur = animation.getAlpha();
			animationImage.setFromPixels(cur.getPixels(), cur.getWidth(), cur.getHeight());
			animationImage.flagImageChanged();
			
			}
		}
	 
	
	if(isFrameNew) {
		thresholdDepthImage();
		contourDepthImage();
		buildPointCloud();
		buildAnimationPointCloud();
	}
	
	return isFrameNew;
}

void DepthVisualizerInput::contourDepthImage(){
	//contourImage.absDiff(tempImage,depthImage);
	contourImage.threshold(panel->getValueI("contourThresh"));
	contourFinder.findContours(contourImage,panel->getValueI("contourMin"), panel->getValueI("contourMax"), panel->getValueI("blobs"), true);
}

void DepthVisualizerInput::thresholdDepthImage() {
	// do processing here on depthImage
	// getting rid of (setting depth = 0) on pixels we don't care about
	// for example, background, etc. 
	//
	unsigned char* depthPixels = depthImage.getPixels();
	unsigned char* animationPixels = animationImage.getPixels();
	unsigned char* trackPixels = contourImage.getPixels();
	unsigned char* colorPixels = colorImage.getPixels();

	int nearThresh = panel->getValueF("nearThreshold");
	int farThresh = panel->getValueF("farThreshold");
	
	for(int y = 0; y < camHeight; y++) {
		for(int x =0; x < camWidth; x++) {
			int i = y * camWidth + x;
			if (depthPixels[i] <  farThresh  || depthPixels[i] >= nearThresh){
				depthPixels[i] = 0;
				colorPixels[i] = 0;
			}
			if (animationPixels[i] <  farThresh  || animationPixels[i] >= nearThresh){
				animationPixels[i] = 0;
			}
			if (trackPixels[i] <  farThresh  || trackPixels[i] >= nearThresh){
				trackPixels[i] = 0;
			}
		}
	}
}

void DepthVisualizerInput::buildPointCloud() {	
	
	//depthImage.absDiff(animationImage);
	unsigned char* depthPixels = depthImage.getPixels();
	unsigned char* contourPixels = contourImage.getPixels();
	unsigned char* colorPixels = colorImage.getPixels();
	
	// generate the point cloud(s)
	
		pointCloud.clear();
		trackCloud.clear();
		// this offset will center the data on the center of the scene
		float offset = -(rawNearThreshold + rawFarThreshold) / 2;
		for(int y = 0; y < camHeight; y+=stepSize) {
			for(int x = 0; x < camWidth; x+=stepSize) {
				int i = y * camWidth + x;
				if(depthPixels[i] != 0) {
					ofxVec3f cur = kinect.getWorldCoordinateFor(x, y);
					ofColor color = kinect.getCalibratedColorAt(x ,y);
					cur *= 100; // convert from meters to cm
					cur.z += offset;
					
					pointCloud.push_back(cur);
					colorGrid[y][x].r = color.r;
					colorGrid[y][x].g = color.g;
					colorGrid[y][x].b = color.b;
					pointGrid[y][x].set(cur);
				} else {
					pointGrid[y][x].set(0, 0, 0);
					//colorGrid[y][x].set(0, 0, 0);
				}
				
				if(contourPixels[i] != 0) {
					ofxVec3f cur = kinect.getWorldCoordinateFor(x, y);
					cur *= 100; // convert from meters to cm
					cur.z += offset;
					
					trackCloud.push_back(cur);
					
					trackGrid[y][x].set(cur);
				} else {
					trackGrid[y][x].set(0, 0, 0);
				}
			}
		}
}

void DepthVisualizerInput::buildAnimationPointCloud(){
	
	unsigned char* animationPixels = animationImage.getPixels();
	double fx_d = 1.0 / 5.9421434211923247e+02;
	double fy_d = 1.0 / 5.9104053696870778e+02;
	float cx_d = 3.3930780975300314e+02;
	float cy_d = 2.4273913761751615e+02;
	
	animationPointCloud.clear();
	// this offset will center the data on the center of the scene
	float animationOffset = -(rawNearThreshold + rawFarThreshold) / 2;
	for(int y = 0; y < camHeight; y+=stepSize) {
		for(int x = 0; x < camWidth; x+=stepSize) {
			int i = y * camWidth + x;
			if(animationPixels[i] != 0) {
				// ok where are we: 
				float pct = animationPixels[i] / 255.0;
				
				float zval = rawNearThreshold + (1-pct) * (rawFarThreshold - rawNearThreshold);
				
				float z = zval / 100.0f;
				ofxVec3f cur;
				cur.x = float((x - cx_d) * z * fx_d);
				cur.y = float((y - cy_d) * z * fy_d);
				cur.z = z;
				
				//ofxVec3f cur = kinect.getWorldCoordinateFor((float)x, (float)y, zval/100.0f);
				cur *= 100; // convert from meters to cm
				cur.z += animationOffset;
				animationPointCloud.push_back(cur);
				
				animationPointGrid[y][x].set(cur);
			} else {
				animationPointGrid[y][x].set(0, 0, 0);
			}
		}
	}
}

void DepthVisualizerInput::drawOrthographic() {
	;  // this needs to be done, from the depth image. 
}

void DepthVisualizerInput::drawPerspective() {
	
//	ofSetColor(255,255,255,170);
	glPointSize(panel->getValueF("pointSize"));
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofxVec3f), &(pointCloud[0][0]));
	glDrawArrays(GL_POINTS, 0, pointCloud.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	
	
	//ofSetColor(0,0,255,170);
	
	glPointSize(panel->getValueF("pointSize"));
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofxVec3f), &(trackCloud[0][0]));
	glDrawArrays(GL_POINTS, 0, trackCloud.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	
	
	
	ofSetColor(255,255,255);
	
	/*
	if (panel->getValueB("drawSceneBox")){
		
		float offset = -(rawNearThreshold + rawFarThreshold) / 2;
		
		ofxPoint3f pts[8];
		
		pts[0] =  kinect.getCalibration().getWorldCoordinateFor(0, 0, rawNearThreshold/100.0);
		pts[1] =  kinect.getCalibration().getWorldCoordinateFor(camWidth, 0, rawNearThreshold/100.0);
		pts[2] =  kinect.getCalibration().getWorldCoordinateFor(camWidth, camHeight, rawNearThreshold/100.0);
		pts[3] =  kinect.getCalibration().getWorldCoordinateFor(0, camHeight, rawNearThreshold/100.0);
		pts[4] =  kinect.getCalibration().getWorldCoordinateFor(0, 0, rawFarThreshold/100.0);
		pts[5] =  kinect.getCalibration().getWorldCoordinateFor(camWidth, 0, rawFarThreshold/100.0);
		pts[6] =  kinect.getCalibration().getWorldCoordinateFor(camWidth, camHeight, rawFarThreshold/100.0);
		pts[7] =  kinect.getCalibration().getWorldCoordinateFor(0, camHeight, rawFarThreshold/100.0);
		
		
		for (int i = 0; i < 8; i++){
			pts[i]*= 100;
			pts[i].z += offset;
		}
		
		ofLine(pts[0], pts[1]);
		ofLine(pts[1], pts[2]);
		ofLine(pts[2], pts[3]);
		ofLine(pts[3], pts[0]);
		
		ofLine(pts[4], pts[5]);
		ofLine(pts[5], pts[6]);
		ofLine(pts[6], pts[7]);
		ofLine(pts[7], pts[4]);
		
		ofLine(pts[0], pts[4]);
		ofLine(pts[1], pts[5]);
		ofLine(pts[2], pts[6]);
		ofLine(pts[3], pts[7]);
	}
	*/
}

void DepthVisualizerInput::drawAnimation(){
	glPointSize(panel->getValueF("pointSize"));
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(ofxVec3f), &(animationPointCloud[0][0]));
	glDrawArrays(GL_POINTS, 0, animationPointCloud.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

void DepthVisualizerInput::drawDebug(){
	
	//ofTranslate((ofGetWidth() - camWidth) / 2, 0, 0);
	ofSetColor(255, 255, 255);
	for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(0,0);
    }

	ofPushMatrix();
		
	ofTranslate(camWidth/2.0, ofGetHeight()*0.85, -500);
		ofRotateY(panel->getValueF("rotateY"));
		if (panel->getValueB("autoRotate")){
			ofRotateY(ofGetElapsedTimef()*5);
		}
		ofScale(2,2,2);
	
		
		// need something here....
		drawPerspective();

	ofPopMatrix();
	
}

void DepthVisualizerInput::loadAnimation(){
	animation.unload();
	animation.load("kinectScan");
}


//--------------------------------------------------------------
void DepthVisualizerInput::exit() {
	if(useKinect) {
		kinect.close();
	}
}