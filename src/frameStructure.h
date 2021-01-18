//
//  frameStructure.h
//  kuSimulation_prty
//
//  Created by LIFE_MAC_46 on 2020/08/31.
//

//#ifndef frameStructure_h
//#define frameStructure_h
#pragma once
#include "ofMain.h"

class FrameStr : public ofBaseApp{

public:
        void setup(ofVec2f _position, float angle);
        void update(ofPixels inputPix);
        void draw();
     
     //Mesh
     ofMatrix4x4 m;
     ofMesh mesh;
     ofVbo vbo;
     ofVboMesh vboMesh;
     ofVboMesh vboMesh2;
     ofVboMesh vboMesh3;
     ofVboMesh vboMesh4;
     vector<ofVec3f> ov = {
         ofVec3f(-750,0,-2500), ofVec3f(-750,0,2500), ofVec3f(750,3700,900)
     };
     vector<ofVec3f> ov2 = {
         ofVec3f(750,0,-2500), ofVec3f(750,0,2500), ofVec3f(-750,3700,900)
     };
     vector<unsigned> oi = {
         0, 1, 2
     };
     void createOctaByVboMesh(ofMatrix4x4 * mat);
     
};

//#endif /* frameStructure_h */
