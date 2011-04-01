#pragma once

#include "ofMain.h"

#include "ofxAutoControlPanel.h"
#include "DepthVisualizerInput.h"
#include "ofShader.h"
#include "ofxOsc.h"
#include "DepthExporter.h"
#include "ofxSyphon.h" 

#define PORT 9000
#define NUM_MSG_STRINGS 20

class testApp : public ofBaseApp {
	
	public:

		void setup();
		void update();
		void draw();
		void exit();
	
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		ofxAutoControlPanel panel;
	
	ofxSyphonServer mainOutputSyphonServer;
	ofxSyphonServer individualTextureSyphonServer;
	
	ofxSyphonClient mClient;
	
	ofxOscReceiver	receiver;
	int				current_msg_string;
	string		msg_strings[NUM_MSG_STRINGS];
	float			timers[NUM_MSG_STRINGS];
	

	
	bool recordButton;
	bool updateButton;
	bool rotateFollow;
	
	bool bButton;
	bool aButton;
	bool downButton;
	bool upButton;
	bool homeButton;
	bool leftButton;
	bool rightButton;
	bool plusButton;
	bool minusButton;
	bool wiiControl;
	
	float record;
	float updates;
	
	float rotateYoffset;
	float rotateXoffset;
	float zoomOffset;
	float wiiRotateYoffset;
	float wiiZoomOffset;
	float wiiRotateXoffset;
	
	float pitch;
	float yaw;
	float roll;
	float accel;
	float accelX;
	
	int nPoints;
	
	float * fftSmoothed;

	DepthVisualizerInput input;
	
	bool needsUpdate;
		
	ofShader dofPoints, dofLines, otherShader;
	
	ofPoint minBound, maxBound;
	ofPoint centroid;
	
	ofPoint tracker;
	
	//ofVideoPlayer video;
	
	void audioReceived 	(float * input, int bufferSize, int nChannels); 
	void audioRequested 	(float * input, int bufferSize, int nChannels); 
	
	
	
	double * left;
	double * right;

	

	double audioLeft;
	double audioRight;
	float audioBuffer;
	int 	bufferCounter;
	int 	drawCounter;
	
	int blendL[6]; // an array to store all the blending values
	int blendR[6]; // an array to store all the blending values
	int currentL, currentR; // indexes to store the current blending value
	
	
};
