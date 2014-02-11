#pragma once

#include "ofMain.h"
#include "ofxCsv.h"
#include "ofxPostProcessing.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        bool mouseDown;
    
        wng::ofxCsv csv;
        wng::ofxCsv csvRecorder;
    
        ofMesh mesh;
        ofMesh sphere;
        vector<ofMesh> hotelMeshes;
        vector<ofVec3f> positions;
    
        ofEasyCam cam;
    
        ofLight light;
    
        vector<ofFloatColor> hotelColors;
        map<float,vector<int> > ratingMap;
    
        int getId(int hotel);
        string getHotelName(int hotel);
        float getStars(int hotel);
        float getPrice(int hotel);
        string getCityName(int hotel);
        string getStateName(int hotel);
        string getCountryCode(int hotel);
        string getCountryName(int hotel);
        string getAddress(int hotel);
        string getLocation(int hotel);
        string getUrl(int hotel);
        string getTripAdvisorUrl(int hotel);
        float getLatitude(int hotel);
        float getLongitude(int hotel);
        float getLatLong(int hotel);
        int getPropertyType(int hotel);
        int getChainId(int hotel);
        int getRooms(int hotel);
        string getFacilities(int hotel);
        string getCheckIn(int hotel);
        string getCheckOut(int hotel);
        float getRating(int hotel);
        int maxid;
        float maxPrice;
        int maxRooms;
        float maxRating;
        float mapChainColor(int hotelNum);
        float lowRating;
        float highRating;
        bool brightnessNormalized;
        bool chainsColored;
        string lowerRating;
        string upperRating;
    
    
        int lonToXPixel(float longitude);
        int latToYPixel(float latitude);
    
        ofxPostProcessing post;
        const ofVec3f & changecol = ofVec3f(0.0, 0.0, 0.0);
    
    
        ofxUICanvas *gui;
        void guiEvent(ofxUIEventArgs &e);
        void exit(); 
        bool drawPadding;

    
};
