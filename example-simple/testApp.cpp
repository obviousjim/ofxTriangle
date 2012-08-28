#include "testApp.h"

void testApp::setup(){
	
	ofBackground(50, 50, 50);
    ofSetFrameRate(60);
    ofSetWindowShape(600, 500);
	
    buffer.allocate(640, 480, GL_RGBA, 1);
    
    createPointsVector();
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
    triangle.triangulate(points);
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

void testApp::createPointsVector(){
    
    //Make a simple concave polygon
    points.clear();
    
    ofPoint point1;
    point1.x = 100;
    point1.y = 100;
    point1.z = 0;
    
    ofPoint point2;
    point2.x = 200;
    point2.y = 100;
    point2.z = 0;
    
    ofPoint point3;
    point3.x = 300;
    point3.y = 200;
    point3.z = 0;
    
    ofPoint point4;
    point4.x = 400;
    point4.y = 100;
    point4.z = 0;
    
    ofPoint point5;
    point5.x = 500;
    point5.y = 100;
    point5.z = 0;
    
    ofPoint point6;
    point6.x = 500;
    point6.y = 400;
    point6.z = 0;
    
    ofPoint point7;
    point7.x = 100;
    point7.y = 400;
    point7.z = 0;
    
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
    points.push_back(point4);
    points.push_back(point5);
    points.push_back(point6);
    points.push_back(point7);
}
