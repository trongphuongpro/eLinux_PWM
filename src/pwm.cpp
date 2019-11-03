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

PWM_CHANNEL PWM0A = {"pwmchip1", "pwm-1:0"};
PWM_CHANNEL PWM0B = {"pwmchip1", "pwm-1:1"};
PWM_CHANNEL PWM1A = {"pwmchip4", "pwm-4:0"};
PWM_CHANNEL PWM1B = {"pwmchip4", "pwm-4:1"};
PWM_CHANNEL PWM2A = {"pwmchip7", "pwm-7:0"};
PWM_CHANNEL PWM2B = {"pwmchip7", "pwm-7:1"};


PWM::PWM(PWM_CHANNEL& pwm) {
	this->channel = pwm.channel;
	this->channelPath = PWM_PATH + pwm.channel + "/";
	this->chipPath = PWM_PATH + pwm.chip + "/";

	deactivate();
	usleep(500);
	activate();
	usleep(500);
}


PWM::~PWM() {

}


int PWM::activate() {
	writeFile(this->chipPath, "export", 0);
	writeFile(this->chipPath, "export", 1);
	return 0;
}


int PWM::deactivate() {
	writeFile(this->chipPath, "unexport", 0);
	writeFile(this->chipPath, "unexport", 1);
	return 0;
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
		cout << filename << endl;
		perror("PWM: write failed to open file ");
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
	if (ns == 0) {
		perror("PWM: frequency = 0 is invalid");
		return -1;
	}
	return writeFile(this->channelPath, "period", ns);
}


int PWM::setPeriod_us(double us) {
	return setPeriod_ns(us * 1000);
}


int PWM::setPeriod_ms(double ms) {
	return setPeriod_ns(ms * 1000000);
}


uint32_t PWM::getPeriod_ns() {
	return stoi(readFile(this->channelPath, "period"));
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


int PWM::setDutyCycle(int duty) {
	if (duty > 100 || duty < 0) {
		perror("PWM: duty should be in range [0, 100]");
		return -1;
	}
	return writeFile(this->channelPath, "duty_cycle", duty * getPeriod_ns() / 100);
}


double PWM::getDutyCycle() {
	return stof(readFile(this->channelPath, "duty_cycle")) / getPeriod_ns();
}


int PWM::setPolarity(PWM_POLARITY mode) {
	if (mode == ACTIVE_LOW) {
		return writeFile(this->channelPath, "polarity", "inversed");
	}
	else if (mode == ACTIVE_HIGH) {
		return writeFile(this->channelPath, "polarity", "normal");
	}
	else {
		perror("PWM: no such Polarity mode");
		return -1;
	}
}


PWM_POLARITY PWM::getPolarity() {
	string polar = readFile(this->channelPath, "polarity");

	if (polar == "normal")
		return ACTIVE_HIGH;
	else
		return ACTIVE_LOW;
}


int PWM::start() {
	return writeFile(this->channelPath, "enable", 1);
}


int PWM::stop() {
	return writeFile(this->channelPath, "enable", 0);
}

} /* namespace BBB */