#ifndef __PWM__
#define __PWM__

#include <string>
#include <cstdint>

#define PWM_PATH	"/sys/class/pwm/"


namespace BBB {

enum PWM_POLARITY {ACTIVE_LOW=0, ACTIVE_HIGH=1};

typedef struct {
	std::string chip;
	std::string channel;
} PWM_CHANNEL;

extern PWM_CHANNEL PWM0A;
extern PWM_CHANNEL PWM0B;
extern PWM_CHANNEL PWM1A;
extern PWM_CHANNEL PWM1B;
extern PWM_CHANNEL PWM2A;
extern PWM_CHANNEL PWM2B;


class PWM {
public:
	PWM(PWM_CHANNEL&);
	~PWM();

	virtual std::string getChannel();

	virtual int setPeriod_ns(uint32_t ns);
	virtual int setPeriod_us(double us);
	virtual int setPeriod_ms(double ms);

	virtual uint32_t getPeriod_ns();
	virtual double getPeriod_us();
	virtual double getPeriod_ms();

	virtual int setFrequency_Hz(double freq);
	virtual int setFrequency_kHz(double freq);
	virtual int setFrequency_MHz(double freq);

	virtual double getFrequency_Hz();
	virtual double getFrequency_kHz();
	virtual double getFrequency_MHz();

	virtual int setDutyCycle(int);
	virtual double getDutyCycle();

	virtual int setPolarity(PWM_POLARITY = ACTIVE_HIGH);
	virtual PWM_POLARITY getPolarity();


	virtual int start();
	virtual int stop();

private:
	std::string channelPath, chipPath, channel;

	int activate();
	int deactivate();
	int writeFile(std::string, std::string, uint32_t);
	int writeFile(std::string, std::string, std::string);
	std::string readFile(std::string, std::string);
};

} /* namespace BBB */

#endif /* __PWM__ */