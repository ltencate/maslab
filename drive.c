// Compile with:
// g++ drive.cpp -o drive  -lmraa
// Controls a motor through a range of speeds using the Cytron motor controller
// Pwm on pin 9, and dir on pin 8.

#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>

#include "mraa.hpp"

int running = 1;

void setMotorSpeed(mraa::Pwm& pwm, mraa::Gpio& dir, double speed) {
  assert(-1.0 <= speed && speed <= 1.0);
  if (speed < 0) {
    dir.write(1);
  }
  else {
    dir.write(0);
  }
  pwm.write(fabs(speed));
}

void sig_handler(int signo)
{
  if (signo == SIGINT) {
    printf("closing spi nicely\n");
    running = 0;
  }
}

int main() {
  // Handle Ctrl-C quit
  signal(SIGINT, sig_handler);

  mraa::Pwm pwm = mraa::Pwm(9);
  pwm.write(0.0);
  pwm.enable(true);
  //assert(pwm != NULL);
  mraa::Pwm pwm1 = mraa::Pwm(10);
  pwm.write(0.0);
  pwm.enable(true);


  mraa::Gpio dir = mraa::Gpio(8);
  //assert(dir != NULL);
  dir.dir(mraa::DIR_OUT);
  dir.write(0);
  
  mraa::Gpio dir1 = mraa::Gpio(7);
  dir.dir(mraa::DIR_OUT);
  dir.write(0);

  double speed = -1.0;
  while (running) {
    std::cout << "Speed: " << speed << std::endl;
    setMotorSpeed(pwm, dir, speed);
    setMotorSpeed(pwm1, dir1, speed);

    speed += 0.1;
    if (speed > 1.0) {
      speed = -1.0;
      // Let the motor spin down
      setMotorSpeed(pwm, dir, 0.0);
      setMotorSpeed(pwm1,dir1,0.0);
      sleep(2.0);
    }
    usleep(100000);
  }
}
