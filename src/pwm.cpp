#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cstdlib>
#include "pwm.h"

using namespace std;

namespace BBB {


PWM::PWM(const char* chip) {
	this->id = chip[7];
	
	this->chipPath = PWM_PATH + string(chip) + "/";
	this->pathA = this->chipPath + "pwm-" + this->id + ":0/";
	this->pathB = this->chipPath + "pwm-" + this->id + ":1/";
}


PWM::~PWM() {

}


int PWM::reset() {

	/* MUST stop PWM channels before unexport */
	stop(A);
	stop(B);
	/* end */

	setDutyCycle(A, 0);
	setDutyCycle(B, 0);

	setPolarity(A, ACTIVE_HIGH);
	setPolarity(B, ACTIVE_HIGH);

	deactivate(A);
	deactivate(B);

	activate(A);
	activate(B);

	int ret = system("sudo systemctl restart udev.service");

	if (ret == 0) {
		cout << "Reset successful" << endl;
	}
	else {
		cout << "Reset failed" << endl;
	}

	return ret;
}


int PWM::activate(CHANNEL channel) {

	return writeFile(this->chipPath, "export", channel);
}


int PWM::deactivate(CHANNEL channel) {

	return writeFile(this->chipPath, "unexport", channel);
}


int PWM::writeFile(string path, string filename, string value) {
	ofstream fs;
	fs.open((path+filename).c_str());
	if (!fs.is_open()) {
		perror(("PWM: write failed to open file '" + string(filename) + "'").c_str());
		return -1;
	}

	fs << value;
	fs.close();
	return 0;
}


int PWM::writeFile(string path, string filename, uint32_t value) {
	return writeFile(path, filename, to_string(value));
}


string PWM::readFile(string path, string filename) {
	ifstream fs;
	fs.open((path+filename).c_str());
	if (!fs.is_open()) {
		perror(("PWM: read failed to open file '" + string(filename) + "'").c_str());
		return 0;
	}

	string input;
	getline(fs, input);
	fs.close();

	return input;
}


int PWM::setPeriod_ns(uint32_t ns) {
	if (reset() != 0) {
		return -1;
	}

	if ((writeFile(this->pathA, "period", ns) == 0) &&
		(writeFile(this->pathB, "period", ns) == 0)) {
		return 0;
	}

	return -1;
}


int PWM::setPeriod_us(double us) {
	return setPeriod_ns(us * 1000);
}


int PWM::setPeriod_ms(double ms) {
	return setPeriod_ns(ms * 1000000);
}


uint32_t PWM::getPeriod_ns() {

	return stoi(readFile(this->pathA, "period"));
}


double PWM::getPeriod_us() {
	return getPeriod_ns() / 1000.0;
}


double PWM::getPeriod_ms() {
	return getPeriod_ns() / 1000000.0;
}


int PWM::setFrequency_Hz(double freq) {

	if (freq == 0) {
		perror("PWM: frequency = 0 is invalid");
		return -1;
	}

	return setPeriod_ns(1000000000 / freq);
}


int PWM::setFrequency_kHz(double freq) {
	return setFrequency_Hz(freq * 1000);
}


int PWM::setFrequency_MHz(double freq) {
	return setFrequency_Hz(freq * 1000000);
}


double PWM::getFrequency_Hz() {
	return 1000000000. / getPeriod_ns();
}


double PWM::getFrequency_kHz() {
	return getFrequency_Hz() / 1000;
}


double PWM::getFrequency_MHz() {
	return getFrequency_Hz() / 1000000;
}


int PWM::setDutyCycle(CHANNEL channel, int duty) {
	if (duty > 100 || duty < 0) {
		perror("PWM: duty should be in range [0, 100]");
		return -1;
	}

	if (channel == A) {
		return writeFile(this->pathA, "duty_cycle", duty * getPeriod_ns() / 100);
	}
	if (channel == B) {
		return writeFile(this->pathB, "duty_cycle", duty * getPeriod_ns() / 100);
	}
	return -1;
}


double PWM::getDutyCycle(CHANNEL channel) {
	if (channel == A) {
		return stof(readFile(this->pathA, "duty_cycle")) / getPeriod_ns();
	}
	if (channel == B) {
		return stof(readFile(this->pathB, "duty_cycle")) / getPeriod_ns();
	}
	return -1;
}


int PWM::setPolarity(CHANNEL channel, POLARITY mode) {
	string path;
	if (channel == A) {
		path = this->pathA;
	}

	if (channel == B) {
		path = this->pathB;
	}

	if (mode == ACTIVE_LOW) {
		return writeFile(path, "polarity", "inversed");
	}
	else if (mode == ACTIVE_HIGH) {
		return writeFile(path, "polarity", "normal");
	}
	else {
		perror("PWM: no such Polarity mode");
		return -1;
	}
}


PWM::POLARITY PWM::getPolarity(CHANNEL channel) {
	string polar;

	if (channel == A) {
		polar = readFile(this->pathA, "polarity");
	}
	if (channel == B) {
		polar = readFile(this->pathB, "polarity");
	}

	if (polar == "normal")
		return ACTIVE_HIGH;
	else
		return ACTIVE_LOW;
}


int PWM::start(CHANNEL channel) {
	if (channel == A)
		return writeFile(this->pathA, "enable", 1);
	if (channel == B)
		return writeFile(this->pathB, "enable", 1);
	return -1;
}


int PWM::stop(CHANNEL channel) {
	if (channel == A)
		return writeFile(this->pathA, "enable", 0);
	if (channel == B)
		return writeFile(this->pathB, "enable", 0);
	return -1;
}

} /* namespace BBB */