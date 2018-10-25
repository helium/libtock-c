#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        None = 0,
        LdpcTc128 = 1,
        LdpcTc256 = 2,
        LdpcTc512 = 3,
    } fec_type_t;

    int helium_init(void);
    
    int helium_driver_check(void);

    int helium_set_address(const char *address);

    int helium_send(unsigned short addr,
            fec_type_t type,
            const char *payload,
            unsigned char len);

#ifdef __cplusplus
}
#endif
