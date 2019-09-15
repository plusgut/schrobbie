#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/joystick.h>
#include <wiringPi.h>


const int PWM_MIN_VALUE = 700;
const int PWM_MAX_VALUE = 2000;

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

void armEsc(int pin) {
    printf("Arming procedure for %d\n", pin);
    delay(2000);
    pwmWrite(pin, PWM_MAX_VALUE);
    delay(3000);
    pwmWrite(pin, PWM_MIN_VALUE);
    delay(12000);
    pwmWrite(pin, 0);
    delay(2000);
    pwmWrite(pin, PWM_MIN_VALUE);
}

int main()
{ 
    printf("Start \n");

    struct js_event event;
    const char *device = "/dev/input/js0";

    int js = open(device, O_RDONLY);

    int left_axis = 1;
    int right_axis = 4;
    int left_pin = 18;
    int right_pin = 19;

    if (js == -1)
    {
       printf("No joystick \n");
       return -1;
    }

    if (wiringPiSetup () == -1)
    {
       printf("No wiring setup possible\n");
       return -2;
    }

    pinMode(left_pin, PWM_OUTPUT);
    pinMode(right_pin, PWM_OUTPUT);
    pwmWrite(left_pin, 0);
    pwmWrite(right_pin, 0);

    armEsc(left_pin);
    armEsc(right_pin);

    printf("Arming complete\n");

    while (read_event(js, &event) == 0)
    {
        if (event.type == JS_EVENT_AXIS)
        {
            if (event.number == left_axis)
            {
                printf("axis: %d value: %d\n", event.number, getPwmValue(event.value));
                pwmWrite(left_pin, getPwmValue(event.value));
            } else if (event.number == right_axis) {
                printf("axis: %d value: %d\n", event.number, getPwmValue(event.value));
                pwmWrite(right_pin, getPwmValue(event.value));
            }
        }
    }
    close(js);

    return 0;
}
