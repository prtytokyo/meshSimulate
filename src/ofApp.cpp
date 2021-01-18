#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
     ofBackground(0);
     ofSetFrameRate(60);
     glPointSize(2.0f);
     ofDisableArbTex();
     
     for(int k=0; k<F_NUMBER; k++){
          ofVec2f posXZ;
          radius = 1200;
          fizPrmX = 501;
          float angle = 360/F_NUMBER*k;
          float rad = angle*(3.14/180);
          posXZ[0] = radius*cos(rad);
          posXZ[1] = radius*sin(rad);
          frameStr[k].setup(posXZ, angle);
     }
     
     scanBox.set(1000, 10, 1000);
     scanType = 0;
     moveRenge =3200;
     
     outputFbo.allocate(300, 6*F_NUMBER, GL_RGB);
     pix.allocate(300, 6*F_NUMBER, 3);
     for(int i=0; i<pix.getWidth(); i++){
          for(int j=0; j<pix.getHeight(); j++){
               pix.setColor(i, j, ofColor(0, 0, 0));
          }
     }
     
     //Gui
     gui.setPosition(0, 0);
     gui.setup("panel");
     gui.add(isStart.set("Play", true));
     gui.add(speed.set( "Speeds", 1.0, 0.0, 10));
     gui.add(isVisibleScaning.set("VisibleGuide", false));
     gui.add(isVisibleGrid.set("VisibleGrid", false));
     gui.add(AbletonOn.set(" UseAbletonTrack", false));
     gui.add(clipNumber.set(" Trrack NUmber", "0"));
     gui.add(modeScan.setup("MODE1_Scan"));
     gui.add(flipCol.set("    FlipCol", true));
     gui.add(color.set("color",ofColor(220,220,220),ofColor(0,0),ofColor(255,255)));
     gui.add (scanXaxis.setup("    ScanXaxis"));
     gui.add(scanYaxis.setup("     ScanYaxis"));
     gui.add(scanZaxis.setup("     ScanZaxis"));
     gui.add(modeVideoMap.setup("MODE2_VideoMap"));
     gui.add(sideviewMap.setup("   SideviewMap"));
     gui.add(topviewMap.setup("    TopviewMap"));
     
     //SyphonOut
     server.setName("OUTPUT");
     
     cam.setPosition(400, 1200, 3000);
     cam.lookAt(ofVec3f(0, 0, 0));
     stopper = 0;
     
     live.setup();


}

//--------------------------------------------------------------
void ofApp::update(){
     ofSetWindowTitle(ofToString(ofGetFrameRate()));
     live.update();
     
     if(isStart){
          int a = (ofGetFrameNum()*speed*4);
           scanPos = float(a%moveRenge) - moveRenge/2 + startPos;
          video.setPaused(false);
     }else{
          video.setPaused(true);
     }
     
     if(AbletonOn){
          video.setVolume(0.f);
     }else video.setVolume(1.f);
     
     if(modeScan) mode = 0;
     if(modeVideoMap) mode = 1;
     if(sideviewMap) mapType = 0;
     if(topviewMap) mapType = 1;

     
     if(mode == 0){
          if(scanXaxis){
               scanType = 0;
               moveRenge =3200;
               startPos = 0;
          }
          if(scanYaxis){
               scanType = 1;
               moveRenge = 450;
               startPos = moveRenge/2;
          }
          if(scanZaxis){
               scanType = 2;
               moveRenge =3200;
               startPos = 0;
          }
          
          switch (scanType) {
               case 0:
                    scanBox.set(10, 3200, 3200);
                    scanBox.setPosition(scanPos, 0, 0);
                    break;
               case 1:
                   scanBox.set(3200, 10, 3200);
                    scanBox.setPosition(0, scanPos, 0);
                    break;
               case 2:
                   scanBox.set(3200, 3200, 10);
                    scanBox.setPosition(0, 0, scanPos);
                    break;
               default:
                    break;
          }
          
          if(flipCol){
               setColor = color;
               darkColor = ofFloatColor(0, 0, 0);
          }else{
               setColor = ofFloatColor(0, 0, 0);
               darkColor = color;
          }
          
          for(int i=0; i<pix.getWidth(); i++){
               for(int j=0; j<pix.getHeight(); j++){
                    pix.setColor(i, j, ofColor(0, 0, 0));
               }
          }
          
          scan(scanType);
     }
     
     
     if(video.isPlaying()){
          video.update();
          video.setSpeed(speed);
          
          if(video.getCurrentFrame() == 0 && stopper == 0){
                    cout << "VideoDnoe" << endl;
                    stopper = 1;
               if(AbletonOn){
                    ofxAbletonLiveTrack *track = live.getTrack(1);
                    if(track->getClips().size()>0){
                         live.selectScene(ofToInt(clipNumber));
//                         ofxAbletonLiveClip *clipByIndex = track->getClip(ofToInt(clipNumber));
//                         clipByIndex->play();
                    }
               }
          }else stopper = 0;
     }
     
     if(mode == 1){
          if(video.isLoaded()){
               vidPix.clear();
               if(mapType == 0) vidFbo = vidFbo_Side;
               else if(mapType == 1) vidFbo = vidFbo_Top;
               vidFbo.begin();
               ofClear(0);
               video.draw(0, 0, vidFbo.getWidth(), vidFbo.getHeight());
               vidFbo.end();
               vidFbo.readToPixels(vidPix);
               
               ofColor setCol1;
               ofColor setCol2;
               
               for(int k=0; k<F_NUMBER; k++){
                    for(int i=0; i<frameStr[k].vboMesh.getNumVertices(); i++){
                         
                         frameStr[k].vboMesh.setColor(i, darkColor);
                         frameStr[k].vboMesh3.setColor(i, darkColor);
                         if(mapType == 0){
                               setCol1 = vidPix.getColor(frameStr[k].vboMesh.getVertices()[i].x + vidPix.getWidth()/2,  (vidPix.getHeight()-1) -  frameStr[k].vboMesh.getVertices()[i].y);
                              setCol2 = vidPix.getColor(frameStr[k].vboMesh3.getVertices()[i].x + vidPix.getWidth()/2,  (vidPix.getHeight()-1) -  frameStr[k].vboMesh3.getVertices()[i].y);
                         }else if(mapType == 1){
                              setCol1 = vidPix.getColor(frameStr[k].vboMesh.getVertices()[i][0] + vidPix.getWidth()/2,  frameStr[k].vboMesh.getVertices()[i][2] + vidPix.getHeight()/2);
                              setCol2 = vidPix.getColor(frameStr[k].vboMesh3.getVertices()[i][0] + vidPix.getWidth()/2,  frameStr[k].vboMesh3.getVertices()[i][2] + vidPix.getHeight()/2);
                         }
                         
                         frameStr[k].vboMesh.setColor(i, setCol1);
                         frameStr[k].vboMesh3.setColor(i, setCol2);
                         
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
                              ii = i -600;
                              j=2;
                         }
                         pix.setColor(ii, j + k*6, frameStr[k].vboMesh.getColor(i));
                         pix.setColor(ii, (j+3) + k*6, frameStr[k].vboMesh3.getColor(i));
                    }
               }
          }
     }
     
     for(int k=0; k<F_NUMBER; k++){
          for(int i=0; i<pix.getWidth(); i++){
               float ii;
               ii = ofMap(i, 0, 300, 0, 240);
               pix.setColor(i, 2 + k*6, frameStr[k].vboMesh.getColor(600+ii));
               pix.setColor(i, 5  + k*6, frameStr[k].vboMesh3.getColor(600+ii));
          }
     }
     
//     rePix.allocate(300, 300, 3);
//         for(float i=0; i<300; i++){
//              for(int j=0; j<300; j++){
//                   if(j<50) rePix.setColor(i, j, pix.getColor(i, 0));
//                   else if(j>=50 && j<100) rePix.setColor(i, j, pix.getColor(i, 1));
//                   else if(j>=100 && j<150){
//                        rePix.setColor(299-i, j, pix.getColor(i, 2));
//                   }
//                   else if(j>=150 && j<200) rePix.setColor(i, j, pix.getColor(i, 3));
//                   else if(j>=200 && j<250) rePix.setColor(i, j, pix.getColor(i, 4));
//                   else if(j>=250 && j<300){
//                        rePix.setColor(299-i, j, pix.getColor(i, 5));
//                   }
//              }
//         }

}

//--------------------------------------------------------------
void ofApp::draw(){
         
     cam.begin();
     if(isVisibleScaning){
           ofEnableDepthTest();
          float rad = 0*(3.14/180);
          ofSetColor(200, 50, 10);
          ofDrawSphere(radius*cos(rad), 500,  radius*sin(rad), 10);
     }
     
     if(isVisibleGrid) ofDrawGrid(200, 200, 10, 10);
     
     ofSetColor(20, 200);
     ofDrawBox(0, -14, 0, 8000, 2, 8000);
     
     //mesh
     for(int i=0; i<F_NUMBER; i++){
          frameStr[i].draw();
     }
     
     if(mode == 0){
          if(isVisibleScaning){
               ofSetColor(255, 180);
               scanBox.draw();
          }
     }
     if(mode == 1){
          if(video.isLoaded()){
               if(isVisibleScaning){
                    vidFbo.getTexture().bind();
                    if(mapType == 0)
                         ofDrawBox(0, 200, 0, vidFbo.getWidth(), vidFbo.getHeight(), 10);
                    if(mapType == 1)
                         ofDrawBox(0, 0, 0, vidFbo.getWidth(), 10, vidFbo.getHeight());
                    vidFbo.getTexture().unbind();
               }
          }
          
     }
     
     ofDisableDepthTest();
     cam.end();
     
     ofSetColor(255);
     ofPushMatrix();
     ofPushStyle();
     ofTexture tex;
     tex.allocate(pix);
     outputFbo.begin();
     ofClear(0);
     tex.draw(0, 0);
     outputFbo.end();
     ofSetColor(255);
     outputFbo.draw(0, ofGetHeight()-outputFbo.getHeight());
     ofPopStyle();
     ofPopMatrix();
     
     if(mode == 1){
          ofSetColor(0);
          ofDrawRectangle(0, ofGetHeight()*0.6, outputFbo.getWidth(), outputFbo.getHeight());
          ofSetColor(255);
          video.draw(0, ofGetHeight()*0.6, outputFbo.getWidth(), outputFbo.getHeight());
     }
     
     
     gui.draw();
     //message
     string mes;
     mes = "If you want to use MODE@_VideoMapping, drag mov of mp4 file in this window.";
     ofDrawBitmapStringHighlight(mes, ofGetWidth()*0.22, ofGetHeight()*0.03);
     
     server.publishTexture(&outputFbo.getTexture());

}

void ofApp::scan(int _scanType){
     for(int k=0; k<F_NUMBER; k++){
          for(int i=0; i<frameStr[k].vboMesh.getNumVertices(); i++){
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
                    ii = i -600;
                    j=2;
               }
               frameStr[k].vboMesh.setColor(i, darkColor);
               frameStr[k].vboMesh3.setColor(i, darkColor);
               float order1;
               float order2;
               order1 = frameStr[k].vboMesh.getVertex(i)[_scanType];
               order2 = frameStr[k].vboMesh3.getVertex(i)[_scanType];
               if(order1 < scanPos){
                    frameStr[k].vboMesh.setColor(i, setColor);
               }
               if(order2 < scanPos){
                    frameStr[k].vboMesh3.setColor(i, setColor);
               }
               pix.setColor(ii, j + k*6, frameStr[k].vboMesh.getColor(i));
               pix.setColor(ii, (j+3) + k*6, frameStr[k].vboMesh3.getColor(i));
          }
     }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
     if(key == ' '){
          
     }
     switch (key) {
          case ' ':
               break;
               
          default:
               break;
     }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
     stringstream path;
     for(int i = 0;i < dragInfo.files.size(); i++){
          path << dragInfo.files[i];
     }
     ofLog() << path.str();
     video.load(path.str());
     video.setLoopState(OF_LOOP_NORMAL);
     video.play();
     
     vidFbo_Side.allocate(radius*2 + fizPrmX, 369, GL_RGB);
     vidFbo_Top.allocate(radius*2 + fizPrmX, radius*2 + fizPrmX, GL_RGB);
     
     mode = 1;

}
