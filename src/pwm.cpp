#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include "pwm.h"

using namespace std;

namespace BBB {

PWM::PWM(string channel) {
	this->channel = channel;
	this->path = PWM_PATH + channel + "/";
}


PWM::~PWM() {

}


string PWM::getChannel() {
	if (this->channel == "pwm-1:0") {
		return "PWM0A";
	}
	if (this->channel == "pwm-1:1") {
		return "PWM0B";
	}
	if (this->channel == "pwm-4:0") {
		return "PWM1A";
	}
	if (this->channel == "pwm-4:1") {
		return "PWM1B";
	}
	if (this->channel == "pwm-7:0") {
		return "PWM2A";
	}
	if (this->channel == "pwm-7:1") {
		return "PWM2B";
	}
	return 0;
}


int PWM::writeFile(string path, string filename, string value) {
	ofstream fs;
	fs.open((path+filename).c_str());
	if (!fs.is_open()) {
		perror("PWM: write failed to open file ");
		return -1;
	}

	fs << value;
	fs.close();
	return 0;
}


int PWM::writeFile(string path, string filename, unsigned int value) {
	return writeFile(path, filename, to_string(value));
}


std::string PWM::readFile(string path, string filename) {
	ifstream fs;
	fs.open((path+filename).c_str());
	if (!fs.is_open()) {
		perror("PWM: read failed to open file ");
		return 0;
	}

	string input;
	getline(fs, input);
	fs.close();

	return input;
}


int PWM::setPeriod_ns(unsigned int ns) {
	if (ns == 0) {
		perror("PWM: frequency = 0 is invalid");
		return -1;
	}
	return writeFile(this->path, "period", ns);
}


int PWM::setPeriod_us(float us) {
	return setPeriod_ns(us * 1000);
}


int PWM::setPeriod_ms(float ms) {
	return setPeriod_ns(ms * 1000000);
}


unsigned int PWM::getPeriod_ns() {
	return stoi(readFile(this->path, "period"));
}


float PWM::getPeriod_us() {
	return getPeriod_ns() / 1000.0;
}


float PWM::getPeriod_ms() {
	return getPeriod_ns() / 1000000.0;
}


int PWM::setFrequency_Hz(float freq) {
	if (freq == 0) {
		perror("PWM: frequency = 0 is invalid");
		return -1;
	}
	return setPeriod_ns(1000000000 / freq);
}


int PWM::setFrequency_kHz(float freq) {
	return setFrequency_Hz(freq * 1000);
}


int PWM::setFrequency_MHz(float freq) {
	return setFrequency_Hz(freq * 1000000);
}


float PWM::getFrequency_Hz() {
	return 1000000000. / getPeriod_ns();
}


float PWM::getFrequency_kHz() {
	return getFrequency_Hz() / 1000;
}


float PWM::getFrequency_MHz() {
	return getFrequency_Hz() / 1000000;
}


int PWM::setDutyCycle(float duty) {
	if (duty > 1 || duty < 0) {
		perror("PWM: duty should be in range [0.0, 1.0]");
		return -1;
	}
	return writeFile(this->path, "duty_cycle", duty * getPeriod_ns());
}


float PWM::getDutyCycle() {
	return stof(readFile(this->path, "duty_cycle")) / getPeriod_ns();
}


int PWM::setPolarity(PWM_POLARITY mode) {
	if (mode == ACTIVE_LOW) {
		return writeFile(this->path, "polarity", "inversed");
	}
	else if (mode == ACTIVE_HIGH) {
		return writeFile(this->path, "polarity", "normal");
	}
	else {
		perror("PWM: no such Polarity mode");
		return -1;
	}
}


PWM_POLARITY PWM::getPolarity() {
	string polar = readFile(this->path, "polarity");

	if (polar == "normal")
		return ACTIVE_HIGH;
	else
		return ACTIVE_LOW;
}


int PWM::start() {
	return writeFile(this->path, "enable", 1);
}


int PWM::stop() {
	return writeFile(this->path, "enable", 0);
}

} /* namespace BBB */