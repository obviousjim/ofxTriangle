/*!
* ofxTriangle by kikko.fr
* -> C++ Triangle warper by Piyush Kumar
* -> Point inside polygon by Theo
*/

#ifndef OFXTRIANGLE_H_INCLUDED
#define OFXTRIANGLE_H_INCLUDED

//JG added this #define to allow to be decoupled from OpenCV
//along with the ability to pass in any vector of points
//allows for use with contours from OpenTSPS
//#define USE_OPENCV

#include "ofMain.h"
#ifdef USE_OPENCV
#include "ofxOpenCv.h"
#endif


typedef struct{
    ofPoint pts[3];
    int index[3];
    
    ofColor randomColor;  // useful for debugging / drawing 
    
} ofxTriangleData;

class ofxTriangle {
    public :

	ofxTriangle(){
        nTriangles = 0;
        //delobject = NULL;
    }
    
	~ofxTriangle(){}
#ifdef USE_OPENCV
	// Triangulate a openCV blob
	void triangulate(ofxCvBlob &cvblob, int resolution = 50);
#endif
	void triangulate(vector<ofPoint> contour, int resolution = 50);

	ofPoint getTriangleCenter(ofPoint *tr);
	bool isPointInsidePolygon(ofPoint *polygon,int N, ofPoint p);

	void draw();
	void draw(float r, float g, float b);

	void draw(float x, float y);
	void clear();

    int nTriangles;
	
    vector <ofPoint> outputPts;
    vector <ofxTriangleData> triangles;
    ofMesh triangulatedMesh;
    
    
    

};

#endif // OFXTRIANGLE_H_INCLUDED
