#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(5);
    
    // load all the images
    ofLog() << "Gathering images...";
    ofDirectory dir;
    int nFiles = dir.listDir(ofToDataPath("imageSequences/"));
    if(nFiles) {
        for(int i=0; i<dir.size(); i++) {
            if (i % 20 == 0)    ofLog() << " - loading image "<<i<<" / "<<dir.size(); // stops at 32760 images that are 320x240px !
            string filePath = dir.getPath(i);
            images.push_back(ofImage());
            images.back().load(filePath);
            filenames.push_back(dir.getFile(i).getFileName());
        }
    }
    
    // setup ofxCcv
    ccv.setup("image-net-2012.sqlite3");
    
    // encode all of the images with ofxCcv
    ofLog() << "Encoding images...";
    for (int i=0; i<images.size(); i++) {
        if (i % 20 == 0)    ofLog() << " - encoding image "<<i<<" / "<<images.size(); // died at 18240 images 320x240px with 16GB RAM
        vector<float> encoding = ccv.encode(images[i], ccv.numLayers()-6); //adjust ccv.numLayers()-x between 1-12 for more convoluted image processing (higher number is more convoluted)
        encodings.push_back(encoding);
    }
    
    // run t-SNE and load image points to imagePoints
    ofLog() << "Run t-SNE on images";
    imagePoints = tsne.run(encodings, 2, 50, 0.1, true); // (vector of images, dimensions (2, 3, ?), perplexity (corresponds to the "number of neighbors" 10-100 is good), theta (the lower the more "accurate", but takes longer), runManually boolean)
    
    // make the images the same size
    for (int i=0; i<images.size(); i++) {
        images[i].resize(100 * images[i].getWidth() / images[i].getHeight(), 100);
    }
    
    // setup gui
    gui.setup();
    gui.add(scale.set("scale", 4.0, 0.0, 10.0));
    gui.add(imageSize.set("imageSize", 1.0, 0.0, 2.0));
}

//--------------------------------------------------------------
void ofApp::update(){
    
    currentImageIndex = (currentImageIndex + 1) % images.size();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate(-ofGetMouseX() * (scale - 0.5), -ofGetMouseY() * (scale - 0.5));
    for (int i=0; i<imagePoints.size(); i++) {
        float x = ofMap(imagePoints[i][0], 0, 1, 0, scale * ofGetWidth());
        float y = ofMap(imagePoints[i][1], 0, 1, 0, scale * ofGetHeight());
        images[i].draw(x, y, imageSize * images[i].getWidth(), imageSize * images[i].getHeight());
        ofDrawBitmapString(filenames[i], x + 14, y + 28);

        //        ofNoFill();
        //        ofDrawCircle(x, y, scale*5);
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(-ofGetMouseX() * (scale - 0.5), -ofGetMouseY() * (scale - 0.5));
    int i = currentImageIndex;
    float x = ofMap(imagePoints[i][0], 0, 1, 0, scale * ofGetWidth());
    float y = ofMap(imagePoints[i][1], 0, 1, 0, scale * ofGetHeight());
    images[i].draw(x, y, imageSize * images[i].getWidth(), imageSize * images[i].getHeight());
    
    ofDrawBitmapString(filenames[i], x + 14, y + 28);
    
    ofNoFill();
    ofSetColor(ofColor::yellow);
    ofDrawRectangle(x, y,imageSize * images[i].getWidth(), imageSize * images[i].getHeight());
    
    ofPopMatrix();

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
    
}
