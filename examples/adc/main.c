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
      res = 1098 * temp;
      char output0[] = "[ADC] volt            ";
      itoa (res, &output0[11], 10);
      putnstr(output0, sizeof(output0));
      putnstr("\r\n", 2);

      delay_ms(1000);
    
  }

  
  return 0;
}
