#include "ofApp.h"

#include "Emitter.h"

//edited minigame file provided by professor
void ofApp::setup() {
  //Setting Frame rate
  ofSetVerticalSync(true);
  ofSetFrameRate(60);

  //sets the game state to start
  gameState = "Start";

  //loads all sprites and sounds needed
  ship.load("sprites/ship.png");
  invaderEmitter.load("sprites/InvaderEmitter.png");
  start.load("background2.png");
  bullet.load("sprites/bullet.png");
  enemies.load("sprites/enemies.png");
  explosion.load("explosion.mp3");
  shipExplosion.load("shipExplosion.mp3");
  music.load("back.mp3");
  backGround.load("background.png");
  
  //set score to 0 and set Player life
  score = 0;
  playerLife = 100;

  //Get the start game time
  gameStartTime = ofGetElapsedTimeMillis();

  // Creates and initializes emitters.
  gun = new Emitter(new SpriteSystem());
  invaders = new Emitter(new SpriteSystem());
  invader2 = new Emitter(new SpriteSystem());

  //forces
   ParticleSystem *sys = emitter2.sys;
   ofVec3f tracking;
   for (int i = 0; i < emitter2.sys -> particles.size(); i++){
        ofVec3f tracking = gun -> trans - emitter2.sys->particles[i].position ;
   }
   emitter2.sys->addForce(new GravityForce(tracking));
   turbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
   emitter2.sys -> addForce(turbForce);
  // setting up the gun
  gun -> haveImage = true;
  gun -> setImage(ship);
  gun -> haveChildImage = true;
  gun -> setChildImage(bullet);
  gun -> setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() - 10, 0));
  gun -> setVelocity(ofVec3f(0, -1000, 0));
  gun -> setRate(0);
  gun -> setLifespan(500);

  // Setting up invader emitter one.
  invaders -> haveImage = true;
  invaders -> setImage(invaderEmitter);
  invaders -> haveChildImage = true;
  invaders -> setChildImage(enemies);
  invaders -> setPosition(ofVec3f(ofGetWindowWidth()/4 , 10, 0));
  invaders -> velocity.set(0, 350, 0);
  invaders -> setLifespan(4000);

  // Setting up invader emitter two.
  invader2 -> haveImage = true;
  invader2 -> setImage(invaderEmitter);
  invader2 -> haveChildImage = true;
  invader2 -> setChildImage(enemies);
  invader2 -> setPosition(ofVec3f(ofGetWindowWidth() / 2, 10, 0));
  invader2 -> velocity.set(0, 350, 0);
  invader2 -> setLifespan(4000);

  //set up particle emitter
  emitter.setOneShot(true);
  emitter.setEmitterType(RadialEmitter);
  emitter.setGroupSize(100);
  emitter.setLifespan(5);
  emitter.setVelocity(ofVec3f(1000, 0, 0));
  emitter.setParticleRadius(5);

  //set up shooting star particle emitter
  emitter2.setGroupSize(1);
  emitter2.setLifespan(20);
  emitter2.setParticleRadius(5);

  // Start player & enemy.
  invaders -> start();
  invader2 -> start();
  gun -> start();

}

//--------------------------------------------------------------
//
void ofApp::update() {
  //Emitter updates
  emitter2.update();
  emitter.update();
  emitter2.sys->update();
  invaders -> update();
  invader2 -> update();
  gun -> update();
  //movement vec
  ofVec3f offset;

  //Explosion emitter setup
  emitter.setPosition(ofVec3f(gun -> trans.x, gun -> trans.y, 0));

  //Shooting star enemy setup
  emitter2.setPosition(ofVec3f(-10, 0, 0));
  emitter2.setVelocity(gun->trans);
  emitter2.setRate(.35);
  //Changing level difficulities
    if(startDifficulty)
    {
        invaders->setRate(1);
        invader2->setRate(1);
    }

    if (mediumDifficulty) {
    invaders -> setRate(3);
    invader2 -> setRate(3);
    emitter2.start();

  }

  if (hardDifficulty) {
    invaders -> setRate(5);
    invader2 -> setRate(5);
    emitter2.setRate(1);

  }

  if (impossibleDifficulty) {
    invaders -> setRate(10);
    invader2 -> setRate(10);
    emitter2.setRate(2);

  }

  // check for collisions between missles and invaders
  checkCollisions();

  // we will randomize initial velocity so that not the invaders
  // are going at the same speed just to make it a little harder/interesting
  // to play
  //
  ofVec3f invV = invaders -> velocity;
  invaders -> setVelocity(ofVec3f(ofRandom(-invV.y / 2, invV.y / 2), invV.y, invV.z));
  ofVec3f invV2 = invader2 -> velocity;

  invader2 -> setVelocity(ofVec3f(ofRandom(-invV2.y / 2, invV2.y / 2), invV2.y, invV2.z));

  //game ends
  if (gameStart == false) {
    gun -> stop();
    invaders -> stop();
    invader2 -> stop();
  }
  //Defines what happens when keys are pressed
  if (isUpPressed)

  {
    offset = ofVec3f(0, -5, 0);
  }

  if (isDownPressed) {
    offset = ofVec3f(0, 5, 0);

  }

  if (isLeftPressed) {
    offset = ofVec3f(-5, 0, 0);
  }
  if (isRightPressed) {
    offset = ofVec3f(5, 0, 0);
  }

  gun -> trans += offset;

}

void ofApp::draw() {
  //Draws emitters
  //emitter.draw();
//emitter2.draw();

  //Draws the start screen
    ofSetBackgroundColor(ofColor::black);
    start.draw(ofGetWidth() / 2 - start.getWidth() / 2, ofGetHeight() / 2 - start.getHeight() / 2);
  //Describe what happens when gameState is play
  if (gameState == "play") {
    //ofSetBackgroundColor(ofColor::black);
    backGround.draw(0, 0, ofGetWidth(), ofGetHeight());
     emitter2.sys->draw();
    start.clear();
    gui.draw();
    gun -> draw();
    invaders -> draw();
    invader2 -> draw();
      if (!gun -> started) {
                 gameStartTime = ofGetElapsedTimeMillis();
                 gameOver = false;
                 gun -> start();
                 invaders -> start();
                 invader2 -> start();
               }
  }

  //Draw all bitmaps
  string scoreText;
  scoreText += "Score: " + std::to_string(score);
  ofDrawBitmapString(scoreText, ofPoint(10, 20));

  string playerHealth;
  playerHealth += "Player Health: " + std::to_string(playerLife);
  ofDrawBitmapString(playerHealth, ofPoint(10, 33));

  string gameOver;
  gameOver += "GAME OVER : SCORE " + std::to_string(score) + "  Press Command to restart";

  if (endGame) {
    ofDrawBitmapString(gameOver, ofPoint(ofGetWidth() / 2 - 175, ofGetHeight() / 2));
  }
    //defines the reset
     if(reset)
     {
         gun -> setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() - 10, 0));
         endGame =false;
         music.play();
         score = 0;
         playerLife = 100;
         invaders -> start();
         invader2 -> start();
     }
   emitter.draw();
    
}

//  This is a simple O(M x N) collision check
//  For each missle check to see which invaders you hit and remove them
//
void ofApp::checkCollisions() {

  // find the distance at which the two sprites (missles and invaders) will collide
  // detect a collision when we are within that distance.
  float collisionDist = gun -> childHeight / 2 + invaders -> childHeight / 2;

  // Loop through all the missiles, then remove any invaders that are within
  // "collisionDist" of the missiles.  the removeNear() function returns the
  // checks number of enemies removed for enemy invader.
  for (int i = 0; i < gun -> sys -> sprites.size(); i++) {
    int counter = invaders -> sys -> removeNear(gun -> sys -> sprites[i].trans, collisionDist);

    if (counter > 0) {
      explosion.play();
    }
    score += counter;
  }

  // checks number of enemies removed for enemy invader 2.
  for (int i = 0; i < gun -> sys -> sprites.size(); i++) {
    int counter = invader2 -> sys -> removeNear(gun -> sys -> sprites[i].trans, collisionDist);

    if (counter > 0) {
      explosion.play();
    }
    score += counter;
  }
  //collision detection for ship
  float shipDist = gun -> height / 2 + invaders -> childHeight / 2;

  for (int i = 0; i < invaders -> sys -> sprites.size(); i++) {
    ofVec3f v = gun -> trans - invaders -> sys -> sprites[i].trans;
    if (v.length() < shipDist) {
      playerLife -= 25;
      invaders -> sys -> sprites[i].trans = ofVec3f(1000, 1000, 0);
      if (playerLife <= 0) {
        emitter.start();
        gameStart = false;
        invaders -> sys -> sprites.clear();
        invader2 -> sys -> sprites.clear();
        emitter2.sys -> particles.clear();
        emitter2.stop();
        gun -> setPosition(ofVec3f(1000, 1000, 100));
        shipExplosion.play();
        endGame = true;

      }
    }
    //collision detection for enemy on ship
    for (int i = 0; i < invader2 -> sys -> sprites.size(); i++) {
      ofVec3f s = gun -> trans - invader2 -> sys -> sprites[i].trans;
      if (s.length() < shipDist) {
        playerLife -= 25;
        invader2 -> sys -> sprites[i].trans = ofVec3f(1000, 1000, 0);
        if (playerLife <= 0) {
          emitter.start();
          gameStart = false;
            invaders->stop();
            invader2->stop();
        invaders -> sys -> sprites.clear();
          invader2 -> sys -> sprites.clear();
          emitter2.sys -> particles.clear();
          emitter2.stop();
          gun -> setPosition(ofVec3f(1000, 1000, 100));
          shipExplosion.play();
          endGame = true;
        }
      }

    }

    //collision detection for shooting star on ship
    for (int i = 0; i < emitter2.sys -> particles.size(); i++) {
      ofVec3f s2 = gun -> trans - emitter2.sys -> particles[i].position;
      if (s2.length() <= shipDist) {
        playerLife -= 50;
        emitter2.sys -> particles[i].position = ofVec3f(1000, 1000, 0);
        if (playerLife <= 0) {
          emitter.start();
          gameStart = false;
          invaders -> sys -> sprites.clear();
          invader2 -> sys -> sprites.clear();
          emitter2.sys -> particles.clear();
          emitter2.stop();
          gun -> setPosition(ofVec3f(1000, 1000, 100));
          shipExplosion.play();
          endGame = true;

        }

      }
    }

      if (score < 30 )
      {
          startDifficulty = true;
          impossibleDifficulty = false;
          mediumDifficulty = false;
          hardDifficulty = false;
      }
    //changing the difficulty based on score
    if (score > 30) {
      mediumDifficulty = true;
      startDifficulty = false;

    }

    if (score > 50) {
      hardDifficulty = true;
      mediumDifficulty = false;
    }

    if (score > 100) {
      impossibleDifficulty = true;
      hardDifficulty = false;
    }

  }
}

//  support dragging of gun with mouse
void ofApp::mouseDragged(int x, int y, int button) {
  ofPoint mouse_cur = ofPoint(x, y);
  ofVec3f delta = mouse_cur - mouseLast;

  // Allow gun to be moved in lower half of screen only
  //
  if ((gun -> trans.y + delta.y) > ofGetWindowHeight() / 2) {
    gun -> trans += delta;
    mouseLast = mouse_cur;
  }
}

//Stop shooting when spacebar is released and set all booleans to false when keys are released
void ofApp::keyReleased(int key) {
  switch (key) {
  case (32):
    gun -> setRate(0);
  case (OF_KEY_LEFT):
    isLeftPressed = false;
    break;
  case (OF_KEY_RIGHT):
    isRightPressed = false;
    break;
  case (OF_KEY_UP):
    isUpPressed = false;
    break;
  case (OF_KEY_DOWN):
    isDownPressed = false;
    break;
      case (OF_KEY_COMMAND) :
          reset = false;
          break;
  }
}

//Booleans set to true when clicked
void ofApp::keyPressed(int key) {
  ofVec3f offset;

  switch (key) {
  case (32):
          if(startDifficulty){
            gun -> setRate(5);
          }
    if (mediumDifficulty) {
      gun -> setRate(3);
    }
    if (hardDifficulty) {
      gun -> setRate(3);
    }
    if (impossibleDifficulty) {
      gun -> setRate(4);
    }
    break;
  case (OF_KEY_UP):
    isUpPressed = true;

    break;
  case (OF_KEY_DOWN):
    isDownPressed = true;
    break;
  case (OF_KEY_LEFT):
    isLeftPressed = true;
    break;
  case (OF_KEY_RIGHT):
    isRightPressed = true;
    break;
    //Command starts the game
  case OF_KEY_COMMAND:
    gameState = "play";
    reset = true;

    if (!gameStart) {
      gameStart = true;

    }
    break;
  case OF_KEY_TAB:
    emitter2.start();
    break;
  }

}

//--------------------------------------------------------------
// Starts game & tracks mouse movements.
//
void ofApp::mousePressed(int x, int y, int button) {
  if (!gun -> started) {
    gameStartTime = ofGetElapsedTimeMillis();
    gameOver = false;
    gun -> start();
    invaders -> start();
    invader2 -> start();
    score = 0;
  }
  mouseLast = ofPoint(x, y);
}
