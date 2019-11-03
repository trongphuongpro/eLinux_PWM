#include "pwm.h"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace BBB;


int main() {
	cout << "17h09" << endl;

	PWM leds(PWM2);

	leds.setPeriod_ms(PWM_A, 2.25);
	usleep(500);
	leds.setPeriod_ms(PWM_B, 2.25);
	usleep(500);

	cout << "led1: " << leds.getPeriod_ms(PWM_A) << "ms "
					 << leds.getFrequency_kHz(PWM_A) << "kHz" << endl;
	cout << "led2: " << leds.getPeriod_ms(PWM_B) << "ms "
					 << leds.getFrequency_kHz(PWM_B) << "kHz" << endl;

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