/**************************************************************
  File:      Main.ino
  Contents:  ME210 Project -- Team && 0x01 
  Authors:   Lars Roemheld, Benjamin Galligan, Ravi Haksar, Dongsuk Shin
  Notes:    Powers a basketball playing Skip-to-my-Loop robot

            Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Timers.h>

/*---------------- Module Defines ---------------------------*/

/* State Machine. Prefix S_, middle abbreviation for functional component */ 
#define S_START             0
#define S_FL_GETFIRSTBALLS  1
#define S_FL_FWDSEARCH      2
#define S_FL_TURNONLINE     3
#define S_GF_FWD            4
#define S_GF_TOLEFT         5
#define S_GF_TORIGHT        6
#define S_GR_REV            7
#define S_GR_TOLEFT         8
#define S_GR_TORIGHT        9
#define S_GR_RELOAD        10
#define S_DUNK             11
unsigned word state = S_START; // Global


/* Timers */
#define T_DEBUG              0
#define T_DEBUG_INTERVAL  1000


/* Which side of the center line are we on? */
#define SIDE_UNKNOWN     0
#define SIDE_LEFT        1
#define SIDE_RIGHT       2
unsigned word start_arena_side = SIDE_UNKNOWN;


/*---------------- Module Function Prototypes ---------------*/

/*---------------- Arduino Main Functions -------------------*/
void setup() {  // setup() function required for Arduino
  Serial.begin(9600);
  Serial.println("Skip to my loop!");

  TMRArd_InitTimer(T_DEBUG, T_DEBUG_INTERVAL;

}

void loop() {  // loop() function required for Arduino
  if (isTimerExpired(T_DEBUG)) timedDebug();
  
  switch (state) {
    case S_START:
      break;
    case S_FL_GETFIRSTBALLS:
      break;
    case S_FL_FWDSEARCH:
      break;
    case S_FL_TURNONLINE:
      break;
    case S_GF_FWD:
      break;
    case S_GF_TOLEFT:
      break;
    case S_GF_TORIGHT:
      break;
    case S_GR_REV:
      break;
    case S_GR_TOLEFT:
      break;
    case S_GR_TORIGHT:
      break;
    case S_GR_RELOAD:
      break;
    case S_DUNK:
      break;
  }  
}


/*------------------ State Machine Functions -------------*/
void setState (unsigned int newState) {
  switch (newState) {
    case S_START:
      break;
    case S_FL_GETFIRSTBALLS:
      break;
    case S_FL_FWDSEARCH:
      break;
    case S_FL_TURNONLINE:
      break;
    case S_GF_FWD:
      break;
    case S_GF_TOLEFT:
      break;
    case S_GF_TORIGHT:
      break;
    case S_GR_REV:
      break;
    case S_GR_TOLEFT:
      break;
    case S_GR_TORIGHT:
      break;
    case S_GR_RELOAD:
      break;
    case S_DUNK:
      break;
    default: 
      Serial.println("Invalid state requested: ");
      Serial.println(newState);
  }
  state = newState;
}





/*---------------- Module Functions -------------------------*/


void timedDebug(void) {
  static int Time = 0;

  TMRArd_InitTimer(T_DEBUG, T_DEBUG_INTERVAL;

  Serial.print(" time:");
  Serial.print(++Time);
  Serial.print(" state:");
  Serial.println(state,DEC);
}








