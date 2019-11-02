#ifndef __PWM__
#define __PWM__

#include <string>

#define PWM_PATH	"/sys/class/pwm/"

#define PWM0A	"pwm-1:0"
#define PWM0B	"pwm-1:1"
#define PWM1A	"pwm-4:0"
#define PWM1B	"pwm-4:1"
#define PWM2A	"pwm-7:0"
#define PWM2B	"pwm-7:1"

namespace BBB {

enum PWM_POLARITY {ACTIVE_LOW=0, ACTIVE_HIGH=1};

class PWM {
public:
	PWM(std::string);
	~PWM();

	virtual std::string getChannel();

	virtual int setPeriod_ns(unsigned int ns);
	virtual int setPeriod_us(float us);
	virtual int setPeriod_ms(float ns);

	virtual unsigned int getPeriod_ns();
	virtual float getPeriod_us();
	virtual float getPeriod_ms();

	virtual int setFrequency_Hz(float freq);
	virtual int setFrequency_kHz(float freq);
	virtual int setFrequency_MHz(float freq);

	virtual float getFrequency_Hz();
	virtual float getFrequency_kHz();
	virtual float getFrequency_MHz();

	virtual int setDutyCycle(float);
	virtual float getDutyCycle();

	virtual int setPolarity(PWM_POLARITY = ACTIVE_HIGH);
	virtual PWM_POLARITY getPolarity();


	virtual int start();
	virtual int stop();

private:
	std::string channel;
	std::string path;

	int writeFile(std::string, std::string, unsigned int);
	int writeFile(std::string, std::string, std::string);
	std::string readFile(std::string, std::string);
};

} /* namespace BBB */

#endif /* __PWM__ */