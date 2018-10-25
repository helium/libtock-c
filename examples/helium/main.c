#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <console.h>
#include <led.h>
#include <timer.h>
#include <rfcore.h>

int num_leds;

static void nop(
  int a __attribute__((unused)),
  int b __attribute__((unused)),
  int c __attribute__((unused)),
  void* d __attribute__((unused))) {}

int main(void) {
    // Ask the kernel how many LEDs are on this board.
    num_leds = led_count();

    // Set some basic device parameters for the client
    
    if (helium_driver_check() != 0) {
        char err[] = "Driver check failure\r\n";
        putnstr_async(err, sizeof(err), nop, NULL);
    } 

    // Blink the LEDs in a binary count pattern and scale
    // to the number of LEDs on the board.
    for (int count = 0; ; count++) {
        for (int i = 0; i < num_leds; i++) {
            if (count & (1 << i)) {
                led_on(i);
            } else {
                led_off(i);
            }
        }
        // This delay uses an underlying timer in the kernel.
        delay_ms(250);
    }
}
