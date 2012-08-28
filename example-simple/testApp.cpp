#include "testApp.h"

void testApp::setup(){
	
	ofBackground(50, 50, 50);
    ofSetFrameRate(60);
    ofSetWindowShape(600, 500);
	
    buffer.allocate(640, 480, GL_RGBA, 1);
    
    loadVertices();
}


void testApp::update(){

    //Draw anything you like inside the buffer
    buffer.begin();
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    ofSetHexColor(0xFF0000);
    ofFill();
    for (int i = 0; i < 480; i += 30){
        ofRect(0, ofGetFrameNum()%30+i, 640, 10);
    }
    buffer.end();
    
    //Triangulate the points
    triangle.clear();
    triangle.triangulate(line.getVertices());
}


void testApp::draw(){
	
    //Render the buffer insode the shape by drawing as individual triangles
    glPushMatrix();
	buffer.getTextureReference().bind();
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangle.nTriangles;i++){
		glTexCoord2f(triangle.triangles[i].a.x, triangle.triangles[i].a.y);
		glVertex2f(triangle.triangles[i].a.x, triangle.triangles[i].a.y);
        
		glTexCoord2f(triangle.triangles[i].b.x, triangle.triangles[i].b.y);
		glVertex2f(triangle.triangles[i].b.x, triangle.triangles[i].b.y);
        
		glTexCoord2f(triangle.triangles[i].c.x, triangle.triangles[i].c.y);
		glVertex2f(triangle.triangles[i].c.x, triangle.triangles[i].c.y);
	}
	glEnd();
	buffer.getTextureReference().unbind();
	glPopMatrix();
    
    // Infos
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("FPS : " + ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapString("nb Triangles : " + ofToString(triangle.nTriangles), 10, 40);
}

void testApp::keyPressed(int key){
	
}

void testApp::loadVertices(){
    line.addVertex(ofPoint(100, 100));
    line.addVertex(ofPoint(200, 100));
    line.addVertex(ofPoint(300, 200));
    line.addVertex(ofPoint(400, 100));
    line.addVertex(ofPoint(500, 100));
    line.addVertex(ofPoint(500, 400));
    line.addVertex(ofPoint(100, 400));
}
