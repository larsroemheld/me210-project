// Define motor pins
#define motorEnA 10
#define motorDirA 12
#define motorEnB 11
#define motorDirB 13

// Define states
#define initial 0
#define go_line 1
#define turn_line 2
#define go_bump 3
#define get_balls 4
#define go_hoop 5
#define dunk 6
#define go_back 7

// Define IR constants
// Sensor 1 is paired with motor A
// Sensor 2 is paired with motor B
#define IRs2 A0
#define IRs1 A1
#define IRs3 A2
#define udark_lim 100

// Define switch pins
#define SW1 2
#define SW2 3
#define SW3 4
#define SW4 5

// Define sonar pins
#define Echo 8
#define Trig 9

// Necessary variables and constants
#define forw 0
#define rev 1
#define dist_lim 20
#define num_hits 2

unsigned char mAflag = 0;
unsigned char mBflag = 0;

float inches;
float duration;

unsigned char state;
unsigned char line_dir;
unsigned char no_move = 0;
unsigned num_balls = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Start main code!");
  
  // Set motor pins as output
  pinMode(motorEnA, OUTPUT);
  pinMode(motorDirA, OUTPUT);
  pinMode(motorEnB, OUTPUT);
  pinMode(motorDirB, OUTPUT);
  
  // Set initial motor direction as forward
  digitalWrite(motorDirA, HIGH);
  digitalWrite(motorDirB, LOW);
  
  // Initial state is "initilization"
  state = initial;
}

void loop()
{
  switch(state) {
    case(initial):
    {
      
      // Calculate which side of the line the robot is on
      digitalWrite(Trig, LOW);
      delayMicroseconds(2);
      digitalWrite(Trig, HIGH);
      delayMicroseconds(5);
      digitalWrite(Trig, LOW);
      
      duration = pulseIn(Echo, HIGH, 10);
      inches = usToInches(duration);
      
      // Set the robot direction based on distance
      if (inches > dist_lim)
      {
        line_dir = forw;
        digitalWrite(motorDirA, HIGH);
        digitalWrite(motorDirB, LOW);
        
      }
      else
      {
        line_dir = rev;
        digitalWrite(motorDirA, LOW);
        digitalWrite(motorDirB, HIGH);
      }
      
      // If the robot is on the line, skip finding the line
      unsigned int valS1 = analogRead(IRs1);
      unsigned int valS2 = analogRead(IRs2);
      if (valS1 > udark_lim or valS2 > udark_lim)
      {
        state = turn_line;
      }
      else
      {
        state = go_line;
        analogWrite(motorEnA, 145);
        analogWrite(motorEnB, 155);
      }

      break;
    }
    case(go_line):
    {
      // Check sensor values
      unsigned int valS1 = analogRead(IRs1);
      unsigned int valS2 = analogRead(IRs2);
      
      // If the sensor reads black, stop the motor
      if (valS1 > udark_lim)
      {
        analogWrite(motorEnA, 0);
        mAflag = 1;
      }
      if (valS2 > udark_lim)
      {
        analogWrite(motorEnB, 0);
        mBflag = 1;
      }
     
      // if both stop, we've reached the line
      if (mAflag && mBflag)
      {
        // Reset direction to forward
        digitalWrite(motorDirA, HIGH);
        digitalWrite(motorDirB, LOW);
        state = turn_line;
      }
    
      break;  
    }
    case(turn_line):
    {
      // Read front sensor and change motor B direction
      unsigned int valS3 = analogRead(IRs3);
      digitalWrite(motorDirB, HIGH);
      
      // Turn until line found
      while (valS3 < udark_lim)
      {
        analogWrite(motorEnA, 135);
        analogWrite(motorEnB, 145);
        valS3 = analogRead(IRs3);
      }      
      
      // Stop motors, reset direction
      analogWrite(motorEnA, 0);
      analogWrite(motorEnB, 0);
      digitalWrite(motorDirB, LOW);
      
      // Change state, and reverse motor directions
      state = go_bump;
      digitalWrite(motorDirA, LOW);
      digitalWrite(motorDirB, HIGH);
      
      break;
    }
    case(go_bump):
    {
      // Line follow until bumpers are hit
      unsigned int valS1 = analogRead(IRs1);
      unsigned int valS2 = analogRead(IRs2);
      unsigned char valSW1 = digitalRead(SW1);
      unsigned char valSW2 = digitalRead(SW2);
      
      if (valS1 < udark_lim && valSW2 == 0)
      {
        analogWrite(motorEnB, 165);
      }
      else if (valS2 < udark_lim && valSW1 == 0)
      {
        analogWrite(motorEnA, 165);
      }
      else if (valSW1 == 0 && valSW2 == 0)
      {
        analogWrite(motorEnA, 215);
        analogWrite(motorEnB, 215);
      }
      else
      {
        analogWrite(motorEnA, 0);
        analogWrite(motorEnB, 0);
        num_balls = num_balls + 1;
        state = get_balls;
      }      
      
      break;
    }    
    case(get_balls):
    {
      // Should we check if the switches are hit?
      if (num_balls < 3)
      {
        delay(3000);
        // change direction
        digitalWrite(motorDirA, HIGH);
        digitalWrite(motorDirB, LOW);
        
        // go forward
        analogWrite(motorEnA, 215);
        analogWrite(motorEnB, 215);
        delay(500);
        analogWrite(motorEnA, 0);
        analogWrite(motorEnB, 0);
        
        // change direction
        digitalWrite(motorDirA, LOW);
        digitalWrite(motorDirB, HIGH);
        
        // go back
        analogWrite(motorEnA, 215);
        analogWrite(motorEnB, 215);
        delay(500);
        analogWrite(motorEnA, 0);
        analogWrite(motorEnB, 0);
        
        // increment counter
        num_balls = num_balls + 1;
      }
      else
      {
        digitalWrite(motorDirA, HIGH);
        digitalWrite(motorDirB, LOW);
        state = go_hoop;
      }
      
      break;
    }
    case(go_hoop):
    {
      // Line follow until bumpers are hit
      unsigned int valS1 = analogRead(IRs1);
      unsigned int valS2 = analogRead(IRs2);
      unsigned char valSW3 = digitalRead(SW3);
      unsigned char valSW4 = digitalRead(SW4);
      
      if (valS1 < udark_lim && valSW3 == 0)
      {
        analogWrite(motorEnB, 165);
      }
      else if (valS2 < udark_lim && valSW4 == 0)
      {
        analogWrite(motorEnA, 165);
      }
      else if (valSW3 == 0 && valSW4 == 0)
      {
        analogWrite(motorEnA, 215);
        analogWrite(motorEnB, 215);
      }
      else
      {
        analogWrite(motorEnA, 0);
        analogWrite(motorEnB, 0);
        state = dunk;
      }  
      
      break;
    }
    case(dunk):
    {
      // Write servo shit here
      delay(5000);
      state = go_bump;
      digitalWrite(motorDirA, LOW);
      digitalWrite(motorDirB, HIGH);
      num_balls = 0;
      break;
    }
    
    default:
    {
      // Worst case - turn both motors off
      analogWrite(motorEnA, 0);
      analogWrite(motorEnB, 0);
      break;
    }
  }
  
}

float usToInches(float us)
{
  return us / 73.746 / 2;
}
  