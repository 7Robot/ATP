#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <linux/input.h>
#include <linux/joystick.h>

#include "jsread.h"

#define NAME_LENGTH 128


int jsread(char * device, int(*callback)(int,int*,int,char*), int verbose)
{
    int fd, i;
    unsigned char axes = 2;
    unsigned char buttons = 2;
    int version = 0x000800;
    char name[NAME_LENGTH] = "Unknown";
    uint16_t btnmap[KEY_MAX - BTN_MISC + 1];
    uint8_t axmap[ABS_MAX + 1];

    int *axis;
    char *button;
    struct js_event js;

    if ((fd = open(device, O_RDONLY)) < 0) {
        perror("open");
        return 1;
    }

    ioctl(fd, JSIOCGVERSION, &version);
    ioctl(fd, JSIOCGAXES, &axes);
    ioctl(fd, JSIOCGBUTTONS, &buttons);
    ioctl(fd, JSIOCGNAME(NAME_LENGTH), name);
    ioctl(fd, JSIOCGAXMAP, axmap);
    ioctl(fd, JSIOCGBTNMAP, btnmap);


    printf("Driver version is %d.%d.%d.\n",
        version >> 16, (version >> 8) & 0xff, version & 0xff);

    printf("Joystick (%s) has %d axes (", name, axes);
    for (i = 0; i < axes; i++)
        printf("%s%s", i > 0 ? ", " : "", axis_names[axmap[i]]);
    puts(")");

    printf("and %d buttons (", buttons);
    for (i = 0; i < buttons; i++)
        printf("%s%s", i > 0 ? ", " : "", button_names[btnmap[i] - BTN_MISC]);
    puts(").");

    if (axes < 2) {
        printf("No enough axes (at least 2 requiered)\n");
        return 1;
    }

    axis = calloc(axes, sizeof(int));
    button = calloc(buttons, sizeof(char));

    while (1) {
        if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
            perror("\nread");
            return 1;
        }

        switch(js.type & ~JS_EVENT_INIT) {
        case JS_EVENT_BUTTON:
            button[js.number] = js.value;
            break;
        case JS_EVENT_AXIS:
            axis[js.number] = js.value;
            break;
        }

        if (verbose) {

            printf("\r");

            if (axes) {
                printf("Axes: ");
                for (i = 0; i < axes; i++)
                    printf("%2d:%6d ", i, axis[i]);
            }

            if (buttons) {
                printf("Buttons: ");
                for (i = 0; i < buttons; i++)
                    printf("%2d:%s ", i, button[i] ? "on " : "off");
            }

            fflush(stdout);

        }

        callback(axes, axis, buttons, button);
    }
}
