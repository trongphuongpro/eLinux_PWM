#include "pwm.h"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace BBB;


int main() {

	PWM leds(PWM2);

	leds.setPeriod_us(4.5);

	cout << "led1: " << leds.getPeriod_ms() << "ms "
					 << leds.getFrequency_kHz() << "kHz" << endl;
	cout << "led2: " << leds.getPeriod_ms() << "ms "
					 << leds.getFrequency_kHz() << "kHz" << endl;

	leds.start(PWM_A);
	leds.start(PWM_B);

	int duty1 = 0, duty2 = 0;
	int sign1 = 1, sign2 = 1;

	while (1) {
		
		leds.setDutyCycle(PWM_A, duty1);
		leds.setDutyCycle(PWM_B, duty2);

		duty1 += sign1*10;
		duty2 += sign2*20;

		if (duty1 >= 100 || duty1 <= 0) {
			sign1 = -sign1;
		}

		if (duty2 >= 100 || duty2 <= 0) {
			sign2 = -sign2;
		}
		usleep(100000);
	}
}