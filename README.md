## library for using PWM on BeagleBone Black Rev.C

### Support: PWM0, PWM1, PWM2

### Requirements
**create udev rules (in /etc/udev/rules.d) with following content:**

*SUBSYSTEM=="pwm", KERNEL=="pwmchip*", ACTION=="add", PROGRAM="/bin/sh -c 'chgrp -R pwm /sys/class/pwm'"*
*SUBSYSTEM=="pwm", KERNEL=="pwmchip*", ACTION=="add", PROGRAM="/bin/sh -c 'chmod -R g=u /sys/class/pwm'"*

