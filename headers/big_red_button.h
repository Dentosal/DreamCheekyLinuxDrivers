#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Functions that software fill implement
void on_open();
void on_close();
void on_press();
void on_release();

int brb_listen() {
    int fd, res, prev, prev_lid_up, prev_button_down, lid_up, button_down, first = 1;
    unsigned char buf[8];

    fd = open("/dev/big_red_button", O_RDWR|O_NONBLOCK); // open device file

    if (fd < 0) {
        perror("Unable to open device");
        return 1;
    }

    while (1) {
        memset(buf, 0, sizeof(buf));
        buf[0] = 0x08;
        buf[7] = 0x02;

        res = write(fd, buf, 8);
        if (res < 0) {
            // unable to write
            perror("write");
            return 1;
        }

        memset(buf, 0, sizeof(buf)); // clear buffer
        res = read(fd, buf, 8);      // read data from device

        if (res >= 0) {
            if (first) {
                prev = buf[0];
                first = 0;
            }
            if (buf[0] != prev) {   // if state changed
                lid_up = (buf[0]&2)>>1;     // bit #2
                button_down = !(buf[0]&1);  // bit #1

                prev_lid_up = (prev&2)>>1;
                prev_button_down = !(prev&1);

                if (lid_up && !prev_lid_up) {
                    on_open();
                } else if (!lid_up && prev_lid_up) {
                    on_close();
                }

                if (button_down && !prev_button_down) {
                    on_press();
                } else if (!button_down && prev_button_down) {
                    on_release();
                }

                prev = buf[0];
            }
        }
        usleep(1000);
    }

    return 0;
}
