/**************************************************************
  File:      me210-project.ino
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Powers a basketball playing Skip-to-my-Loop robot

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

/*---------------- Includes ---------------------------------*/
// ME210 Timers included for timing
#include "Timers.h"

// Setup for servo
#include <Servo.h>
Servo arm;

// Our motor module
#include "Motors.h"
// Our bumber module
#include "Bumpers.h"
// Our tape sensor module
#include "TapeSensors.h"
// Our sonar sensor module
#include "Sonar.h"



/*---------------- Module Defines ---------------------------*/

/* State Machine. Prefix S_, middle abbreviation for functional component */ 
#define S_START             0
#define S_DEBUG             1
// Find line
#define S_FL_FINDLINE       2
#define S_FL_TURNONLINE     3
#define S_GET_BALLS         4
// Go Forward
#define S_GF_FWD            5
// Go Reverse
#define S_GR_REV            7
#define S_GR_RELOAD        10
#define S_DUNK             11

// NEVER WRITE TO THIS VARIABLE DIRECTLY, ALWAYS USE setState()!
unsigned char state = S_START; // Global;


/* Timers */
#define T_DEBUG              0
#define T_DEBUG_INTERVAL  15
#define T_GAME               1
#define T_GAME_LENGTH     120000 // 120 Seconds = 2 minutes

/* Which side of the center line are we on? */
#define SIDE_UNKNOWN     0
#define SIDE_LEFT        1
#define SIDE_RIGHT       2
unsigned char startArenaSide = SIDE_UNKNOWN;

/* Which way did we last turn? */
#define NO_TURN          0
#define TURN_LEFT        1
#define TURN_RIGHT       2

/* Sensor parameters and thresholds */
#define HALF_FIELD_WIDTH 19 // Half the field width minus offset, used as threshold for ultrasonic side sensing on startup
#define SONAR_START_ACCURACY_PINGS 20 // How many sonar pings we send off and average over when we start the game
#define SONAR_LOOP_ACCURACY_PINGS   5 // How many sonar pings we send off and average over when periodically checking distance (blocking!)

int temp_debug = true;
static unsigned char lastTurn = NO_TURN;

/*---------------- Module Function Prototypes ---------------*/

/*---------------- Arduino Main Functions -------------------*/
void setup() {  // setup() function required for Arduino
  Serial.begin(9600);
  Serial.println("Skip to my loop!");
  
  arm.attach(6);

  initBumpers();
  initMotors();
  initTapeSensors();
  initSonar();

  TMRArd_InitTimer(T_DEBUG, T_DEBUG_INTERVAL);
  TMRArd_InitTimer(T_GAME, T_GAME_LENGTH);

  setMotorSpeed(0);
  // TODO: read endgame switch, set flag to get only one ball
  setState(S_START);  
  delay(2000);
}

void loop() {  // loop() function required for Arduino
  // Break after game
  if (TMRArd_IsTimerExpired(T_GAME)) {
    setMotorSpeed(0);
    arm.write(50); // Wiggle wiggle wiggle
    delay(400);
    arm.write(130);
    delay(400);
    return;
  };

// Note: debugging via serial connection is blocking and breaks line following
// if (TMRArd_IsTimerExpired(T_DEBUG)) timedDebug();
//  if(Serial.available() || temp_debug == true) {
//    temp_debug = false;
//    Serial.println("STOPPED");
//    setMotorSpeed(0);
//    unsigned char c;
//    c = Serial.read();
//    while (!Serial.available()) { delay(100); }
//    c = Serial.read();
//    Serial.println("CONTINUED");
//    setState(S_START);
//  }

  int left, right;
  switch (state) {
    case S_START:
      int startDistanceToFront;
      // Send SONAR_START_ACCURACY_PINGS pings, average distance
      startDistanceToFront = getSonarFrontDistanceInInches(SONAR_START_ACCURACY_PINGS);
      
      startArenaSide = (startDistanceToFront < HALF_FIELD_WIDTH) ? SIDE_LEFT : SIDE_RIGHT;

      left = isLeftSensorOnTape();
      right = isRightSensorOnTape();
      if (left || right) {
        setState(S_FL_TURNONLINE);
      } else {
        setState(S_FL_FINDLINE);
      }

      break;
    case S_FL_FINDLINE:
      /* If we think we are in the left side (hence going towards the right) and end up half-way between center line 
         and the right side, we were on the right side inititally (or we missed the line). In that case, reverse!   */
      // NOTE: this is somewhat blocking code, keep in mind for debug!
      // int curDistanceToFront;
      // curDistanceToFront = getSonarFrontDistanceInInches(SONAR_LOOP_ACCURACY_PINGS);
      // if (startArenaSide == SIDE_LEFT && curDistanceToFront >= HALF_FIELD_WIDTH * 3 / 2) reverseMotors();
      // if (startArenaSide == SIDE_RIGHT && curDistanceToFront <= HALF_FIELD_WIDTH * 3 / 2) reverseMotors();      

      left = isLeftSensorOnTape();
      right = isRightSensorOnTape();

      if (left && right) {
        // Break the bot
        if (startArenaSide == SIDE_LEFT) {
          setLeftMotorSpeed(170);
          setRightMotorSpeed(160);
        } else {
          setLeftMotorSpeed(-170);
          setRightMotorSpeed(-160);
        }
        delay(150);
        setMotorSpeed(0);
        delay(1500);
        setState(S_FL_TURNONLINE);
      }
      break;
    case S_FL_TURNONLINE:
      if (isFrontSensorOnTape()) {
          setLeftMotorSpeed(-150);
          setRightMotorSpeed(150);
          delay(135);
          setMotorSpeed(0);
          delay(1500);
          setState(S_GET_BALLS);
      }
      break;
    case S_GET_BALLS:
      while(!isAnyBackBumperPressed()) {
        setLeftMotorSpeed(-220);
        setRightMotorSpeed(-230);
      }
      setState(S_GR_RELOAD);
    case S_GF_FWD:
      lineFollowFWD();
      if (isAnyFrontBumperPressed()) setState(S_DUNK);
      break;
    case S_GR_REV:
      lineFollowREV();
      if (isAnyBackBumperPressed()) setState(S_GR_RELOAD);
      break;
    case S_DUNK:
      break;
    default:
      break;
  }  
}


/*------------------ State Machine Functions -------------*/
void setState (unsigned int newState) {
  state = newState;
  // Serial.print("State change: ");
  // Serial.println(newState);
  signed int arenaTurnSign;
  
  switch (newState) {
    case S_START:
      break;
    case S_DEBUG:
      setMotorSpeed(0);
      Serial.println("DEBUG");
      delay(10000);
      break;
    case S_FL_FINDLINE:
      if (startArenaSide == SIDE_LEFT) {
        // High PWM burst to ensure motor running
        setLeftMotorSpeed(-255);
        setRightMotorSpeed(-255);
        delay(20);
        // And then these are the real values
        setLeftMotorSpeed( -170);
        setRightMotorSpeed(-170);
      } else {
        // High PWM burst to ensure motor running
        setLeftMotorSpeed(255);
        setRightMotorSpeed(255);
        delay(20);
        // And then these are the real values
        setLeftMotorSpeed( 170);
        setRightMotorSpeed(170);
      } 
      break;
    case S_FL_TURNONLINE:
      // High PWM burst to ensure motor running
      setLeftMotorSpeed(255);
      setRightMotorSpeed(-255);
      delay(20);
      // And then these are the real values
      setLeftMotorSpeed(  170);
      setRightMotorSpeed(-170);
      break;
    case S_GF_FWD:
      setLeftMotorSpeed( 215);
      setRightMotorSpeed(215);
      break;
    case S_GR_REV:
      setLeftMotorSpeed( -215);
      setRightMotorSpeed(-215);
      break;
    case S_GR_RELOAD:
    // TODO: RELOAD and GETBALLS are redundant, get rid of getballs again
      requestBalls(3);
      /**if(!isLeftSensorOnTape() && !isRightSensorOnTape()) {
        while(!isFrontSensorOnTape()) {
          setLeftMotorSpeed(-165);
          setRightMotorSpeed(165);
        }
      }
      delay(1000);*/

      setState(S_GF_FWD);
      break;
    case S_DUNK:
      dunkBalls();
      setState(S_GR_REV);
      break;
    default: 
      Serial.println("Invalid state requested: ");
      Serial.println(newState);
  }
}




/* Blocking transition functions */

// Will request the specified number of balls by hitting the bumper and waiting
void requestBalls(char numBalls) {
  // assert numBalls correct
  if (numBalls < 1 or numBalls > 3) return;

  // Get required number of balls
  for (int iBall = 1; iBall <= numBalls; iBall++) {
    // Make sure we hit the bumper
    while (!isAnyBackBumperPressed()) {
      //lineFollowREV();
      setLeftMotorSpeed(-215);
      setRightMotorSpeed(-205);
    }

    // Wait for a moment to show off pressed state
    setMotorSpeed(0);
    delay(500);

    // Release the bumper
    while (isAnyBackBumperPressed()) {
      //lineFollowFWD();
      setLeftMotorSpeed(223);
      setRightMotorSpeed(230);
    }
    delay(150);

    // Wait for ball
    setMotorSpeed(0);
    delay(2000);   
  }

  // Realign the bot to the line
  if (!isLeftSensorOnTape() && !isRightSensorOnTape()) {
    // Align bot to wall again
    while (!isAnyBackBumperPressed()) {
      setLeftMotorSpeed(-215);
      setRightMotorSpeed(-205);
    }
    setMotorSpeed(0);

    int curDistanceToLeft, curArenaSide;
    curDistanceToLeft = getSonarLeftDistanceInInches(SONAR_START_ACCURACY_PINGS);
    curArenaSide = (curDistanceToLeft < HALF_FIELD_WIDTH) ? SIDE_LEFT : SIDE_RIGHT;

    // Turn towards center line
    while (!isFrontSensorOnTape()) {
      setLeftMotorSpeed((curArenaSide == SIDE_LEFT) ? 170 : -170);      
      setRightMotorSpeed((curArenaSide == SIDE_LEFT) ? -170 : 170);
    }

    // Break turning motion
    setLeftMotorSpeed((curArenaSide == SIDE_LEFT) ? -150 : 150);
    setRightMotorSpeed((curArenaSide == SIDE_LEFT) ? 150 : -150);
    delay(70);
    setMotorSpeed(0);
    delay(150);

    // Go forward onto line
    while (!isLeftSensorOnTape() && !isRightSensorOnTape()) {
      setLeftMotorSpeed( 170);
      setRightMotorSpeed(170);      
    }

    // Break the forward movement
    setLeftMotorSpeed(-160);
    setRightMotorSpeed(-160);
    delay(100);
    setMotorSpeed(0);
    delay(300);

    // Turn towards center line
    while (!isFrontSensorOnTape()) {
      setLeftMotorSpeed((curArenaSide == SIDE_LEFT) ? -170 : 170);      
      setRightMotorSpeed((curArenaSide == SIDE_LEFT) ? 170 : -170);
    }

    // Break turning motion
    setLeftMotorSpeed((curArenaSide == SIDE_LEFT) ? 150 : -150);
    setRightMotorSpeed((curArenaSide == SIDE_LEFT) ? -150 : 150);
    delay(100);
    setMotorSpeed(0);
    delay(150);

    // And off we go!
  }
}

void dunkBalls() {
  setMotorSpeed(0);
  delay(300);
  
  // Make sure the servo is initially stopped
  arm.write(90);
  delay(500);
  
  // Move the arm up
  arm.write(20);
  delay(455);
  
  // Wait to score points
  arm.write(90);
  delay(2000);
  
  // Move arm back down
  arm.write(165);
  delay(440);
}

/*---------------- Module Functions -------------------------*/

void lineFollowFWD() {
  int left, right;
  //static int last_state = 0;

  left = isLeftSensorOnTape();
  right = isRightSensorOnTape();
  //forward = isFrontSensorOnTape();
  
  /*if (getSonarFrontDistanceInInches(1) < 3) { //Close to wall
    if (left && right) {
      setLeftMotorSpeed(150);
      setRightMotorSpeed(165);
    }
    else if (!left && !right) {
      //Do nothing
    } else if (!left) {
      setLeftMotorSpeed(200);
      setRightMotorSpeed(150);
      lastTurn = TURN_RIGHT;
      //last_state = 3;
    } else if (!right) {
      setLeftMotorSpeed(150);
      setRightMotorSpeed(200);
      lastTurn = TURN_LEFT;
      //last_state = 4;
    }*/
  if (left && right) {
      setLeftMotorSpeed(210);
      setRightMotorSpeed(205);
      //last_state = 1;
  } else if (!left && !right) {
  } else if (!left) {
    //setLeftMotorSpeed(255);
    setRightMotorSpeed(155);
    //lastTurn = TURN_RIGHT;
    //last_state = 3;
  } else if (!right) {
    setLeftMotorSpeed(155);
    //setRightMotorSpeed(255);
    //lastTurn = TURN_LEFT;
    //last_state = 4;
  }
  delay(35);
}

void lineFollowREV() {
  int left, right;

  left = isLeftSensorOnTape();
  right = isRightSensorOnTape();
  if (left && right) {
    setLeftMotorSpeed(-218);
    setRightMotorSpeed(-205);
  } else if (!left && !right) { // do nothing
  } else if (!left) {
    //setLeftMotorSpeed(-255);
    setRightMotorSpeed(-165);
  } else if (!right) {
    setLeftMotorSpeed(-165);
    //setRightMotorSpeed(-255);
  }
  delay(35);
//  delay(50); // Debug: Does this actually work?
}

void timedDebug(void) {
  static int Time = 0;
  int temp;

  TMRArd_InitTimer(T_DEBUG, T_DEBUG_INTERVAL);

  Serial.println('----------------------------------------------');
  Serial.print(" time:");
  Serial.print(++Time);
  Serial.print(" state:");
  Serial.println(state,DEC);

  Serial.println("TAPE Left/Right/Front:");
  Serial.print(isLeftSensorOnTape());
  Serial.print(" / ");
  Serial.print(isRightSensorOnTape());
  Serial.print(" / ");
  Serial.println(isFrontSensorOnTape());
  Serial.println("LIGHT left/right/front: ");
  Serial.print(analogRead(PIN_LEFT_TAPESENSOR));
  Serial.print(" / ");
  Serial.print(analogRead(PIN_RIGHT_TAPESENSOR));
  Serial.print(" / ");
  Serial.println(analogRead(PIN_FRONT_TAPESENSOR));

  Serial.println("Bumper FrontLeft, FrontRight, BackLeft, BackRight:");
  Serial.print(isLeftFrontBumperPressed());
  Serial.print(" / ");
  Serial.print(isRightFrontBumperPressed());
  Serial.print(" / ");
  Serial.print(isLeftBackBumperPressed());
  Serial.print(" / ");
  Serial.println(isRightBackBumperPressed());

  Serial.print("Sonar inches: ");
  Serial.println(getSonarFrontDistanceInInches(5));
  Serial.println(getSonarLeftDistanceInInches(5));
}


