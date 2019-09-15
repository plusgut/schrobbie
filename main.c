#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/joystick.h>
#include <wiringPi.h>

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
    int min_value = 700;
    int max_value = 2000;
    int grow_value = max_value - min_value;

    return min_value + percentage * grow_value;
}
int main()
{
    struct js_event event;
    const char *device = "/dev/input/js0";

    int js = open(device, O_RDONLY);
    int left_axis = 1;
    int right_axis = 4;
    int left_pin = 18;
    int right_pin = 19;

    if (js == -1)
    {
        return -1;
    }

    if (wiringPiSetup () == -1)
    {
       return -2;
    }

    pinMode(left_pin, PWM_OUTPUT);
    pinMode(right_pin, PWM_OUTPUT);

    /* This loop will exit if the controller is unplugged. */
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
