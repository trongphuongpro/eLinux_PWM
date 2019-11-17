## library for using PWM on BeagleBone Black Rev.C

### Support: PWM0, PWM1, PWM2

### Requirements
**create udev rules (in /etc/udev/rules.d) with following content:**

*SUBSYSTEM=="pwm", KERNEL=="pwmchip\*", ACTION=="add", PROGRAM="/bin/sh -c 'chgrp -R pwm /sys/class/pwm'"*
*SUBSYSTEM=="pwm", KERNEL=="pwmchip\*", ACTION=="add", PROGRAM="/bin/sh -c 'chmod -R g=u /sys/class/pwm'"*


**add uboot overlays for PWM channels**
open file **/boot/uEnv.txt** and change:

*uboot_overlay_addr4=/lib/firmware/BB-PWM0-00A0.dtbo*
*uboot_overlay_addr5=/lib/firmware/BB-PWM1-00A0.dtbo*
*uboot_overlay_addr6=/lib/firmware/BB-PWM2-00A0.dtbo*
