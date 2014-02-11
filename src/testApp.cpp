#include "testApp.h"

bool sortHotelByRating(ofVec2f i, ofVec2f j){
    return i.y < j.y;
}

//--------------------------------------------------------------
void testApp::setup(){
    csv.loadFile(ofToDataPath("hotelsbase.txt"),"~");

    maxid = -1;
    maxPrice = -1.0;
    maxRooms = -1;
    maxRating = 10.0;
    for (int i = 0; i < csv.numRows - 1; i++) {
        if (getChainId(i) > maxid)
            maxid = getChainId(i);
        if (getPrice(i) > maxPrice)
            maxPrice = getPrice(i);
        if (getRooms(i) > maxRooms)
            maxRooms = getRooms(i);
    }
    
    cout << "maxid:" << maxid << endl;
    
    vector<ofVec2f> priceSort;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int hotel = 0; hotel < csv.numRows -1; hotel++) {
        
        map<float,vector<int> >::iterator it = ratingMap.find(getRating(hotel));
        if (it == ratingMap.end())
            ratingMap.insert(std::pair<float,vector<int> > (getRating(hotel),vector<int>(hotel)));
        else
            (*it).second.push_back(hotel);
        //cout << (*it).second.size() << endl;
        //push_back(hotel);
    
        float earthRadius = 300;
        float latitude = ofDegToRad(getLatitude(hotel));
        float longitude = ofDegToRad(getLongitude(hotel));
        float x = earthRadius * cos(latitude) * cos(longitude);
        float y = earthRadius * cos(latitude) * sin(longitude);
        float z = earthRadius * sin(latitude);
        //positions.push_back(ofVec3f(x,y,z));
        
        float hue;
        float saturation;
        float brightness;
        hue = getChainId(hotel)/maxid;
        saturation = 0.4;
        brightness = 1;
        
        ofFloatColor color = ofFloatColor::fromHsb((float)25/255,saturation,brightness);
        /*if (getChainId(hotel) != 0){
            color = ofFloatColor::fromHsb(mapChainColor(hotel), saturation, brightness);
        }*/
        
        
        hotelColors.push_back(color);
        mesh.addVertex(ofVec3f(x,y,z));
        mesh.addNormal(ofVec3f(x,y,z));
        //mesh.addColor(color);
    }
    mesh.clearColors();
    mesh.addColors(hotelColors);
    
    //std::sort(ratingSort.begin(),ratingSort.end(),sortHotelByRating);

    
    sphere = ofMesh::sphere(300,100,OF_PRIMITIVE_TRIANGLE_STRIP);
    vector< ofFloatColor > sphereCol;
    for (int i = 0; i < sphere.getVertices().size(); i++){
        sphereCol.push_back(ofFloatColor(0,0,0,1));
    }
    sphere.addColors(sphereCol);
    sphere.clearNormals();
    sphere.addNormals(sphere.getVertices());
    //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofSetGlobalAmbientColor(ofColor(0,0,0,0));
    light.setup();
    light.setSpotlight();
    light.lookAt(ofVec3f(0,0,0));
    
    cam.setGlobalPosition(-37.6022, -647.508, 147.265);
    cam.setTarget(ofVec3f(0,0,0));
    
     //cam.setAutoDistance(true);
    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<FxaaPass>()->setEnabled(true);
    post.createPass<LimbDarkeningPass>()->setEnabled(true);
    post.createPass<BloomPass>()->setEnabled(true);
    
    
    
    lowRating = 0.0;
    highRating = 10.0;
    drawPadding = false;
    gui = new ofxUISuperCanvas("Settings");
    //gui->addLabel("TOGGLES");
    gui->addToggle("Color Chain Hotels", false, 44, 44);
    //gui->addToggle("Normalize Brightness by Price", false, 44, 44);
    gui->addSpacer();
    gui->addLabel("Filter by Rating");
	gui->addRangeSlider("Filter by Rating", 0.0, 10.0, &lowRating, &highRating);
    gui->addSpacer();
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);


}


float testApp::mapChainColor(int hotelNum) {
    float ratio = (float)getChainId(hotelNum)/maxid;
    /*cout << "min color: " << (float)42/255 << endl;
    cout << "max color: " << (float)205/255 << endl;
    cout << "chain Color: " << (ratio*163)/255 + (float)42/255 << endl;*/
    return (ratio*163 + 42)/255;
    
}
//--------------------------------------------------------------
void testApp::update(){
    /*cout << "cam distance: " << cam.getDistance() << endl;
    cout << "cam position: " << cam.getGlobalPosition() << endl;
    cout << "cam oriention: " << cam.getGlobalOrientation() << endl;*/
    if (cam.getDistance() < 210) {
        cam.setDistance(cam.getDistance()+1);
    }
    mesh.clearColors();
    mesh.addColors(hotelColors);
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // copy enable part of gl state
    glPushAttrib(GL_ENABLE_BIT);
    
    // setup gl state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    

    post.begin(cam);
    light.setPosition(cam.getPosition());
    light.lookAt(cam.getTarget());
    light.enable();
    
    ofPushMatrix();
        sphere.drawFaces();
    ofPopMatrix();
   
    ofPushMatrix();
        mesh.draw();
    ofPopMatrix();
    
    light.disable();
    
    // end scene and draw
    post.end();
    
    // set gl state back to original
    glPopAttrib();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    mouseDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    mouseDown = false;

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(name == "Color Chain Hotels")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value: " << toggle->getValue() << endl;
        for (int hotel = 0; hotel < hotelColors.size(); hotel++) {
            ofFloatColor prevColor = hotelColors.at(hotel);
            float hue;
            float saturation;
            float brightness;
            prevColor.getHsb(hue, saturation, brightness);
            
            if (toggle->getValue()) {
                chainsColored = true;
                if (getChainId(hotel) > 0) {
                    hue = mapChainColor(hotel);
                    saturation = 1.0;
                }
            }
            else {
                chainsColored = false;
                hue = 25.0/255;
                saturation = 0.4;
            }

            hotelColors.at(hotel) = ofFloatColor::fromHsb(hue,saturation,brightness);
        }
    }
    
    else if(name == "Normalize Brightness by Price")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value: " << toggle->getValue() << endl;
        for (int hotel = 0; hotel < hotelColors.size(); hotel++) {
            ofFloatColor prevColor = hotelColors.at(hotel);
            float hue;
            float saturation;
            float brightness;
            prevColor.getHsb(hue, saturation, brightness);
            
            if (toggle->getValue()){
                brightness = atan(PI*getPrice(hotel)/maxPrice/2);
                brightnessNormalized = true;
            }
            else {
                brightness = 1.0;
                brightnessNormalized = false;
            }
            
            hotelColors.at(hotel) = ofFloatColor::fromHsb(hue,saturation,brightness);
        }
    }
    else if(name == "Filter by Rating")
    {
        ofxUIRangeSlider *ratingSlider = (ofxUIRangeSlider *) e.widget;
        //cout << "value low: " << ratingSlider->getValueLow() << endl;
        //cout << "value high: " << ratingSlider->getValueHigh() << endl;
        float lowVal = ratingSlider->getValueLow();
        float highVal = ratingSlider->getValueHigh();
        if (!mouseDown){
            for(int hotel = 0; hotel < hotelColors.size(); hotel++) {
                
                float rating = getRating(hotel);
                ofFloatColor prevColor = hotelColors.at(hotel);
                float hue;
                float saturation;
                float brightness;
                
                prevColor.getHsb(hue, saturation, brightness);
                
                if (rating < lowVal || rating > highVal){
                    brightness = 0.0;
                }
                else if(brightnessNormalized)
                    brightness = atan(PI*getPrice(hotel)/maxPrice/2);
                else
                    brightness = 1.0;
                
                if(chainsColored){
                    if (getChainId(hotel) > 0) {
                        hue = mapChainColor(hotel);
                        saturation = 1.0;
                    }
                    else {
                        hue = 25.0/255;
                        saturation = 0.4;
                    }
                }
                else{
                    hue = 25.0/255;
                    saturation = 0.4;
                }
        
                hotelColors.at(hotel) = ofFloatColor::fromHsb(hue,saturation,brightness);
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::exit()
{
    delete gui;
}


int testApp::getId(int hotel){
    return csv.getInt(hotel+1,0);
}

string testApp::getHotelName(int hotel){
    return csv.getString(hotel+1,1);
}

float testApp::getStars(int hotel){
    return csv.getFloat(hotel+1,2);
}

float testApp::getPrice(int hotel){
    return csv.getFloat(hotel+1,3);
}

string testApp::getCityName(int hotel){
    return csv.getString(hotel+1,4);
}

string testApp::getStateName(int hotel){
    return csv.getString(hotel+1,5);
}

string testApp::getCountryCode(int hotel){
    return csv.getString(hotel+1,6);
}

string testApp::getCountryName(int hotel){
    return csv.getString(hotel+1,7);
}

string testApp::getAddress(int hotel){
    return csv.getString(hotel+1,8);
}

string testApp::getLocation(int hotel){
    return csv.getString(hotel+1,9);
}

string testApp::getUrl(int hotel){
    return csv.getString(hotel+1,10);
}

string testApp::getTripAdvisorUrl(int hotel){
    return csv.getString(hotel+1,11);
}

float testApp::getLatitude(int hotel){
    return csv.getFloat(hotel+1,12);
}

float testApp::getLongitude(int hotel){
    return csv.getFloat(hotel+1,13);
}

float testApp::getLatLong(int hotel){
    return csv.getFloat(hotel+1,14);
}

int testApp::getPropertyType(int hotel){
    return csv.getInt(hotel+1,15);
}

int testApp::getChainId(int hotel){
    return csv.getInt(hotel+1,16);
}

int testApp::getRooms(int hotel){
    return csv.getInt(hotel+1,17);
}

string testApp::getFacilities(int hotel){
    return csv.getString(hotel+1,18);
}

string testApp::getCheckIn(int hotel){
    return csv.getString(hotel+1,19);
}

string testApp::getCheckOut(int hotel){
    return csv.getString(hotel+1,20);
}

float testApp::getRating(int hotel){
    return csv.getFloat(hotel+1,21);
}

int testApp::lonToXPixel(float longitude){
    return (int) ((ofGetWidth()/360.0) * (180 + longitude));
}

int testApp::latToYPixel(float latitude) {
    
    return (int) ((ofGetHeight()/180.0) * (90 - latitude));
}




