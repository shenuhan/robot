/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo grab;  // create servo object to control a servo
Servo base;  // create servo object to control a servo
Servo first;  // create servo object to control a servo
Servo second;  // create servo object to control a servo
Servo third;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int maxOpen = 153;
int minClose = 70;

int grabNumber = 9;
int baseNumber = 10;
int firstNumber = 11;
int secondNumber = 12;
int thirdNumber = 13;

float firstLimit = PI/6;
float secondLimit = PI/2;
float thirdLimit = PI/2;

float armLength = 5;

class Position {
  public :
    float x;
    float y;

  public :
    float sqDistance(Position& pos) {
      return pow(pos.x-x,2) + pow(pos.y-y,2);
    } 
};

class AnglePosition {
  public : 
    float alpha;
    float beta;
    float gamma;

  public :
    AnglePosition() {
    }
    AnglePosition(float a,float b,float g) {
      if (a > firstLimit) {
        alpha = firstLimit;
      } else if (a < -firstLimit) {
        alpha = -firstLimit;
      } else {
        alpha = a;
      }
      if (b > secondLimit) {
        beta = secondLimit;
      } else if (b < -secondLimit) {
        beta = -secondLimit;
      } else {
        beta = b;
      }
      if (g > thirdLimit) {
        gamma = thirdLimit;
      } else if (g < -thirdLimit) {
        gamma = -thirdLimit;
      } else {
        gamma = g;
      }
    }

  public :
    void toPosition(Position& pos) {
      pos.x = armLength * sin(alpha) + armLength * sin(alpha+beta) + armLength * sin(alpha+beta+gamma);
      pos.y = armLength + armLength * cos(alpha) + armLength * cos(alpha+beta) + armLength * cos(alpha+beta+gamma);
    }
  public :
    void copy(AnglePosition p) {
      alpha = p.alpha;
      beta = p.beta;
      gamma = p.gamma;
    }

  public :
    void setServos() {
      first.write(90 - alpha * 180 / PI);
      second.write(90 - beta * 180 / PI);
      third.write(90 - gamma * 180 / PI);
  }
};

void setup() {
  grab.attach(grabNumber);  // attaches the servo on pin 9 to the servo object
  base.attach(baseNumber);  // attaches the servo on pin 9 to the servo object
  first.attach(firstNumber);  // attaches the servo on pin 9 to the servo object
  second.attach(secondNumber);  // attaches the servo on pin 9 to the servo object
  third.attach(thirdNumber);  // attaches the servo on pin 9 to the servo object
}

bool oneRun = false;
AnglePosition current;

void loop() {
    // goToTwentySlow();              // tell servo to go to position in variable 'pos'
    if (!oneRun) {
      initialize();
      current.setServos();
      Position there;
      there.x = 10;
      there.y = 5;
      goThere(there, 0.01);
    }
    oneRun = true;
    delay(15);
}

void initialize() {
  current.alpha = 0;
  current.beta = 0;
  current.gamma = 0;
}

void goThere(Position aim, float speeds) {
  Position c;
  current.toPosition(c);
  while(aim.sqDistance(c) > 1) {
    AnglePosition bestNew;
    float bestDistance = 10000000;
    for(float i = -speeds; i < speeds+speeds/10; i += speeds) {
      for(float j = -speeds; j < speeds+speeds/10; j += speeds) {
        for(float k = -speeds; k < speeds+speeds/10; k += speeds) {
          AnglePosition p(current.alpha + i, current.beta + j, current.gamma + k);
          Position pos;
          p.toPosition(pos);
          float d = pos.sqDistance(aim);
          if (d < bestDistance) {
            bestNew.copy(p);
            bestDistance = d;
          }
        }
      }
    }
    current.copy(bestNew);
    current.setServos();
    delay(10);
  }
}

void setCurrent() {
  current.setServos();
}

void goToZero() {
  base.write(90);
  first.write(90);
  second.write(90);
  third.write(90);
}

void goToZeroCinq() {
  base.write(90.5);
  first.write(90.5);
  second.write(90.5);
  third.write(90.5);
}

void goToTen() {
  base.write(80);
  first.write(80);
  second.write(80);
  third.write(80);
}

void openClose() {
  for (int i = maxOpen; i > minClose; i--) { 
    grab.write(i);
    delay(15);
  }
  for (int i = minClose; i < maxOpen; i++) { 
    grab.write(i);
    delay(15);
  }
}

void goToTwentySlow() {
  for (int i = 0; i < 40; i++) { 
    // base.write(90-i);
    first.write(90-i);
    second.write(90-i);
    third.write(90-i);
    delay(15);
  }
  for (int i = 40; i > -40; i--) { 
    // base.write(90-i);
    first.write(90-i);
    second.write(90-i);
    third.write(90-i);
    delay(15);
  }
}

