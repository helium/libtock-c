#include "rfcore.h"
#include "tock.h"
#include <stdlib.h>

#define HELIUM_DRIVER           (0xCC1312)
#define ALLOW_NUM_W             (0)
#define ALLOW_NUM_R             (1)
#define ALLOW_NUM_C             (2)
#define SUBSCRIBE_TX            (0)
#define SUBSCRIBE_RX            (1)
#define DRIVER_SIMPLE_RFCORE    (0x000CC1312)

enum cmd {
    COMMAND_DRIVER_CHECK = 0,
    COMMAND_INITIALIZE = 1,
    COMMAND_RETURN_RADIO_STATUS = 2,
    COMMAND_STOP_RADIO_OPERATION = 3,
    COMMAND_FORCE_STOP_RADIO = 4,
    COMMAND_SET_DEVICE_CONFIG = 5,
    COMMAND_SEND = 6,
    COMMAND_SET_ADDRESS = 7,

};


typedef struct {
    const char * address;
    bool on;
} device_params;

unsigned char BUF_CFG[27];

// Internal callback for transmission
static int tx_result;
static int tx_acked;

static void tx_done_callback(int result,
                             int acked,
                             __attribute__ ((unused)) int arg3,
                             void* ud) {
  tx_result     = result;
  tx_acked      = acked;
  *((bool*) ud) = true;
}

// Internal callback for receive
static void rx_done_callback(__attribute__ ((unused)) int pans,
                             __attribute__ ((unused)) int dst_addr,
                             __attribute__ ((unused)) int src_addr,
                             void* ud) {
  *((bool*) ud) = true;
}


int helium_driver_check(void) {
    return command(HELIUM_DRIVER, COMMAND_DRIVER_CHECK, 0, 0);
}

int helium_init(void) {
    return command(HELIUM_DRIVER, COMMAND_INITIALIZE, 0, 0);
}

int helium_set_address(const char *address) {
    if (!address) return TOCK_EINVAL;
    int err = allow(HELIUM_DRIVER, ALLOW_NUM_C, (void *) address, 0);
    if (err<0) return err;
    return command(HELIUM_DRIVER, COMMAND_SET_ADDRESS, 0, 0);
}

int helium_send(unsigned short addr,
        fec_type_t ftype,
        const char *payload,
        unsigned char len) {
  // Setup parameters in ALLOW_CFG and ALLOW_TX
  int err = allow(HELIUM_DRIVER, ALLOW_NUM_C, (void *) BUF_CFG, 11);
  if (err < 0) return err;
  BUF_CFG[0] = ftype;
  err = allow(HELIUM_DRIVER, ALLOW_NUM_W, (void *) payload, len);
  if (err < 0) return err;

  // Subscribe to the transmit callback
  bool tx_done = false;
  err = subscribe(HELIUM_DRIVER, SUBSCRIBE_TX,
                  tx_done_callback, (void *) &tx_done);
  if (err < 0) return err;

  // Issue the send command and wait for the transmission to be done.
  err = command(HELIUM_DRIVER, COMMAND_SEND, (unsigned int) addr, 0);
  if (err < 0) return err;
  yield_for(&tx_done);
  if (tx_result != TOCK_SUCCESS) {
    return tx_result;
  } else if (tx_acked == 0) {
    return TOCK_ENOACK;
  } else {
    return TOCK_SUCCESS;
  }

}

