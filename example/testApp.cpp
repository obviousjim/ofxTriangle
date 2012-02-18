#include "testApp.h"

void testApp::setup(){
	
	ofBackground(255, 255, 255);
    ofSetFrameRate(60);
	
    colorImage.allocate(CAM_WIDTH, CAM_HEIGHT);
    threImg.allocate(CAM_WIDTH, CAM_HEIGHT);
    bgImg.allocate(CAM_WIDTH, CAM_HEIGHT);
	
	vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(CAM_WIDTH, CAM_HEIGHT);
	
	threshold = 20;
	bLearnBackground = true;
}


void testApp::update(){
	
	vidGrabber.grabFrame();
	
	if (vidGrabber.isFrameNew())
	{
        colorImage.setFromPixels(vidGrabber.getPixels(), CAM_WIDTH, CAM_HEIGHT);
        colorImage.mirror(false, true);
		
        if(bLearnBackground)
        {
            bgImg = colorImage;
            bLearnBackground = false;
        }
		
        threImg = colorImage;
        threImg.absDiff(bgImg);
        threImg.blur(5);
        threImg.threshold(threshold);
		
        contourFinder.findContours(threImg, 20, (CAM_WIDTH*CAM_HEIGHT)/3, 5, false);
		
        triangle.clear();
		
        for (int i=0; i<contourFinder.nBlobs; i++)
        {
            triangle.triangulate(contourFinder.blobs[i], max( 3.0f, (float)contourFinder.blobs[i].pts.size()/5));
        }
		
	}
}


void testApp::draw(){
	
    ofSetColor(255, 255, 255);
	
	// Camera view
    colorImage.draw(CAM_WIDTH, 0);
	
    // Thresholded image + contour finder
    threImg.draw(0, 0);
    contourFinder.draw(0, 0);
	
    // Triangulation
    triangle.draw(CAM_WIDTH, 0);
	
    // Infos
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("FPS : " + ofToString(ofGetFrameRate()), 10, CAM_HEIGHT + 20);
    ofDrawBitmapString("nb Triangles : " + ofToString(triangle.nTriangles), 10, CAM_HEIGHT + 40);
}

void testApp::keyPressed(int key){
	
	switch (key) {
		case ' ':
			bLearnBackground = true;
			break;
		case 's':
			vidGrabber.videoSettings();
			break;
	}
}
