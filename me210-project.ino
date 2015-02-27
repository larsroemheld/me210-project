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
// Find line
#define S_FL_GETFIRSTBALLS  1
#define S_FL_FWDSEARCH      2
#define S_FL_TURNONLINE     3
// Go Forward
#define S_GF_FWD            4
#define S_GF_TOLEFT         5
#define S_GF_TORIGHT        6
// Go Reverse
#define S_GR_REV            7
#define S_GR_TOLEFT         8
#define S_GR_TORIGHT        9
#define S_GR_RELOAD        10
#define S_DUNK             11

// NEVER WRITE TO THIS VARIABLE DIRECTLY, ALWAYS USE setState()!
unsigned char state = S_GF_FWD; // Global;


/* Timers */
#define T_DEBUG              0
#define T_DEBUG_INTERVAL  1000
#define T_GAME
#define T_GAME_LENGTH     120*1000

/* Which side of the center line are we on? */
#define SIDE_UNKNOWN     0
#define SIDE_LEFT        1
#define SIDE_RIGHT       2
unsigned char startArenaSide = SIDE_UNKNOWN;

/* Sensor parameters and thresholds */
#define HALF_FIELD_WIDTH 24 // Half the field width, used as threshold for ultrasonic side sensing on startup
#define SONAR_START_ACCURACY_PINGS 20 // How many sonar pings we send off and average over when we start the game

int temp_debug = true;

/*---------------- Module Function Prototypes ---------------*/

/*---------------- Arduino Main Functions -------------------*/
void setup() {  // setup() function required for Arduino
  Serial.begin(9600);
  Serial.println("Skip to my loop!");

  initBumpers();
  initMotors();
  initTapeSensors();
//  initSonar();

  TMRArd_InitTimer(T_DEBUG, T_DEBUG_INTERVAL);
  setState(state);  
}

void loop() {  // loop() function required for Arduino
  if (TMRArd_IsTimerExpired(T_DEBUG)) timedDebug();
  
  if(Serial.available() || temp_debug == true) {
    temp_debug = false;
    Serial.println("STOPPED");
    setMotorSpeed(0);
    unsigned char c;
    c = Serial.read();
    while (!Serial.available()) { delay(100); }
    c = Serial.read();
    Serial.println("CONTINUED");
    setState(S_GF_FWD);
  }


  switch (state) {
    case S_START:
      int startDistanceToLeft;
      // Send SONAR_START_ACCURACY_PINGS pings, average distance
      startDistanceToLeft = getSonarLeftDistanceInInches(SONAR_START_ACCURACY_PINGS);
      
      // TODO: test what if we happen to start right on the line
      startArenaSide = (startDistanceToLeft < HALF_FIELD_WIDTH) ? SIDE_LEFT : SIDE_RIGHT;

      // DEBUG
      startArenaSide = SIDE_RIGHT;
      setState(S_FL_GETFIRSTBALLS);
      break;
    case S_FL_GETFIRSTBALLS:
      Serial.println("WARNING: State machine was in S_FL_GETFIRSTBALLS, but this should always be a transient state!");
      break;
    case S_FL_FWDSEARCH:
      if (areBothSensorsOnTape()) setState(S_FL_TURNONLINE);
      break;
    case S_FL_TURNONLINE:
      Serial.println("WARNING: State machine was in S_FL_TURNONLINE, but this should always be a transient state!");
      break;
    case S_GF_FWD:
      if (isAnyFrontBumperPressed()) setState(S_DUNK);
      if (!isLeftSensorOnTape()) setState(S_GF_TORIGHT);
      if (!isRightSensorOnTape()) setState(S_GF_TOLEFT);
      break;
    case S_GF_TOLEFT:
      if (isAnyFrontBumperPressed()) setState(S_DUNK);
      if (areBothSensorsOnTape()) setState(S_GF_FWD);
      break;
    case S_GF_TORIGHT:
      if (isAnyFrontBumperPressed()) setState(S_DUNK);
      if (areBothSensorsOnTape()) setState(S_GF_FWD);
      break;
    case S_GR_REV:
      if (isAnyBackBumperPressed()) setState(S_GR_RELOAD);
      if (!isLeftSensorOnTape()) setState(S_GR_TORIGHT);
      if (!isRightSensorOnTape()) setState(S_GR_TOLEFT);
      break;
    case S_GR_TOLEFT:
      if (isAnyBackBumperPressed()) setState(S_GR_RELOAD);
      if (areBothSensorsOnTape()) setState(S_GR_REV);
      break;
    case S_GR_TORIGHT:
      if (isAnyBackBumperPressed()) setState(S_GR_RELOAD);
      if (areBothSensorsOnTape()) setState(S_GR_REV);
      break;
    case S_GR_RELOAD:
      break;
    case S_DUNK:
      break;
  }  
}


/*------------------ State Machine Functions -------------*/
void setState (unsigned int newState) {
  state = newState;
  Serial.print("State change: ");
  Serial.println(newState);

  signed int arenaTurnSign;
  
  switch (newState) {
    case S_START:
      break;
    case S_FL_GETFIRSTBALLS:
      requestBalls(3);
      
      arenaTurnSign = (startArenaSide == SIDE_LEFT) ? 1 : -1;
      
      turnRightInPlace(200, arenaTurnSign * MAX_MOTOR_SPEED / 2);

      setState(S_FL_FWDSEARCH);
      break;
    case S_FL_FWDSEARCH:
      setMotorSpeed(MAX_MOTOR_SPEED);
      break;
    case S_FL_TURNONLINE:
      arenaTurnSign = (startArenaSide == SIDE_LEFT) ? 1 : -1;
      
      turnRightInPlace(500, - arenaTurnSign * MAX_MOTOR_SPEED / 2);

      setState(S_GF_FWD);
      break;
    case S_GF_FWD:
      setMotorSpeed(MAX_MOTOR_SPEED);
      break;
    case S_GF_TOLEFT:
      setLeftMotorSpeed(MAX_MOTOR_SPEED * 3 / 5);
      setRightMotorSpeed(MAX_MOTOR_SPEED);
      break;
    case S_GF_TORIGHT:
      setLeftMotorSpeed(MAX_MOTOR_SPEED);
      setRightMotorSpeed(MAX_MOTOR_SPEED * 3 / 5);
      break;
      
    case S_GR_REV:
      setMotorSpeed(-MAX_MOTOR_SPEED);
      break;
    case S_GR_TOLEFT:
      setLeftMotorSpeed(- MAX_MOTOR_SPEED * 3 / 5);
      setRightMotorSpeed(- MAX_MOTOR_SPEED);
      break;
    case S_GR_TORIGHT:
      setLeftMotorSpeed(- MAX_MOTOR_SPEED);
      setRightMotorSpeed(- MAX_MOTOR_SPEED * 3 / 5);
      break;
    case S_GR_RELOAD:
      requestBalls(3);
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

  for (int iBall = 1; iBall <= numBalls; iBall++) {
    // Make sure we hit the bumper
    while (!isAnyBackBumperPressed()) {
      setMotorSpeed(-MAX_MOTOR_SPEED);
      delay(20);
    }

    // Push a little further, then Wait for a moment to show off pressed state
    delay(100);
    setMotorSpeed(0);
    delay(500);

    // Release the bumper
    while (isAnyBackBumperPressed()) {
      setMotorSpeed(MAX_MOTOR_SPEED);

      delay(20);
    }

    // Wait for ball
    setMotorSpeed(0);
    delay(2000);   
  }
}

// Will turn on the spot. Will turn right for positive turnSpeed, left for negative turnSpeed. The parameter gives the duration and speed of the turn.
void turnRightInPlace(unsigned int milliSecs, signed int turnSpeed) {
  signed int oldLeftSpeed, oldRightSpeed;

  oldLeftSpeed = setLeftMotorSpeed(   -turnSpeed);
  oldRightSpeed = setRightMotorSpeed(  turnSpeed);

  delay(milliSecs);

  setLeftMotorSpeed(oldLeftSpeed);
  setRightMotorSpeed(oldRightSpeed);
}

void dunkBalls() {
  Serial.println("DUNK!");
}

/*---------------- Module Functions -------------------------*/


void timedDebug(void) {
  static int Time = 0;
  int temp;
  
  TMRArd_InitTimer(T_DEBUG, T_DEBUG_INTERVAL);

  Serial.print(" time:");
  Serial.print(++Time);
  Serial.print(" state:");
  Serial.println(state,DEC);

 Serial.print(" left/right:");
 Serial.println(analogRead(PIN_LEFT_TAPESENSOR));
 Serial.println(analogRead(PIN_RIGHT_TAPESENSOR));

 // Serial.print(" sonar:");
 // Serial.println(debugSonar());

}








