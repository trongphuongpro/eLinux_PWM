#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include "pwm.h"

using namespace std;

namespace BBB {


PWM::PWM(const char* chip) {
	this->id = chip[7];
	
	this->chipPath = PWM_PATH + string(chip) + "/";
	this->pathA = this->chipPath + string("pwm-") + this->id + string(":0/");
	this->pathB = this->chipPath + string("pwm-") + this->id + string(":1/");

	this->period = 1;

	reset();
}


PWM::~PWM() {

}


void PWM::reset() {
	stop(PWM_A);
	stop(PWM_B);

	setDutyCycle(PWM_A, 0);
	setDutyCycle(PWM_B, 0);

	setPolarity(PWM_A, ACTIVE_HIGH);
	setPolarity(PWM_B, ACTIVE_HIGH);

	deactivate(PWM_A);
	deactivate(PWM_B);

	activate(PWM_A);
	activate(PWM_B);

	cout << "Resetting done!" << endl;
}


int PWM::activate(PWM_CHANNEL channel) {

	return writeFile(this->chipPath, "export", channel);
}


int PWM::deactivate(PWM_CHANNEL channel) {

	return writeFile(this->chipPath, "unexport", channel);
}


int PWM::writeFile(string path, string filename, string value) {
	ofstream fs;
	fs.open((path+filename).c_str());
	if (!fs.is_open()) {
		perror("PWM: write failed to open file ");
		cout << "file: " << filename << endl;
		return -1;
	}

	fs << value;
	fs.close();
	return 0;
}


int PWM::writeFile(string path, string filename, uint32_t value) {
	return writeFile(path, filename, to_string(value));
}


std::string PWM::readFile(string path, string filename) {
	ifstream fs;
	fs.open((path+filename).c_str());
	if (!fs.is_open()) {
		cout << filename << endl;
		perror("PWM: read failed to open file ");
		return 0;
	}

	string input;
	getline(fs, input);
	fs.close();

	return input;
}


int PWM::setPeriod_ns(uint32_t ns) {
	reset();

	setPeriod_ns(PWM_A, ns);
	setPeriod_ns(PWM_B, ns);

	return 0;
}


int PWM::setPeriod_us(double us) {
	reset();

	setPeriod_us(PWM_A, us);
	setPeriod_us(PWM_B, us);

	return 0;
}


int PWM::setPeriod_ms(double ms) {
	reset();

	setPeriod_ms(PWM_A, ms);
	setPeriod_ms(PWM_B, ms);

	return 0;
}


int PWM::setPeriod_ns(PWM_CHANNEL channel, uint32_t ns) {

	cout << "set period -> " << this->period << endl;

	if (channel == PWM_A) {  
		return writeFile(this->pathA, "period", ns);
	}
	if (channel == PWM_B) {
		return writeFile(this->pathB, "period", ns);
	}
	
	return -1;
}


int PWM::setPeriod_us(PWM_CHANNEL channel, double us) {
	return setPeriod_ns(channel, us * 1000);
}


int PWM::setPeriod_ms(PWM_CHANNEL channel, double ms) {
	return setPeriod_ns(channel, ms * 1000000);
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
	reset();

	setFrequency_Hz(PWM_A, freq);
	setFrequency_Hz(PWM_B, freq);

	return 0;
}


int PWM::setFrequency_kHz(double freq) {
	reset();

	setFrequency_kHz(PWM_A, freq);
	setFrequency_kHz(PWM_B, freq);

	return 0;
}


int PWM::setFrequency_MHz(double freq) {
	reset();

	setFrequency_MHz(PWM_A, freq);
	setFrequency_MHz(PWM_B, freq);

	return 0;
}


int PWM::setFrequency_Hz(PWM_CHANNEL channel, double freq) {
	if (freq == 0) {
		perror("PWM: frequency = 0 is invalid");
		return -1;
	}
	return setPeriod_ns(channel, 1000000000 / freq);
}


int PWM::setFrequency_kHz(PWM_CHANNEL channel, double freq) {
	return setFrequency_Hz(channel, freq * 1000);
}


int PWM::setFrequency_MHz(PWM_CHANNEL channel, double freq) {
	return setFrequency_Hz(channel, freq * 1000000);
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


int PWM::setDutyCycle(PWM_CHANNEL channel, int duty) {
	if (duty > 100 || duty < 0) {
		perror("PWM: duty should be in range [0, 100]");
		return -1;
	}

	if (channel == PWM_A) {
		return writeFile(this->pathA, "duty_cycle", duty * getPeriod_ns() / 100);
	}
	if (channel == PWM_B) {
		return writeFile(this->pathB, "duty_cycle", duty * getPeriod_ns() / 100);
	}
	return -1;
}


double PWM::getDutyCycle(PWM_CHANNEL channel) {
	if (channel == PWM_A) {
		return stof(readFile(this->pathA, "duty_cycle")) / getPeriod_ns();
	}
	if (channel == PWM_B) {
		return stof(readFile(this->pathB, "duty_cycle")) / getPeriod_ns();
	}
	return -1;
}


int PWM::setPolarity(PWM_CHANNEL channel, PWM_POLARITY mode) {
	string path;
	if (channel == PWM_A) {
		path = this->pathA;
	}

	if (channel == PWM_B) {
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


PWM_POLARITY PWM::getPolarity(PWM_CHANNEL channel) {
	string polar;

	if (channel == PWM_A) {
		polar = readFile(this->pathA, "polarity");
	}
	if (channel == PWM_B) {
		polar = readFile(this->pathB, "polarity");
	}

	if (polar == "normal")
		return ACTIVE_HIGH;
	else
		return ACTIVE_LOW;
}


int PWM::start(PWM_CHANNEL channel) {
	if (channel == PWM_A)
		return writeFile(this->pathA, "enable", 1);
	if (channel == PWM_B)
		return writeFile(this->pathB, "enable", 1);
	return -1;
}


int PWM::stop(PWM_CHANNEL channel) {
	if (channel == PWM_A)
		return writeFile(this->pathA, "enable", 0);
	if (channel == PWM_B)
		return writeFile(this->pathB, "enable", 0);
	return -1;
}

} /* namespace BBB */