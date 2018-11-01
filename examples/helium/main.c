#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <console.h>
#include <led.h>
#include <timer.h>
#include <rfcore.h>

#define PKT_SIZE 128 
int num_leds;
#define TEAM_NAME "something";

unsigned char address[10] = "something";

int main(void) {
    // Set some basic device parameters for the client
    if (!helium_driver_check()) {
        printf("Driver check OK\r\n");
    } else {
        printf("Driver check FAIL\r\n");
    } 
  
    if (!helium_set_address(address)) {
        printf("Set address OK\r\n");
    } else {
        printf("Set address FAIL\r\n");
    }
    
    int a;
    int b;
    int c;

    char message[157]; 
    sprintf(message, "{\"team\":\"%s\",\"payload\":{\"temp\":%d,\"xyz\":%d,\"lux\":%d}}", address,a,b,c);
    
    /*    
    led_toggle(1);
    int res = helium_send(0x0000, CAUT_TYPE_NONE, message, sizeof(message));
    if (res != TOCK_SUCCESS) {
        printf("\r\nSend Fail\r\n");
        led_toggle(0);
        delay_ms(500);
        led_toggle(1);
        led_toggle(0);
    } else {
        printf("\r\nSend SUCCESS\r\n");
        led_toggle(1);
        delay_ms(500);
    }
    */
    while(1) {
        led_toggle(1);
        int res = helium_send(0x0000, CAUT_TYPE_NONE, message, sizeof(message));
        if (res != TOCK_SUCCESS) {
            printf("\r\nSend Fail\r\n");
            led_toggle(0);
            delay_ms(500);
            led_toggle(1);
            led_toggle(0);
        } else {
            printf("\r\nSend SUCCESS\r\n");
            led_toggle(1);
            delay_ms(500);
        }
    }
    return 0;
}
