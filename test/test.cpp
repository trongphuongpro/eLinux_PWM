#include "pwm.h"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace BBB;


int main() {
	PWM led1(PWM2B);
	PWM led2(PWM2A);

	led1.setPeriod_ms(0.25);
	led1.setDutyCycle(0);

	led2.setPeriod_ms(0.25);
	led2.setDutyCycle(0);

	cout << "led1: " << led1.getPeriod_ms() << "ms "
					 << led1.getFrequency_kHz() << "kHz" << endl;
	cout << "led2: " << led2.getPeriod_ms() << "ms "
					 << led2.getFrequency_kHz() << "kHz" << endl;

	led1.start();
	led2.start();

	int duty1 = 0, duty2 = 0;
	int sign1 = 1, sign2 = 1;

	while (1) {
		
		led1.setDutyCycle(duty1);
		led2.setDutyCycle(duty2);

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