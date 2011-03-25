#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	
		
	//Setup for sound input
	ofSoundStreamSetup(2,2,this, 44100, 256, 4);	
	left = new float[256];
	right = new float[256];
	outLeft = new float[256];
	outRight = new float[256];
	
	fftSmoothed = ofSoundGetSpectrum(128);


	bufferCounter = 0;
	drawCounter = 0;

	//Setup for OSC Port(s)
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );
	current_msg_string = 0;
	
	//boolean setups
	recordButton=false;
	updateButton=false;
	rotateFollow=true;
	wiiControl=false;
	
	input.setup(panel);  // < -- could pass in useKinnect here?
	
	panel.setup("Control Panel", 0, 5, 500, 850);
	panel.setBackgroundColor(simpleColor(0,0,0,40));
	panel.addPanel("Threshold and Scale", 2);
	panel.addSlider("near clipping", "nearClipping", 0, 0, 400, true);
	panel.addSlider("far clipping", "farClipping", 350, 0, 400, true);
	panel.addSlider("near threshold", "nearThreshold", 255, 0, 255, true);
	panel.addSlider("far threshold", "farThreshold", 0, 0, 255, true);
	panel.addSlider("contour threshold", "contourThresh", 255,0,255,true);
	panel.addSlider("contourMin","contourMin", 40,40,5000,true);
	panel.addSlider("contourMax","contourMax", 30000,5000,30000,true);
	panel.addSlider("number of blobs", "blobs", 1,0,100, true);
	
	
	panel.setWhichColumn(1);
	panel.addSlider("scale", "scale", 7, 1, 10);
	panel.addSlider("rotate x axis", "rotateX",0,-45,45,true);
	panel.addSlider("rotate y axis", "rotateY", 0, -360, 360, false);	
	panel.addSlider("center point", "centerPoint", 0, -1000,1000,true);
	panel.addSlider("camera angle", "camAngle", 0,-30,24,true);
	panel.addSlider("x position", "xPosition", ofGetWidth()/2, 0, ofGetWidth(), true);
	panel.addSlider("y position", "yPosition", ofGetHeight()/2, -ofGetHeight()/2, ofGetHeight(), true);
	panel.addSlider("point spacing size", "stepSize", 2, 1, 100, true);
	panel.addToggle("animation","animation",true);
	panel.addToggle("pointCloud","pointCloud",true);


	
	panel.addPanel("Interaction",2);
	panel.addSlider2D("cameraControl","cameraControl",0,0,-90,90,-45,45,true);
	panel.addSlider("zoom", "zoom", -500,-500,500, true);
	panel.addSlider2D("light position", "lightPosition", 1,1,-20,20,-20,20,false);
	panel.addSlider("lightZ","lightZ",0,-50,50,false);
	panel.addToggle("use WII", "wiiControl",false);
	panel.addToggle("Kinect RGB", "rgb", true);
	panel.addToggle("GL Material", "glMatOn",true);
	panel.addSlider("number of points","numPoints",6,1,6,true);
	panel.addToggle("points", "points", false);
	panel.addToggle("lines", "lines", false);
	panel.addToggle("line loop", "lineLoop", false);
	
	

	panel.setWhichColumn(1);
	
	panel.addSlider2D("soundControl","soundControl",1,0,0,5,1,0,false);//---------------------RIGHT HERE!
	panel.addToggle("control z","z",true);
	panel.addToggle("control x","x",true);
	panel.addToggle("control y","y",true);
	panel.addToggle("control rotation","cRotation",false);
	panel.addSlider2D("spacing and size", "spaceSize", 2,1,1,20,1,30,true);
		panel.addToggle("line strip", "lineStrip", false);
	panel.addToggle("quads", "quads", false);
	panel.addToggle("quad strip", "quadStrip", false);
	panel.addToggle("triangles", "triangles", false);
	panel.addToggle("triangle strip", "triangleStrip", false);
	panel.addToggle("triangle strip", "triangleFan", false);
	

	panel.addPanel("Lights",2);
	panel.addToggle("GL Lighting", "glLightingOn",true);
	panel.addSlider("shininess","shininess",0,0,100,false);
	panel.addSlider("ambientRed","ambientRed",0,0,1,false);
	panel.addSlider("ambientGreen","ambientGreen",0,0,1,false);
	panel.addSlider("ambientBlue","ambientBlue",0,0,1,false);
	panel.addSlider("diffuseRed","diffuseRed",0,0,1,false);
	panel.addSlider("diffuseGreen","diffuseGreen",0,0,1,false);
	panel.addSlider("diffuseBlue","diffuseBlue",0,0,1,false);
	panel.addSlider("specularRed","specularRed",0,0,1,false);
	panel.addSlider("specularGreen","specularGreen",0,0,1,false);
	panel.addSlider("specularBlue","specularBlue",0,0,1,false);

	panel.setWhichColumn(1);
	panel.addToggle("auto rotateY", "autoRotateY", false);
	panel.addToggle("auto rotateX", "autoRotateX", false);
	panel.addToggle("auto rotateZ", "autoRotateZ", false);
	panel.addToggle("draw debug", "drawDebug", false);
	panel.addToggle("draw axis", "drawAxis", true);
	panel.addToggle("record", "doRecording", false);
	panel.addToggle("reloadAnimation","reloadAnimation",false);
	panel.addToggle("export ply", "exportPoly", false);
	panel.addToggle("export dance ply", "exportDancePoly", false);
	

		panel.addSlider("playback speed", "playSpeed", 0.5, -1, 1, false);
		panel.addToggle("playbackPause", "playbackPause", false);
	
	
	panel.addPanel("DOF");
	panel.addToggle("glOn","glOn",true);
	panel.addToggle("usePoints", "usePoints", true);	
	panel.addSlider("focusDistance", "focusDistance", 1300, 0, 1600);
	panel.addSlider("aperture", "aperture", 0.06, 0.001, 0.2);
	panel.addSlider("pointBrightness", "pointBrightness", .8, 0, 1);
	panel.addSlider("rgbBrightness", "rgbBrightness", 4, 0, 10);
	panel.addSlider("maxPointSize", "maxPointSize", 20, 5, 40);
	panel.addToggle("spiralLines", "spiralLines", false);
	panel.addSlider("maxLineLength", "maxLineLength", 50, 0, 100);
	panel.addSlider("maxLines", "maxLines", 10, 0, 100000);
	
		
	dofPoints.setup("shaders/DOFCloud");
	
	dofLines.setup("shaders/DOFLine");
	otherShader.setup("shaders/gradient");
	
	needsUpdate=false;
	
	panel.reloadSettings();
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	
	int nBandsToGet = 128;
	int numberOfFreqsToGet = 256;
	float * val = ofSoundGetSpectrum(nBandsToGet);// request 128 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		// smooth the values out a little
		fftSmoothed[i] *= 0.96f;
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
	}
	
	rotateYoffset=fftSmoothed[100]*200;
	
	vector<ofPoint>& pointCloud = input.pointCloud;
	vector<ofPoint>& danceCloud = input.danceCloud;

	if (panel.getValueB("exportPoly")){
		
		DepthExporter::exportCloud("out.ply", pointCloud);
		
		panel.setValueB("exportPoly", false);
	}
	if (panel.getValueB("exportDancePoly")){
		
		DepthExporter::exportCloud("outDance.ply", pointCloud);
		
		panel.setValueB("exportDancePoly", false);
	}
	
	
	while( receiver.hasWaitingMessages() )
	{
		
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		for ( int i=0; i<m.getNumArgs(); i++ )
		{
			if(m.getAddress() == "/wii/1/button/B"){
				
				if( m.getArgAsFloat(i)==1.0){
					bButton=true;
				}
				else{
					bButton=false;
				}
			}
			if(m.getAddress() == "/wii/1/accel/pry/0"){
				pitch = m.getArgAsFloat(i);
				pitch*=100;
				
				
			}
			if(m.getAddress() == "/wii/1/accel/pry/2"){
				yaw = m.getArgAsFloat(i);
				yaw*=100;
				
			}

		}
	}



	
	
	if(input.contourFinder.nBlobs >=2){
		float avex=0.0;
		float avey=0.0;
		for (int i = 0; i < input.contourFinder.nBlobs; i++){
			avex+=input.contourFinder.blobs[i].centroid.x;
			avex/=input.contourFinder.nBlobs;
			avey+=input.contourFinder.blobs[i].centroid.y;
			avey/=input.contourFinder.nBlobs;
			//rotateYoffset=((avex)*.05);
			//rotateXoffset=((avey)*.05);
		}
	}
	/*
	for (int i = 0; i < 256; i++){
	
		
		
		audioLeft=left[i];
		audioRight=right[i];
		if(panel.getValueB("cRotation")){
			rotateYoffset=audioRight*panel.getValueF("soundControl",0)*10;
		}
		audioLeft=ofMap(audioLeft,-1,1,0,1);
		if(audioLeft>panel.getValueF("soundControl",1)){
			audioLeft*=4;
		}
		else{
			audioLeft/=2;
		}
		if(audioRight>panel.getValueF("soundControl",1)){
			audioRight*=4;
		}
		else{
			audioRight/=2;
		}
		
		}
	*/
	
	
	if(panel.getValueB("wiiControl")){
		rotateYoffset=ofMap(yaw,0,100,-90,90);
	zoomOffset=ofMap(pitch,0,100,-500,500);
	}
	
	audioRight*=panel.getValueF("soundControl",0);
	audioLeft*=panel.getValueF("soundControl",0);
	input.update();
	//ofSoundUpdate();
	
}


//--------------------------------------------------------------
void testApp::draw() {
	
	ofBackground(0, 0, 0);
	
	// draw debug or non debug
	
	if (panel.getValueB("drawDebug")){
		input.drawDebug();
		
	} else {
		
				
		ofPushMatrix();
			// center everything
		ofPushMatrix();
		ofTranslate(ofGetHeight()/2,ofGetHeight()/2,panel.getValueI("zoom")+zoomOffset);
		ofPushMatrix();
		ofTranslate(panel.getValueI("xPosition"),panel.getValueI("yPosition"));
		
		if (panel.getValueB("autoRotateY")){
			ofRotateY(ofGetElapsedTimef()*50);
			
		}
		 
			ofRotateY((panel.getValueI("cameraControl",0)-180)-rotateYoffset);
			ofRotateX(panel.getValueI("cameraControl",1)+rotateXoffset);
		
		
		if (panel.getValueB("autoRotateX")){
			ofRotateX(ofGetElapsedTimef()*100);
		}
		if (panel.getValueB("autoRotateZ")){
			ofRotateZ(ofGetElapsedTimef()*100);
		}
		
		
		if(panel.getValueB("drawAxis")){
			ofPushMatrix();
			ofRotateX(-panel.getValueI("rotateX"));
			ofSetColor(255,0,0);
			ofLine(0,0,0,0,200,0);
			ofLine(0,0,0,0,-200,0);
			ofSetColor(0,255,0);
			ofLine(0,0,0,200,0,0);
			ofLine(0,0,0,-200,0,0);
			ofSetColor(0, 0, 255);
			ofLine(0,0,0,0,0,200);
			ofLine(0,0,0,0,0,-200);
			ofSetColor(80,80,80);
			ofTranslate(0,200,0);
			ofRotateX(-90);
			ofCircle(0,0,200);
			ofSetColor(255,255,255);
			ofPopMatrix();
		}
		
		ofTranslate(0,0,panel.getValueI("centerPoint"));
		ofRotateX(panel.getValueI("rotateX"));
		
		float scale = (panel.getValueF("scale"));
		

		
		ofScale(scale+fftSmoothed[50]*20, scale, scale);
		
		
		
		if(panel.getValueB("glOn")){	
			
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);
			// super helpful: http://pmviewer.sourceforge.net/method.php
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
			
			if(panel.getValueB("usePoints")) {
				
				dofPoints.begin();
				
				dofPoints.setUniform1f("focusDistance", panel.getValueF("focusDistance"));
				dofPoints.setUniform1f("aperture", panel.getValueF("aperture"));
				dofPoints.setUniform1f("pointBrightness", panel.getValueF("pointBrightness"));
				dofPoints.setUniform1f("rgbBrightness", panel.getValueF("rgbBrightness"));
				dofPoints.setUniform1f("maxPointSize", panel.getValueF("maxPointSize"));

				if(panel.getValueB("pointCloud")){
					ofSetColor(255,255,255);
					input.drawPerspective();
				}
				
				if(panel.getValueB("animation")){
					ofSetColor(255,255,255);
					input.drawAnimation();
				}
				
				
				dofPoints.end();
			
				
			} else {
				dofLines.begin();
				//otherShader.begin();		
				dofLines.setUniform1f("aspectRatio", ofGetWidth() / ofGetHeight());
				dofLines.setUniform1f("lineWidth", 1);
				dofLines.setUniform1f("focusDistance", panel.getValueF("focusDistance"));
				dofLines.setUniform1f("aperture", panel.getValueF("aperture"));
				dofPoints.setUniform1f("pointBrightness", panel.getValueF("pointBrightness"));
				dofPoints.setUniform1f("rgbBrightness", panel.getValueF("rgbBrightness"));
				
				GLint sideLocation = dofLines.getAttributeLocation("side");
				GLint nextLocation = dofLines.getAttributeLocation("next");
				
				vector<ofxVec3f>& points = input.animationPointCloud;
				vector<ofPoint>& points2 = input.pointCloud;
				
				float maxLineLength = panel.getValueF("maxLineLength");
				float maxLines = panel.getValueF("maxLines");
				
				ofSeedRandom(0); // same random numbers every frame
				if(panel.getValueB("animation")){
					ofSetColor(153,0,26);
					input.drawAnimation();
				}
				if(panel.getValueB("pointCloud")){
					ofSetColor(255,255,255);
					input.drawPerspective();
				}
				int n = points.size();
				int n2 = points2.size();
				
				if(panel.getValueB("spiralLines")) {
					glBegin(GL_TRIANGLE_FAN);
										
					float w = input.camWidth;
					float h = input.camHeight;
					float steps = 100.;
					for(int i = 0; i < maxLines; i++) {
					
						float theta = i / steps;
						float r = (input.camHeight / 2) * ((float) i / maxLines);
						int x = (w / 2) + r * cos(theta);
						int y = (h / 2) + r * sin(theta);
						x = ofClamp(x, 0, w);
						y = ofClamp(y, 0, h);
						
						float ntheta = (i + 1) / steps;
						float nr = (input.camHeight / 2) * ((float) (i + 1) / maxLines);
						int nx = (w / 2) + nr * cos(ntheta);
						int ny = (h / 2) + nr * sin(ntheta);
						nx = ofClamp(nx, 0, w);
						ny = ofClamp(ny, 0, h);
						
						ofPoint& cur = input.pointGrid[y][x];
						ofxVec3f& next = input.animationPointGrid[ny][nx];
						
						// simple check to make sure we're not at 0,0,0
						if(next.z != 0 && cur.z != 0) {
							// place two vertices to represent the top and bottom
							// at this vertex
							dofLines.setAttribute1f(sideLocation, -.5);
							dofLines.setAttribute3f(nextLocation, next.x, next.y, next.z);
							glVertex3f(cur.x, cur.y, cur.z); // top vertex
							dofLines.setAttribute1f(sideLocation, +.5);
							dofLines.setAttribute3f(nextLocation, next.x, next.y, next.z);
							glVertex3f(cur.x, cur.y, cur.z); // bottom vertex
						}
					}
					glEnd();
					
				} else {
					
					for(int i = 0; i < maxLines; i++) {
						
						ofxVec3f& cur = points[(int) ofRandom(0, n)];
						ofPoint& next = points2[(int) ofRandom(0, n2)];
				
						float lineLength = cur.distance(next);
						
						glBegin(GL_TRIANGLE_STRIP);
						if(lineLength < maxLineLength) {
							ofSetColor(0,204,51,180);
							// place two vertices to represent the top and bottom
							// of the beginning of the line
							dofLines.setAttribute1f(sideLocation, -.5);
							dofLines.setAttribute3f(nextLocation, next.x, next.y, next.z);
							glVertex3f(cur.x, cur.y, cur.z); // top vertex
							dofLines.setAttribute1f(sideLocation, +.5);
							dofLines.setAttribute3f(nextLocation, next.x, next.y, next.z);
							glVertex3f(cur.x, cur.y, cur.z); // bottom vertex

							
							
							
							
							// extrapolate where the line would go next
							ofxVec3f after = next + (next - cur);
							
							// place two vertices to represent the top and bottom
							// of the end of the line
							dofLines.setAttribute1f(sideLocation, -.5);
							dofLines.setAttribute3f(nextLocation, after.x, after.y, after.z);
							glVertex3f(next.x, next.y, next.z); // top vertex
							dofLines.setAttribute1f(sideLocation, +.5);
							dofLines.setAttribute3f(nextLocation, after.x, after.y, after.z);
						glVertex3f(next.x, next.y, next.z); // bottom vertex
													}
						glEnd();
					
					}
				}
				//otherShader.end();
				
			dofLines.end();
				if(panel.getValueB("animation")){
					ofSetColor(153,0,26,120);
					input.drawAnimation();
				}
				if(panel.getValueB("pointCloud")){
					ofSetColor(255,255,255,70);
					input.drawPerspective();
				}
				
			}
		}
		else{
			
			if(panel.getValueB("pointCloud")){
				ofSetColor(255,255,255,200);
				input.drawPerspective();
			}
			 
			
			if(panel.getValueB("animation")){
				ofSetColor(140,0,26,255);
				input.drawAnimation();
			}
			
			int w = input.camWidth;
			int h = input.camHeight;
			
			int vertexStep = panel.getValueI("spaceSize",1);
			
			int xMax = w - vertexStep;
			int yMax = h - vertexStep;
			
			if(panel.getValueB("glLightingOn")){
			glEnable(GL_LIGHTING);
			}
			glEnable(GL_DEPTH_TEST);
			if(panel.getValueB("glMatOn")){
			glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
			glEnable ( GL_COLOR_MATERIAL ) ;
			}
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_LIGHT0);
			glEnable(GL_NORMALIZE);
			//glBlendFunc(GL_SRC_ALPHA,GL_DST_COLOR);
			//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);//maybe
			//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);//good
			//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA);//so soo
			//glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);//interesting
			//glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);//also interesting
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
			
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
			 			
			
			GLfloat ambientLight[] = {panel.getValueF("ambientRed"),panel.getValueF("ambientGreen"),panel.getValueF("ambientBlue"),1};
			GLfloat diffuseLight[] = {panel.getValueF("diffuseRed"),panel.getValueF("diffuseGreen"),panel.getValueF("diffuseBlue"),1};
			GLfloat specularLight[] = {panel.getValueF("specularRed"),panel.getValueF("specularGreen"),panel.getValueF("specularBlue"),1};
			GLfloat position[] = {panel.getValueF("lightPosition", 0),panel.getValueF("lightPosition",1),panel.getValueF("lightZ")};
			
		
			
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
			glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
			glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
			glLightfv(GL_LIGHT0, GL_POSITION, position);
			
			
			GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1 };
			GLfloat low_shininess[] = { panel.getValueF("shininess")};
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
			GLfloat mat_emission[] = {0.0, 0.0, 0.0, 1.0};
			glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
			
			
			if(panel.getValueB("pointCloud")){
				
				input.drawPerspective();
			}
			else{
				
			glPointSize(panel.getValueF("spaceSize",0));
			
				if(panel.getValueB("points")){
					glBegin(GL_POINTS);
				}
				if(panel.getValueB("quads")){
					glBegin(GL_QUADS);
				}
				if(panel.getValueB("quadStrip")){
					glBegin(GL_QUAD_STRIP);
					
				}
				if(panel.getValueB("triangles")){
					glBegin(GL_TRIANGLES);
				}
				if(panel.getValueB("triangleFan")){
					glBegin(GL_TRIANGLE_FAN);
				}
				if(panel.getValueB("triangleStrip")){
					glBegin(GL_TRIANGLE_STRIP);
				}
				if(panel.getValueB("lines")){
					glBegin(GL_LINES);
					
				}
				if(panel.getValueB("lineLoop")){
					glBegin(GL_LINE_LOOP);
					
				}
				if(panel.getValueB("lineStrip")){
					glBegin(GL_LINE_STRIP);
					
				}
			ofxVec3f zero(0, 0, 0);
		
			for(int y = 0; y < yMax; y += vertexStep) {
				for(int x = 0; x < xMax; x+= vertexStep) {
					
					
					ofxVec3f& nw = input.pointGrid[y][x];
			
					ofxVec3f& ne = input.pointGrid[y][x + vertexStep];
				
					ofxVec3f& se = input.pointGrid[y + vertexStep][x + vertexStep];
			
					ofxVec3f& sw = input.pointGrid[y + vertexStep][x];
				
					ofColor color = input.kinect.getCalibratedColorAt(x,y);
					
					int soundScalar = panel.getValueF("soundControl",0);
					float soundSense = panel.getValueF("soundControl",1);
					
				
				
					if(nw != zero && ne != zero && sw != zero && se != zero) {
						
						ofxVec3f right = ne - nw;
						ofxVec3f down = sw - nw;
						ofxVec3f out = down.cross(right);
						
						input.danceCloud.clear();
						
						if(panel.getValueB("z")){
						nw.z-=audioLeft;
						sw.z-=audioLeft;
						se.z-=audioLeft;
						ne.z-=audioLeft;
						}
						if(panel.getValueB("x")){
						nw.x-=audioRight;
						sw.x-=audioRight;
						se.x+=audioRight;
						ne.x+=audioRight;
						}
						if(panel.getValueB("y")){
						nw.y-=audioRight;
						sw.y-=audioRight;
						se.y+=audioRight;
						ne.y+=audioRight;
						}
						
						input.danceCloud.push_back(nw);
						input.danceCloud.push_back(sw);
						input.danceCloud.push_back(ne);
						input.danceCloud.push_back(se);
					
						
						float red = ofRandom(0,255);
						float green = ofRandom(0,255);
						float blue = ofRandom(0,255);
						float alpha = ofRandom(140,255);
						
						
					
						glNormal3f(out.x, out.y, out.z);
						
						if(panel.getValueB("rgb")){
						ofSetColor(color.r,color.g,color.b);
						}
						else if(panel.getValueB("glMatOn")){
							ofSetColor(255,255,255);
						}
						else{
							ofSetColor(red,green,blue);
						} 
						
					//glColor3f(out.x, out.y, out.z);
					//	glVertex3f(out.x, out.y, out.z);
					//	glVertex3f(right.x, right.y, right.z);
					//	glVertex3f(down.x, down.y, down.z);
						if(panel.getValueI("numPoints")==6){
							glVertex3f(nw.x, nw.y, nw.z);
							glVertex3f(ne.x, ne.y, ne.z);
							glVertex3f(se.x, se.y, se.z);
							glVertex3f(nw.x, nw.y, nw.z);
							glVertex3f(sw.x, sw.y, sw.z);
							glVertex3f(se.x, se.y, se.z);
							
							
						}
						if(panel.getValueI("numPoints")==5){
							glVertex3f(nw.x, nw.y, nw.z);
							glVertex3f(se.x, se.y, se.z);
							glVertex3f(ne.x, ne.y, ne.z);
							glVertex3f(sw.x, sw.y, sw.z);
							glVertex3f(nw.x, nw.y, nw.z);
						}
						if(panel.getValueI("numPoints")==4){
							glVertex3f(nw.x, nw.y, nw.z);
							glVertex3f(sw.x, sw.y, sw.z);
							glVertex3f(se.x, se.y, se.z);
							glVertex3f(ne.x, ne.y, ne.z);
						}
						if(panel.getValueI("numPoints")==3){
							glVertex3f(nw.x, nw.y, nw.z);
							glVertex3f(sw.x, sw.y, sw.z);
							glVertex3f(se.x, se.y, se.z);
							
						}
						if(panel.getValueI("numPoints")==2){
							glVertex3f(nw.x, nw.y, nw.z);
							glVertex3f(sw.x, sw.y, sw.z);
						}
						if(panel.getValueI("numPoints")==1){
							glVertex3f(nw.x, nw.y, nw.z);	
						}
					}
				}
			}
			}
			glEnd();
		
			if(panel.getValueB("glLightingOn")){
			glDisable(GL_LIGHTING);
			}
			if(panel.getValueB("glMatOn")){
				glEnable ( GL_COLOR_MATERIAL );
			}
			
			glDisable(GL_DEPTH_TEST);

			for (int i = 0; i < input.contourFinder.nBlobs; i++){
				float offset = -(panel.getValueI("nearClipping") + panel.getValueI("farClipping") / 2);
				int x = input.contourFinder.blobs[i].centroid.x;
				int y = input.contourFinder.blobs[i].centroid.y;
				int bx = input.contourFinder.blobs[i].boundingRect.x;
				int by = input.contourFinder.blobs[i].boundingRect.y;
				int w = input.contourFinder.blobs[i].boundingRect.width;
				int h = input.contourFinder.blobs[i].boundingRect.height;
				
				
				ofxVec3f cur = input.kinect.getWorldCoordinateFor(x, y);
				ofxVec3f blob = input.kinect.getWorldCoordinateFor(bx, by);
				
				if(cur.z!=0){
				cur *= 100; // convert from meters to cm
				cur.z += offset;
				blob *= 100; // convert from meters to cm
				blob.z += offset;
				ofNoFill();
				ofSetColor(255,255,255,200);
					
					ofBox(cur,(w)/scale);
					
					ofFill();
					ofSetColor(255,0,0,200);
					ofSphere(cur,.5);
				}
			}				
		}
				ofPopMatrix();	
				ofPopMatrix();
				ofPopMatrix();
	}
	
/*
	if (panel.getValueB("doRecording") || recordButton==true){
		static int counter= 0;
		
		ofImage temp;
		temp.allocate(640,480, OF_IMAGE_COLOR_ALPHA);
		unsigned char * colorAlphaPix = temp.getPixels();
		unsigned char * pix = input.depthImage.getPixels();
		for(int i = 0; i < 640*480; i++){
			colorAlphaPix[i*4] = pix[i];
			colorAlphaPix[i*4+1] = pix[i];
			colorAlphaPix[i*4+2] = pix[i];
			colorAlphaPix[i*4+3] = pix[i];
		}
		counter++;
		
		temp.saveImage("kinectScan/output_" + ofToString(counter) + ".png");
		ofSetColor(255,0,0,200);
		ofCircle(ofGetWidth()/2,40,20);
	}
	
	if(panel.getValueB("reloadAnimation") || updateButton==true){
		input.loadAnimation();
		panel.setValueB("reloadAnimation", false);
		ofSetColor(0,255,0,200);
		ofCircle(ofGetWidth()/2,40,20);
	}
	*/

	

	
	   
}

//--------------------------------------------------------------
void testApp::exit() {
}

void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
	// samples are "interleaved"

	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];

		
	}
	bufferCounter++;
	
}
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){	
	// samples are "interleaved"
	
	for (int i = 0; i < bufferSize; i++){
		output[i*2] = left[i];
		output[i*2+1]=right[i];
	
		
	}

	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	if(key == 'f') {
		ofToggleFullscreen();
	}
	if(key == 'a') {
		input.loadAnimation();
	}
	if(key == 'c') {
		panel.clearAllChanged();
	}
	
	switch (key){
		case ' ':
			input.bLearnBakground = true;
			input.trackCloud.clear();
			break;
	}

}



//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

