#pragma once

#include "ofMain.h"
#include "ofxAutoControlPanel.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "scanDataPlayer.h"
#include "ofx3dGraphics.h"

class DepthVisualizerInput {
	
public: 
	
	// you can get three things from this class
	
	// 1) grayscale depth image
	ofxCvGrayscaleImage depthImage;
	ofxCvGrayscaleImage	animationImage;
	ofxCvGrayscaleImage contourImage;
	ofxCvGrayscaleImage tempImage;
	ofxCvColorImage colorImage;
	
	// 2) point cloud of projected points
	vector<ofPoint> pointCloud;
	
	vector<ofPoint> trackCloud;
	
	vector<ofPoint> blobCloud;
	
	vector<ofPoint> danceCloud;
	
	vector<ofxVec3f> animationPointCloud;
	
	// 3) same as above, but structured as a grid
	//    empty points are (0,0,0)
	ofxVec3f pointGrid[480][640];
	ofxVec3f animationPointGrid[480][640];
	ofxVec3f blobGrid[480][640];
	ofxVec3f trackGrid[480][640];
	ofColor colorGrid[480][640];
	
	int camWidth, camHeight;
	float rawFarThreshold, rawNearThreshold;
	int stepSize;
	bool bLearnBakground;
	
	ofxCvContourFinder contourFinder;

	
	ofxKinect kinect;
	scanDataPlayer animation;
	
	ofxControlPanel* panel;
	void setup(ofxControlPanel& panel);
	bool update(); // returns true if this is a new frame
	void drawDebug();
	void drawOrthographic();
	void drawPerspective();
	void drawAnimation();
	void loadAnimation();
	void contourDepthImage();
	
	bool usingKinect();
	void exit();
	
protected:
	
	void thresholdDepthImage();
	void buildPointCloud();
	void buildAnimationPointCloud();
	
};