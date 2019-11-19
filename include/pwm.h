#ifndef __PWM__
#define __PWM__

#include <string>
#include <cstdint>

#define PWM_PATH	"/sys/class/pwm/"

#define PWM0	"pwmchip1"
#define PWM1	"pwmchip4"
#define PWM2	"pwmchip7"

namespace BBB {

class PWM {
public:
	enum POLARITY {ACTIVE_LOW=0, ACTIVE_HIGH=1};
	enum CHANNEL {A=0, B=1};

	PWM(const char* chip);
	~PWM();

	virtual int setPeriod_ns(uint32_t ns);
	virtual int setPeriod_us(double us);
	virtual int setPeriod_ms(double ms);

	virtual int setFrequency_Hz(double freq);
	virtual int setFrequency_kHz(double freq);
	virtual int setFrequency_MHz(double freq);

	virtual uint32_t getPeriod_ns();
	virtual double getPeriod_us();
	virtual double getPeriod_ms();

	virtual double getFrequency_Hz();
	virtual double getFrequency_kHz();
	virtual double getFrequency_MHz();

	virtual int setDutyCycle(CHANNEL, int);
	virtual double getDutyCycle(CHANNEL);

	virtual int setPolarity(CHANNEL, POLARITY = ACTIVE_HIGH);
	virtual POLARITY getPolarity(CHANNEL);

	virtual int start(CHANNEL);
	virtual int stop(CHANNEL);
	

private:
	std::string pathA, pathB, chipPath;
	char id;

	virtual int reset();

	int activate(CHANNEL);
	int deactivate(CHANNEL);
	int writeFile(std::string, std::string, uint32_t);
	int writeFile(std::string, std::string, std::string);
	std::string readFile(std::string, std::string);
};

} /* namespace BBB */

#endif /* __PWM__ */