//
//  frameStructure.cpp
//  kuSimulation_prty
//
//  Created by LIFE_MAC_46 on 2020/08/31.
//

#include "frameStructure.h"

//--------------------------------------------------------------
void FrameStr::setup(ofVec2f _position, float angle){
     m.glScale(0.1, 0.1, 0.1);
     m.glRotate(-90 - angle, 0, 1, 0);
     m.translate(_position.x, 0, _position.y);
     createOctaByVboMesh(&m);
     
//     glPointSize(4);

}

//--------------------------------------------------------------
void FrameStr::update(ofPixels inputPix){
     ofColor darkColor;
     darkColor.set(0, 0, 0);
     
     for(int i=0; i<vboMesh.getNumVertices(); i++){
          int ii;
          int j;
          if(i < 300){
               ii = i;
               j=0;
          }
          else if(i >=300 && i< 600){
               ii = i-300;
               j=1;
          }
          else{
                ii = ofMap(i -600, 0, 240, 0, 300);
//               ii = i -600;
               j=2;
          }
          vboMesh.setColor(i, darkColor);
          vboMesh3.setColor(i, darkColor);
          
//          vboMesh.setColor(i, inputPix.getColor(ii, j));
//          vboMesh3.setColor(i, inputPix.getColor(ii, j+3));
//          float order1;
//          float order2;
//          order1 = vboMesh.getVertex(i)[_scanType];
//          order2 = vboMesh3.getVertex(i)[_scanType];
//          if(order1 < scanPos && order1 >=0){
//               vboMesh.setColor(i, setColor);
//          }
//          if(order2 < scanPos && order2 >=0){
//               vboMesh3.setColor(i, setColor);
//          }
//          pix.setColor(ii, j, vboMesh.getColor(i));
//          pix.setColor(ii, (j+3), vboMesh3.getColor(i));
     }

}

//--------------------------------------------------------------
void FrameStr::draw(){
     ofSetColor(0);
     vboMesh2.draw(OF_MESH_WIREFRAME);
     vboMesh4.draw(OF_MESH_WIREFRAME);
     ofSetColor(255);
     vboMesh.draw(OF_MESH_POINTS);
     vboMesh3.draw(OF_MESH_POINTS);

}

void FrameStr::createOctaByVboMesh(ofMatrix4x4 * mat){
     vector<glm::vec3> vecs;
     vector<glm::vec3> vecs2;
     vector<ofColor_<float>> col11;
     vector<glm::vec3> vecs3;
     vector<glm::vec3> vecs4;
     vector<ofColor_<float>> col22;
      
     for (int i = 0; i < ov.size(); i++) {
          vecs2.push_back(ov[i] * *mat);
          vecs4.push_back(ov2[i] * *mat);
     }
      //01
      for(int i=0; i<300; i++){
           ofVec3f vv;
           vv = ov[1]-ov[0];
           vecs.push_back((vv/300*i + ov[0])* *mat);
           col11.push_back(ofFloatColor(255, 0 ,0));
      }
      for(int i=0; i<300; i++){
           ofVec3f vv;
           vv = ov[2]-ov[0];
           vecs.push_back((vv/300*i + ov[0]) * *mat);
           col11.push_back(ofFloatColor(255, 0 ,0));
      }
      for(int i=0; i<240; i++){
           ofVec3f vv;
           vv = ov[2]-ov[1];
           vecs.push_back((vv/240*i + ov[1]) * *mat);
           col11.push_back(ofFloatColor(255, 0 ,0));
      }
      //02
      for(int i=0; i<300; i++){
           ofVec3f vv;
           vv = ov2[1]-ov2[0];
           vecs3.push_back((vv/300*i + ov2[0]) * *mat);
           col22.push_back(ofFloatColor(0, 255 ,0));
      }
      for(int i=0; i<300; i++){
           ofVec3f vv;
           vv = ov2[2]-ov2[0];
           vecs3.push_back((vv/300*i + ov2[0]) * *mat);
           col22.push_back(ofFloatColor(0, 255 ,0));
      }
      for(int i=0; i<240; i++){
           ofVec3f vv;
           vv = ov2[2]-ov2[1];
           vecs3.push_back((vv/240*i + ov2[1]) * *mat);
           col22.push_back(ofFloatColor(0, 255 ,0));
      }
     
      vboMesh.addVertices(vecs);
      vboMesh.addColors(col11);
      vboMesh2.addVertices(vecs2);
      vboMesh2.addIndices(oi);
     
      vboMesh3.addVertices(vecs3);
      vboMesh3.addColors(col22);
      vboMesh4.addVertices(vecs4);
      vboMesh4.addIndices(oi);
     
}
