#include <fcntl.h>
#include <stdio.h>
#include <linux/joystick.h>

int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    return -1;
}

float getPercentage(int value) {
	return (float)(value * -1) / (float)32767;
}

int main()
{
    struct js_event event;
    const char *device = "/dev/input/js0";

    int js = open(device, O_RDONLY);
    int left_axis = 1;
    int right_axis = 4;

    if (js == -1)
        return -1;

    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0)
    {
        if (event.type == JS_EVENT_AXIS && event.value <= 0) {
           if (event.number == left_axis) {
		   printf("axis: %d value: %f\n", event.number, getPercentage(event.value));

	   } else if (event.number == right_axis) {
		   printf("axis: %d value: %f\n", event.number, getPercentage(event.value));
  
	   } 
       	}
    }
    close(js);

    return 0;
}
