#include "big_red_button.h"
#include <time.h>

int main() {
    brb_listen();
    return 0;
}

void on_press() {
    printf("button pressed, %d\n", time(NULL));
}

void on_release() {
    printf("button released, %d\n", time(NULL));
}

void on_open() {
    printf("lid opened, %d\n", time(NULL));
}

void on_close() {
    printf("lid closed, %d\n", time(NULL));
}
