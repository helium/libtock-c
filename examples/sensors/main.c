#include <stdbool.h>
#include <stdio.h>

#include <adc.h>
#include <console.h>

char hello[] = "[ADC] Starting ADC App.\r\n";

static bool adc = false;



int main(void) {
  putstr(&hello);

  adc    = driver_exists(DRIVER_NUM_ADC);



  return 0;
}
