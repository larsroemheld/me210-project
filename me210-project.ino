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
#define S_FL_FINDLINE       2
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
unsigned char state = S_START; // Global;


/* Timers */
#define T_DEBUG              0
#define T_DEBUG_INTERVAL  1000
#define T_GAME               1
#define T_GAME_LENGTH     120*1000

/* Which side of the center line are we on? */
#define SIDE_UNKNOWN     0
#define SIDE_LEFT        1
#define SIDE_RIGHT       2
unsigned char startArenaSide = SIDE_UNKNOWN;

/* Sensor parameters and thresholds */
#define HALF_FIELD_WIDTH 24 // Half the field width, used as threshold for ultrasonic side sensing on startup
#define SONAR_START_ACCURACY_PINGS 20 // How many sonar pings we send off and average over when we start the game
#define SONAR_LOOP_ACCURACY_PINGS   5 // How many sonar pings we send off and average over when periodically checking distance (blocking!)

int temp_debug = true;

/*---------------- Module Function Prototypes ---------------*/

/*---------------- Arduino Main Functions -------------------*/
void setup() {  // setup() function required for Arduino
  Serial.begin(9600);
  Serial.println("Skip to my loop!");

  initBumpers();
  initMotors();
  initTapeSensors();
  initSonar();

  TMRArd_InitTimer(T_DEBUG, T_DEBUG_INTERVAL);
  TMRArd_InitTimer(T_GAME, T_GAME_LENGTH);
  setState(state);  
}

void loop() {  // loop() function required for Arduino
  if (TMRArd_IsTimerExpired(T_DEBUG)) timedDebug();
  // Break after game
  // if (TMRArd_IsTimerExpired(T_GAME)) while (true) delay(100);

  //DEBUG
  // delay(50);
  // return;


  if(Serial.available() || temp_debug == true) {
    temp_debug = false;
    Serial.println("STOPPED");
    setMotorSpeed(0);
    unsigned char c;
    c = Serial.read();
    while (!Serial.available()) { delay(100); }
    c = Serial.read();
    Serial.println("CONTINUED");
    setState(S_START);
  }


  switch (state) {
    case S_START:
      int startDistanceToFront;
      // Send SONAR_START_ACCURACY_PINGS pings, average distance
      startDistanceToFront = getSonarFrontDistanceInInches(SONAR_START_ACCURACY_PINGS);
      
      startArenaSide = (startDistanceToFront < HALF_FIELD_WIDTH) ? SIDE_LEFT : SIDE_RIGHT;

      setState(S_FL_FINDLINE);
      break;
    case S_FL_FINDLINE:
      /* If we think we are in the left side (hence going towards the right) and end up half-way between center line 
         and the right side, we were on the right side inititally (or we missed the line). In that case, reverse!   */
      // NOTE: this is somewhat blocking code, keep in mind for debug!
      int curDistanceToFront;
      curDistanceToFront = getSonarFrontDistanceInInches(SONAR_LOOP_ACCURACY_PINGS);
      // if (startArenaSide == SIDE_LEFT && curDistanceToFront >= HALF_FIELD_WIDTH * 3 / 2) reverseMotors();
      // if (startArenaSide == SIDE_RIGHT && curDistanceToFront <= HALF_FIELD_WIDTH * 3 / 2) reverseMotors();      


      Serial.println(isLeftSensorOnTape());
      Serial.println(isRightSensorOnTape());

      if (isLeftSensorOnTape() && isRightSensorOnTape()) setState(S_FL_TURNONLINE);
      break;
    case S_FL_TURNONLINE:
      if (isFrontSensorOnTape()) setState(S_GR_RELOAD);
      break;
    case S_GF_FWD:
      if (isAnyFrontBumperPressed()) setState(S_DUNK);
      if (!isLeftSensorOnTape()) setState(S_GF_TORIGHT);
      if (!isRightSensorOnTape()) setState(S_GF_TOLEFT);
      break;
    case S_GF_TOLEFT:
      if (isAnyFrontBumperPressed()) setState(S_DUNK);
      if (areAllSensorsOnTape()) setState(S_GF_FWD);
      break;
    case S_GF_TORIGHT:
      if (isAnyFrontBumperPressed()) setState(S_DUNK);
      if (areAllSensorsOnTape()) setState(S_GF_FWD);
      break;
    case S_GR_REV:
      if (isAnyBackBumperPressed()) setState(S_GR_RELOAD);
      if (!isLeftSensorOnTape()) setState(S_GR_TORIGHT);
      if (!isRightSensorOnTape()) setState(S_GR_TOLEFT);
      break;
    case S_GR_TOLEFT:
      if (isAnyBackBumperPressed()) setState(S_GR_RELOAD);
      if (areAllSensorsOnTape()) setState(S_GR_REV);
      break;
    case S_GR_TORIGHT:
      if (isAnyBackBumperPressed()) setState(S_GR_RELOAD);
      if (areAllSensorsOnTape()) setState(S_GR_REV);
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
    case S_FL_FINDLINE:
      arenaTurnSign = (startArenaSide == SIDE_LEFT) ? 1 : -1;

      if (startArenaSide == SIDE_LEFT) {
        // Manually fine-tuned values
        setLeftMotorSpeed( 145);
        setRightMotorSpeed(155);
      } else {
        // Manually fine-tuned values
        setLeftMotorSpeed( -145);
        setRightMotorSpeed(-155);
      }
      break;
    case S_FL_TURNONLINE:
      setLeftMotorSpeed(  135);
      setRightMotorSpeed(-145);
      break;
    case S_GF_FWD:
      setMotorSpeed(215);
      break;
    case S_GF_TOLEFT:
      setLeftMotorSpeed( 165);
      setRightMotorSpeed(215);
      break;
    case S_GF_TORIGHT:
      setLeftMotorSpeed( 215);
      setRightMotorSpeed(165);
      break;
      
    case S_GR_REV:
      setMotorSpeed(-215);
      break;
    case S_GR_TOLEFT:
      setLeftMotorSpeed( -165);
      setRightMotorSpeed(-215);
      break;
    case S_GR_TORIGHT:
      setLeftMotorSpeed( -215);
      setRightMotorSpeed(-165);
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
    Serial.print("Request ");
    Serial.print(iBall);

    while (!isAnyBackBumperPressed()) {
      setMotorSpeed(-MAX_MOTOR_SPEED);
      delay(20);
    }
    Serial.print("Bumper contact");

    // Wait for a moment to show off pressed state
    setMotorSpeed(0);
    delay(500);

    // Release the bumper
    Serial.print("Release!");
    setMotorSpeed(MAX_MOTOR_SPEED);
    while (isAnyBackBumperPressed()) {
      delay(20);
    }
    Serial.print("Released!");

    // Wait for ball
    setMotorSpeed(0);
    delay(2000);   
  }
}

void dunkBalls() {
  Serial.println("DUNK!");
}

/*---------------- Module Functions -------------------------*/


void timedDebug(void) {
  static int Time = 0;
  int temp;
  return;
  
  TMRArd_InitTimer(T_DEBUG, T_DEBUG_INTERVAL);

  Serial.print(" time:");
  Serial.print(++Time);
  Serial.print(" state:");
  Serial.println(state,DEC);

  Serial.println("Tape Left/Right/Front:");
  Serial.println(isLeftSensorOnTape());
  Serial.println(isRightSensorOnTape());
  Serial.println(isFrontSensorOnTape());

  Serial.println("Bumper FrontLeft, FrontRight, BackLeft, BackRight:");
  Serial.println(isLeftFrontBumperPressed());
  Serial.println(isRightFrontBumperPressed());
  Serial.println(isLeftBackBumperPressed());
  Serial.println(isRightBackBumperPressed());

  Serial.println("Sonar inches:");
  Serial.println(getSonarFrontDistanceInInches(5));

  // Serial.print(" left/right: ");
  // Serial.print(analogRead(PIN_LEFT_TAPESENSOR));
  // Serial.print(" / ");
  // Serial.println(analogRead(PIN_RIGHT_TAPESENSOR));

 // Serial.print(" sonar:");
 // Serial.println(debugSonar());

}








