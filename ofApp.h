#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.h"
#include "Particle.h"
#include "ParticleEmitter.h"


typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;



class Emitter;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void checkCollisions();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y) {}
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button) {}
		void mouseEntered(int x, int y) {}
		void mouseExited(int x, int y) {}
		void windowResized(int w, int h) {}
		void dragEvent(ofDragInfo dragInfo) {}
		void gotMessage(ofMessage msg) {}
		
		Emitter *invaders, *gun,*invader2;
		int score;
        int playerLife;
        string gameState;
		float gameStartTime;
		bool gameOver = false;
		bool gameStart = false;
        bool endGame = false;
    
        bool isUpPressed = false;
        bool isDownPressed = false;
        bool isLeftPressed = false;
        bool isRightPressed = false;
        bool mediumDifficulty = false;
        bool hardDifficulty = false;
        bool impossibleDifficulty = false;
        bool reset = false;
        bool startDifficulty = false;
		
        //Vector for mouses position
        ofVec3f mouseLast;

        //Setting up all sounds
		ofSoundPlayer shot;
		ofSoundPlayer explosion;
        ofSoundPlayer shipExplosion;
        ofSoundPlayer music;
        
        //Setting up images
        ofImage start;
        ofImage bullet;
		ofImage ship;
        ofImage enemies;
		ofImage invaderEmitter;
        ofImage backGround;
        
        //Setting up GUI
        ofxPanel gui;
        ofxFloatSlider rate;
        ofxFloatSlider Bullrate;
        ofxFloatSlider lifeSpan;

        //setting up emitter
        ParticleEmitter emitter;
        ParticleEmitter emitter2;
        Particle player;
        ParticleSystem sys;

        //Forces
        TurbulenceForce *turbForce;
        GravityForce *gravityForce;
};


