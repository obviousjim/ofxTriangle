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

#include <del_interface.hpp>

using namespace tpp;
//using namespace std;

typedef struct
{
    ofPoint a;
    ofPoint b;
    ofPoint c;

    float area;

} ofxTriangleData;

class ofxTriangle
{
    public :

        ofxTriangle(){}
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


        Delaunay* delobject;

        int nTriangles;
        vector <ofxTriangleData> triangles;


};

#endif // OFXTRIANGLE_H_INCLUDED
