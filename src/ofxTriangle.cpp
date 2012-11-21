#include "ofxTriangle.h"
#include "triangle.h"








#ifdef USE_OPENCV
void ofxTriangle::triangulate(ofxCvBlob &cvblob, int resolution){		
	triangulate(cvblob.pts, resolution);
}
#endif


void triangulatePoints(char * flags, triangulateio * in, triangulateio * mid, triangulateio * out){
    triangulate(flags, in,  mid, out);
}


void ofxTriangle::triangulate(vector<ofPoint> contour, int resolution){

    int bSize = contour.size();
    float maxi = min(resolution, bSize);
	
    
    struct triangulateio in, mid, out, vorout;
    in.numberofpoints = maxi;
    in.numberofpointattributes = 0;
    in.pointmarkerlist = NULL;
    in.pointlist = (REAL *) malloc(maxi * 2 * sizeof(REAL));
    in.numberofregions = 0;
    in.regionlist =  NULL;
    
    
    for(int i = 0; i < maxi; i++) {
        //int id = (int) ( (float)i/maxi*bSize );
		int indx = ofMap(i, 0, maxi, 0, bSize);
        in.pointlist[i*2+0] = contour[indx].x;
        in.pointlist[i*2+1] = contour[indx].y;
    }
	
    //z = start from zero
    //Q = quiet
    //N = no nodes
    
    mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
    /* Not needed if -N switch used or number of point attributes is zero: */
    mid.pointattributelist = (REAL *) NULL;
    mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
    mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
    /* Not needed if -E switch used or number of triangle attributes is zero: */
    mid.triangleattributelist = (REAL *) NULL;
    mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
    /* Needed only if segments are output (-p or -c) and -P not used: */
    mid.segmentlist = (int *) NULL;
    /* Needed only if segments are output (-p or -c) and -P and -B not used: */
    mid.segmentmarkerlist = (int *) NULL;
    mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
    mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */
 
    
    // V = verbose -- helpful!
    // q = angle constraint
    // z = start from zero
    
    triangulatePoints("zQq32", &in, &mid, NULL);
    
    //printf("Initial triangulation:\n\n");
    //report(&mid, 1, 1, 1, 1, 1, 0);
    //    for (int i = 0; i < mid.numberofpoints; i++) {
    //        printf("Point %4d:", i);
    //        for (int j = 0; j < 2; j++) {
    //            printf("  %.6g", mid.pointlist[i * 2 + j]);
    //        }
    //        printf("\n");
    //
    //    }
    
    
    
    nTriangles = 0;
    
    for (int i = 0; i < mid.numberoftriangles; i++) {
        ofxTriangleData triangle;
        
        int whichPt;
        
        whichPt = mid.trianglelist[i * mid.numberofcorners + 0];
        triangle.a = ofPoint(  mid.pointlist[ whichPt * 2 + 0],  mid.pointlist[ whichPt * 2 + 1]);
        whichPt = mid.trianglelist[i * mid.numberofcorners + 1];
        triangle.b = ofPoint(  mid.pointlist[ whichPt * 2 + 0],  mid.pointlist[ whichPt * 2 + 1]);
        whichPt = mid.trianglelist[i * mid.numberofcorners + 2];
        triangle.c = ofPoint(  mid.pointlist[ whichPt * 2 + 0],  mid.pointlist[ whichPt * 2 + 1]);
        
        ofPoint tr[3];
        tr[0] = triangle.a;
        tr[1] = triangle.b;
        tr[2] = triangle.c;
		
        if( isPointInsidePolygon(&contour[0], contour.size(), getTriangleCenter(tr) ) ) {
            triangles.push_back(triangle);
            nTriangles++;
        }

    }
    
    free(in.pointlist);
    free(mid.pointlist);
    free(mid.pointattributelist);
    free(mid.pointmarkerlist);
    free(mid.trianglelist);
    free(mid.triangleattributelist);
    
    return;

}

void ofxTriangle::clear(){
    triangles.clear();
    nTriangles = 0;
}

ofPoint ofxTriangle::getTriangleCenter(ofPoint *tr){
    float c_x = (tr[0].x + tr[1].x + tr[2].x) / 3;
    float c_y = (tr[0].y + tr[1].y + tr[2].y) / 3;
    return ofPoint(c_x, c_y);
}

bool ofxTriangle::isPointInsidePolygon(ofPoint *polygon,int N, ofPoint p)
{
    int counter = 0;
    int i;
    double xinters;
    ofPoint p1,p2;

    p1 = polygon[0];

    for (i=1;i<=N;i++)
    {
        p2 = polygon[i % N];
        if (p.y > MIN(p1.y,p2.y)) {
            if (p.y <= MAX(p1.y,p2.y)) {
                if (p.x <= MAX(p1.x,p2.x)) {
                    if (p1.y != p2.y) {
                        xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
                        if (p1.x == p2.x || p.x <= xinters){
                            counter++;
						}
                    }
                }
            }
        }
        p1 = p2;
    }
	return counter % 2 != 0;
}

void ofxTriangle::draw(float x, float y) {
    ofPushMatrix();
    ofTranslate(x, y, 0);
	draw();
    ofPopMatrix();
}

void ofxTriangle::draw() {
	draw(ofRandom(0, 255),ofRandom(0, 255),ofRandom(0, 255));
}

void ofxTriangle::draw(float r, float g, float b) {
    ofFill();
	
    for (int i=0; i<nTriangles; i++){
        ofSetColor( ofRandom(0,255));
        ofTriangle( triangles[i].a.x, triangles[i].a.y,
				   triangles[i].b.x, triangles[i].b.y,
				   triangles[i].c.x, triangles[i].c.y);
    }
}
