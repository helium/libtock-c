#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>


#include <adc.h>
#include <console.h>

char hello[] = "[ADC] Starting ADC App.\r\n";

static bool adc = false;



int main(void) {
  putnstr(hello, sizeof(hello));

  adc    = driver_exists(DRIVER_NUM_ADC);
  if (adc) {
    char detected[] = "[ADC] Driver detected.\r\n";
    putnstr(detected, sizeof(detected));
  }

  uint32_t res = 0;
  while(true) {
      uint16_t temp;
      adc_sample_sync(2, &temp);
      printf("Raw: %u\r\n", temp);
      res =  (((temp * 4300) + 2047) / 4095);

      printf("Volt: %lu\r\n", res);

      delay_ms(1000);
    
  }

  
  return 0;
}
