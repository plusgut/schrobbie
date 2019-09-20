#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/joystick.h>
#include <wiringPi.h>


const int PWM_MIN_VALUE = 700;
const int PWM_MAX_VALUE = 2000;
const int ESC_LEFT_PIN = 1; // 18;
const int ESC_RIGHT_PIN = 24; // 19;

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
    pwmWrite(ESC_LEFT_PIN, value);
    pwmWrite(ESC_RIGHT_PIN, value);
}

void armEsc() {
    printf("Arming procedure is running\n");
    delay(2000);
    setPwmValueBoth(PWM_MAX_VALUE);
    delay(3000);
    setPwmValueBoth(PWM_MIN_VALUE);
    delay(12000);
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

    if (wiringPiSetup() == -1)
    {
       printf("No wiring setup possible\n");
       return -2;
    }

    pwmSetClock(1920);
    pwmSetRange(200);
    pwmSetMode(PWM_MODE_MS);

    pinMode(ESC_LEFT_PIN, PWM_OUTPUT);
    pinMode(ESC_LEFT_PIN, PWM_OUTPUT);
    setPwmValueBoth(PWM_MODE_MS);

    armEsc();

    printf("Arming complete\n");

    while (read_event(js, &event) == 0)
    {
        if (event.type == JS_EVENT_AXIS && event.value <= 0)
        {
            if (event.number == left_axis)
            {
                printf("axis: %d value: %d\n", event.number, getPwmValue(event.value));
                pwmWrite(ESC_LEFT_PIN, getPwmValue(event.value));
            } else if (event.number == right_axis) {
                printf("axis: %d value: %d\n", event.number, getPwmValue(event.value));
                pwmWrite(ESC_LEFT_PIN, getPwmValue(event.value));
            }
        }
    }
    close(js);

    return 0;
}
