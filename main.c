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

int main()
{
    struct js_event event;
    const char *device = "/dev/input/js0";

    int js = open(device, O_RDONLY);

    if (js == -1)
        return -1;

    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0)
    {
        if (event.type == JS_EVENT_AXIS) {
       		printf("axis: %d value: %d\n", event.number, event.value);
	}
    }

    close(js);

    return 0;
}
