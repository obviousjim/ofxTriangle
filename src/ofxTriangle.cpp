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
    
    printf("Initial triangulation:\n\n");
    //report(&mid, 1, 1, 1, 1, 1, 0);
        for (int i = 0; i < mid.numberofpoints; i++) {
            printf("Point %4d:", i);
            for (int j = 0; j < 2; j++) {
                printf("  %.6g", mid.pointlist[i * 2 + j]);
            }
            printf("\n");

        }
    
    
    
    nTriangles = 0;
    
    
    std::map < int , ofPoint  > goodPts;
    
    for (int i = 0; i < mid.numberoftriangles; i++) {
        ofxTriangleData triangle;
        
        int whichPt;
        
        for (int j = 0; j < 3; j++){
            whichPt = mid.trianglelist[i * 3 + j];
            triangle.pts[j] = ofPoint(  mid.pointlist[ whichPt * 2 + 0],  mid.pointlist[ whichPt * 2 + 1]);
            triangle.index[j] = whichPt;
            
            
            
        }
        
        ofPoint tr[3];
        tr[0] = triangle.pts[0];
        tr[1] = triangle.pts[1];
        tr[2] = triangle.pts[2];
		
        if( isPointInsidePolygon(&contour[0], contour.size(), getTriangleCenter(tr) ) ) {
            
            triangle.randomColor = ofColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
        
            triangles.push_back(triangle);
            
            // store the good points in a map
            for (int j = 0; j < 3; j++){
                goodPts[triangle.index[j]] = triangle.pts[j]; 
            }
            
            nTriangles++;
        }
    }
    
    
    // put all good points in a vector and handle the remapping of indices.
    // the indices stored above were for all points, but since we drop triangles, we 
    // can drop non used points, and then remap all the indces. 
    
    outputPts.clear();
    std::map < int, int > indexChanges;
    std::map< int , ofPoint >::iterator iter;
    for (iter = goodPts.begin(); iter != goodPts.end(); ++iter) {
        cout << iter->first << " " << iter->second << endl;
        indexChanges[iter->first] = outputPts.size();
        outputPts.push_back(iter->second);
    }
    
    for (int i = 0; i < triangles.size(); i++){
        for (int j = 0; j < 3; j++){
            triangles[i].index[j] = indexChanges[triangles[i].index[j]];
        }
    }
    
    
    triangulatedMesh.clear();
    triangulatedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    for (int i = 0; i < outputPts.size(); i++){
        triangulatedMesh.addVertex(outputPts[i]);
    }
    
    for (int i = 0; i < triangles.size(); i++){
        triangulatedMesh.addIndex(triangles[i].index[0]);;
        triangulatedMesh.addIndex(triangles[i].index[1]);;
        triangulatedMesh.addIndex(triangles[i].index[2]);;
    }
    
    

    // depending on flags, we may need to adjust some of the memory clearing
    // (see tricall.c for full listings)
    
    free(in.pointlist);
    free(mid.pointlist);
    if (mid.pointattributelist != NULL) free(mid.pointattributelist);
    if (mid.pointmarkerlist != NULL) free(mid.pointmarkerlist);
    free(mid.trianglelist);
    if (mid.triangleattributelist != NULL) free(mid.triangleattributelist);
    
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
        ofSetColor( triangles[i].randomColor);

        ofTriangle( outputPts[triangles[i].index[0]], 
        outputPts[triangles[i].index[1]],
                   outputPts[triangles[i].index[2]]);
        
        
        ofSetColor(255,255,255);
        triangulatedMesh.drawWireframe();
        
        
    }
}
