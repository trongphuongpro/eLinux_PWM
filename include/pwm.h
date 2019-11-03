#ifndef __PWM__
#define __PWM__

#include <string>
#include <cstdint>

#define PWM_PATH	"/sys/class/pwm/"

#define PWM0	"pwmchip1"
#define PWM1	"pwmchip4"
#define PWM2	"pwmchip7"

namespace BBB {

enum PWM_POLARITY {ACTIVE_LOW=0, ACTIVE_HIGH=1};
enum PWM_CHANNEL {PWM_A=0, PWM_B=1};

class PWM {
public:
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

	virtual int setDutyCycle(PWM_CHANNEL, int);
	virtual double getDutyCycle(PWM_CHANNEL);

	virtual int setPolarity(PWM_CHANNEL, PWM_POLARITY = ACTIVE_HIGH);
	virtual PWM_POLARITY getPolarity(PWM_CHANNEL);

	virtual int start(PWM_CHANNEL);
	virtual int stop(PWM_CHANNEL);
	

private:
	std::string pathA, pathB, chipPath;
	char id;
	uint32_t period;

	virtual int setPeriod_ns(PWM_CHANNEL, uint32_t ns);
	virtual int setPeriod_us(PWM_CHANNEL, double us);
	virtual int setPeriod_ms(PWM_CHANNEL, double ms);

	virtual int setFrequency_Hz(PWM_CHANNEL, double freq);
	virtual int setFrequency_kHz(PWM_CHANNEL, double freq);
	virtual int setFrequency_MHz(PWM_CHANNEL, double freq);

	virtual void reset();

	int activate(PWM_CHANNEL);
	int deactivate(PWM_CHANNEL);
	int writeFile(std::string, std::string, uint32_t);
	int writeFile(std::string, std::string, std::string);
	std::string readFile(std::string, std::string);
};

} /* namespace BBB */

#endif /* __PWM__ */