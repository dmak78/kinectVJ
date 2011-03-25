#pragma once

#include "ofMain.h"

#include "ofxAutoControlPanel.h"
#include "DepthVisualizerInput.h"
#include "ofShader.h"
#include "ofxOsc.h"
#include "DepthExporter.h"

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
	
	ofxOscReceiver	receiver;
	int				current_msg_string;
	string		msg_strings[NUM_MSG_STRINGS];
	float			timers[NUM_MSG_STRINGS];
	

	
	bool recordButton;
	bool updateButton;
	bool rotateFollow;
	
	bool bButton;
	bool wiiControl;
	
	float record;
	float updates;
	
	float rotateYoffset;
	float rotateXoffset;
	float zoomOffset;
	
	float pitch;
	float yaw;
	
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
	
	
	
	float * left;
	float * right;
	float * outLeft;
	float * outRight;
	

	float audioLeft;
	float audioRight;
	float audioBuffer;
	int 	bufferCounter;
	int 	drawCounter;
	
	
};
