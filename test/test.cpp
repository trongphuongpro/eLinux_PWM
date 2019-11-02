#include "pwm.h"
#include <iostream>


using namespace std;
using namespace BBB;


int main() {
	PWM led(PWM2B);
	led.setPeriod_ms(1000);
	led.setDutyCycle(0.5);
	led.start();

	while (1) {

	}
}