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

int maxOpen = 130;
int minClose = 70;

int grabNumber = 9;
int baseNumber = 10;
int firstNumber = 11;
int secondNumber = 12;
int thirdNumber = 13;

float firstLimit = PI/6;
float secondLimit = PI/2;
float thirdLimit = PI/2;

float baseArmLength = 5;
float firstArmLength = 5;
float secondArmLength = 5;
float thirdArmLength = 7;

class Position {
  public :
    float x;
    float y;

  public :
    float sqDistance(Position& pos) {
      return pow(pos.x-x,2) + pow(pos.y-y,2);
    } 
};

class Zero {
  public : 
    float alpha = 0.2f;
    float beta = 0.1f;
    float gamma = 0.2f;
};

Zero zero = Zero();

class AnglePosition : public Zero {
  public :
    AnglePosition() {
      alpha = 0;
      beta = 0;
      gamma = 0;
    }
    AnglePosition(float a,float b,float g) {
      init(a,b,g);
    }

    void init(float a,float b,float g) {
      if (a-zero.alpha > firstLimit) {
        alpha = firstLimit;
      } else if (a-zero.alpha < -firstLimit) {
        alpha = -firstLimit;
      } else {
        alpha = a;
      }
      if (b-zero.beta > secondLimit) {
        beta = secondLimit;
      } else if (b-zero.beta < -secondLimit) {
        beta = -secondLimit;
      } else {
        beta = b;
      }
      if (g-zero.gamma > thirdLimit) {
        gamma = thirdLimit;
      } else if (g-zero.gamma < -thirdLimit) {
        gamma = -thirdLimit;
      } else {
        gamma = g;
      }
    }

  public :
    void toPosition(Position& pos) {
      pos.x = firstArmLength * sin(alpha - zero.alpha) + secondArmLength * sin(alpha-zero.alpha+beta-zero.beta) + thirdArmLength * sin(alpha-zero.alpha+beta-zero.beta+gamma-zero.gamma);
      pos.y = baseArmLength + firstArmLength * cos(alpha-zero.alpha) + secondArmLength * cos(alpha-zero.alpha+beta-zero.beta) + thirdArmLength * cos(alpha-zero.alpha+beta-zero.beta+gamma-zero.gamma);
    }
  public :
    void copy(AnglePosition p) {
      alpha = p.alpha;
      beta = p.beta;
      gamma = p.gamma;
    }
  public :
    float minus(AnglePosition p) {
      return pow(alpha-p.alpha,2) + pow(beta-p.beta,2) + pow(gamma-p.gamma,2);
    }

  public :
    void setServos() {
      first.write(90 - (alpha-zero.alpha) * 180 / PI);
      second.write(90 - (beta-zero.beta) * 180 / PI);
      third.write(90 - (gamma-zero.beta) * 180 / PI);
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
float angleBase = 0.0f;

void loop() {
    // goToTwentySlow();              // tell servo to go to position in variable 'pos'
    if (!oneRun) {
      initialize();
      current.setServos();
      /*
      pickAndPut(13.5,9,0,14,10,PI/6);
      
      pickAndPut(15,7.5,0,14,10,PI/6);
      pickAndPut(15,7,0,14,10,PI/6);
      */
      AnglePosition zero = AnglePosition(0,0,0);
      pickAndPut(15,7,0,14,10,PI/6);
    }
    oneRun = true;
    delay(15);
}

void fullDance() {
    for (int i = 0; i < 2; i++) {
      dance(2, 80);
      headDance(4, 50);
    }
}

void dance(int nb, int speed) {
  AnglePosition to(PI/6,-PI/4,PI/4);
  for (int i = 0; i < nb ; i++) {
    to.init(PI/6,-PI/3,PI/4);
    moveToFluid(to,PI/4, speed);
    to.init(-PI/6,PI/3,-PI/4);
    moveToFluid(to,-PI/4, speed);
  }
}

void snake(int nb, int speed) {
  AnglePosition to(0,0,0);
  for (int i = 0; i < nb ; i++) {
    for (int j = 0; j < 100; j++) {
      to.init(i*cos(j * PI/50),-j * PI/50, 0);
      moveToFluid(to,PI/4, speed);
      to.init(-PI/6,PI/3,-PI/4);
      moveToFluid(to,-PI/4, speed);
    }
  }
}

void headDance(int nb, int speed) {
  AnglePosition to = AnglePosition(0,0,0);
  moveToFluid(to,0, speed);
  for (int i = 0; i < nb ; i++) {
    to.init(PI/10,-PI/5,PI/1.9);
    moveToFluid(to, PI/10, speed);
    to.init(0,0,0);
    moveToFluid(to, -PI/10, speed);
  }
}

void pickAndPut(float fromx, float fromy, float frombase, float tox, float toy, float tobase) {
  AnglePosition zero = AnglePosition(0,0,0);
  moveTo(zero, angleBase);
  moveTo(zero, frombase);
  Position there;
  grabOpen();
  there.x = fromx;
  there.y = fromy;
  goThere(there, 0.01);
  grabClose();
  there.x = tox;
  there.y = toy;
  moveTo(zero, angleBase);
  moveTo(zero, tobase);
  goThere(there, 0.01);
  grabOpen();
  moveTo(zero, angleBase);
}

void initialize() {
  current.alpha = 0;
  current.beta = 0;
  current.gamma = 0;
  angleBase = 0;
}

void moveToFluid(AnglePosition p, float baseTo, int speed) {
  float stepalpha = (p.alpha-current.alpha)/speed;
  float stepbeta = (p.beta-current.beta)/speed;
  float stepgamma = (p.gamma-current.gamma)/speed;
  float stepbase = (baseTo-angleBase)/speed;
  while(current.minus(p) > 0.00001 || abs(baseTo - angleBase) > 0.0001) {
    current.alpha = abs(p.alpha-current.alpha) <= abs(stepalpha) ? p.alpha : current.alpha + stepalpha;
    current.beta = abs(p.beta-current.beta) <= abs(stepbeta) ? p.beta : current.beta + stepbeta;
    current.gamma = abs(p.gamma-current.gamma) <= abs(stepgamma) ? p.gamma : current.gamma + stepgamma;
    angleBase = abs(baseTo-angleBase) <= abs(stepbase) ? baseTo : angleBase + stepbase;
    current.setServos();
    base.write(90 - angleBase * 180 / PI);
    delay(5);
  }
  current.alpha = p.alpha;
  current.beta = p.beta;
  current.gamma = p.gamma;
  angleBase = baseTo;
  current.setServos();
  base.write(90 - angleBase * 180 / PI);
}

void moveTo(AnglePosition p, float baseTo) {
  while(current.minus(p) > 0.00001 || pow(baseTo - angleBase, 2) > 0.00001) {
    current.alpha = pow(p.alpha-current.alpha,2) < 0.0001 ? p.alpha : (p.alpha > current.alpha ? current.alpha + 0.01 : current.alpha - 0.01);
    current.beta = pow(p.beta-current.beta,2) < 0.0001 ? p.beta : (p.beta > current.beta ? current.beta + 0.01 : current.beta - 0.01);
    current.gamma = pow(p.gamma-current.gamma,2) < 0.0001 ? p.gamma : (p.gamma > current.gamma ? current.gamma + 0.01 : current.gamma - 0.01);
    angleBase = pow(baseTo-angleBase,2) < 0.0001 ? baseTo : (baseTo > angleBase ? angleBase + 0.01 : angleBase - 0.01);
    current.setServos();
    base.write(90 - angleBase * 180 / PI);
    delay(10);
  }
  current.alpha = p.alpha;
  current.beta = p.beta;
  current.gamma = p.gamma;
  angleBase = baseTo;
  base.write(90 - angleBase * 180 / PI);
}

void goToZero() {
  base.write(90);
  first.write(90);
  second.write(90);
  third.write(90);
}

void goThere(Position aim, float speeds) {
  Position c;
  current.toPosition(c);
  while(aim.sqDistance(c) > 0.1) {
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
    Position best;
    bestNew.toPosition(best);
    Position curr;
    current.toPosition(curr);
    if (best.sqDistance(curr) < 0.001) return;

    current.copy(bestNew);
    current.setServos();
    delay(10);
  }
}

void setCurrent() {
  current.setServos();
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
  grabOpen();
  grabClose();
}

void grabClose() {
  for (int i = maxOpen; i > minClose; i--) { 
    grab.write(i);
    delay(15);
  }
}

void grabOpen() {
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

