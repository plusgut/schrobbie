#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/joystick.h>
#include <bcm2835.h>

const int PWM_MIN_VALUE = 30;
const int PWM_MAX_VALUE = 700;
const int ESC_LEFT_PIN = 18;
const int ESC_RIGHT_PIN = 19;
const int PWM_CHANNEL_LEFT = 0;
const int PWM_CHANNEL_RIGHT = 1;
const int RANGE = 1024;
const int CLOCK = 1200000 / 140;

int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event)) {
        return 0;
    }

    return -1;
}

float getPercentage(int value)
{
    return (float)(value * -1) / (float)32767;
}

int getPwmValue(int value) {
    float percentage = getPercentage(value);

    return PWM_MIN_VALUE + percentage * (PWM_MAX_VALUE - PWM_MIN_VALUE);
}

void setPwmValueBoth(int value)
{
    bcm2835_pwm_set_data(PWM_CHANNEL_LEFT, value);
    bcm2835_pwm_set_data(PWM_CHANNEL_RIGHT, value);
}

void armEsc() {
    printf("Arming procedure is running\n");
    delay(2000);
    printf("Setting to max value\n");
    setPwmValueBoth(PWM_MAX_VALUE);
    delay(3000);
    printf("Setting to min pulse\n");
    setPwmValueBoth(PWM_MIN_VALUE);
    delay(12000);
    printf("Zeroing again\n");
    setPwmValueBoth(0);
    delay(2000);
    setPwmValueBoth(PWM_MIN_VALUE);
}

int main()
{ 
    printf("Start \n");

    struct js_event event;
    const char *device = "/dev/input/js0";

    int js = open(device, O_RDONLY);

    int left_axis = 1;
    int right_axis = 4;

    if (js == -1)
    {
       printf("No joystick \n");
       return -1;
    }

    if (!bcm2835_init())
    {
       printf("No wiring setup possible\n");
       return -2;
    }

    bcm2835_gpio_fsel(ESC_LEFT_PIN, BCM2835_GPIO_FSEL_ALT5);
    bcm2835_gpio_fsel(ESC_RIGHT_PIN, BCM2835_GPIO_FSEL_ALT5);

    bcm2835_pwm_set_clock(CLOCK);
    bcm2835_pwm_set_mode(PWM_CHANNEL_LEFT, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL_LEFT, RANGE);

    bcm2835_pwm_set_mode(PWM_CHANNEL_RIGHT, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL_RIGHT, RANGE);

    printf("Zeroing previous state\n");
    setPwmValueBoth(0);

    armEsc();

    printf("Arming complete\n");

    while (read_event(js, &event) == 0)
    {
        if (event.type == JS_EVENT_AXIS && event.value <= 0)
        {
            if (event.number == left_axis)
            {
                printf("axis: %d value: %d\n", event.number, getPwmValue(event.value));
                bcm2835_pwm_set_data(PWM_CHANNEL_LEFT, getPwmValue(event.value));
            } else if (event.number == right_axis) {
                printf("axis: %d value: %d\n", event.number, getPwmValue(event.value));
                bcm2835_pwm_set_data(PWM_CHANNEL_RIGHT, getPwmValue(event.value));
            }
        }
    }
    close(js);

    return 0;
}
