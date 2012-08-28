#include "testApp.h"

void testApp::setup(){
	
	ofBackground(50, 50, 50);
    ofSetFrameRate(60);
    ofSetWindowShape(600, 500);
	
    buffer.allocate(640, 480, GL_RGBA);
    
    loadVertices();
}


void testApp::update(){

    fillBuffer();
    
    //Triangulate the points
    triangle.clear();
    triangle.triangulate(line.getVertices());
}


void testApp::draw(){
    
    drawSolidFillShape();
    drawBufferFillShape();
    drawInfo();
}

void testApp::keyPressed(int key){
	
}

void testApp::loadVertices(){
    //A rectangle with a chunk taken out, making a concave shape
    line.addVertex(ofPoint(0, 0));
    line.addVertex(ofPoint(200, 0));
    line.addVertex(ofPoint(200, 300));
    line.addVertex(ofPoint(0, 300));
    line.addVertex(ofPoint(0, 220));
    line.addVertex(ofPoint(75, 150));
    line.addVertex(ofPoint(0, 80));
}

void testApp::fillBuffer(){
    buffer.begin();
    ofClear(0, 0, 0);
    
    //Draw anything you like inside the buffer
    ofSetHexColor(0xFF0000);
    ofFill();
    for (int i = 0; i < 480; i += 30){
        ofRect(0, ofGetFrameNum()%30+i, 640, 10);
    }
    buffer.end();
}

void testApp::drawSolidFillShape(){
    ofPushMatrix();
    ofTranslate(70, 125);
    ofBeginShape();
    ofSetColor(255, 192, 64);
    vector<ofPoint> vertices = line.getVertices();
    for(int i = 0; i < vertices.size(); i++){
        ofVertex(vertices[i]);
    }
    ofEndShape();
    ofPopMatrix();
}

void testApp::drawBufferFillShape(){
    
    //Render the buffer inside the shape by drawing as individual triangles
    ofPushMatrix();
    ofTranslate(340, 125);
    buffer.getTextureReference().bind();
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < triangle.nTriangles; i++){
        glTexCoord2f(triangle.triangles[i].a.x, triangle.triangles[i].a.y);
        glVertex2f(triangle.triangles[i].a.x, triangle.triangles[i].a.y);
        
        glTexCoord2f(triangle.triangles[i].b.x, triangle.triangles[i].b.y);
        glVertex2f(triangle.triangles[i].b.x, triangle.triangles[i].b.y);
        
        glTexCoord2f(triangle.triangles[i].c.x, triangle.triangles[i].c.y);
        glVertex2f(triangle.triangles[i].c.x, triangle.triangles[i].c.y);
    }
    glEnd();
    buffer.getTextureReference().unbind();
    ofPopMatrix();   
}

void testApp::drawInfo(){
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("FPS : " + ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapString("nb Triangles : " + ofToString(triangle.nTriangles), 10, 40);
    ofDrawBitmapString("Buffer filled shape:", 340, 109);
    ofDrawBitmapString("Solid filled shape:", 70, 109);
}
