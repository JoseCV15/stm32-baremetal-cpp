#ifndef STATUS_H
#define STATUS_H

/**
 * @brief Status codes for all implemented drivers. All drivers that return a status should use status_t
 * So that every driver returns the same error/status contract.
*/
typedef enum {
    STATUS_OK  = 0,               /*Success */
    STATUS_INVALID_ARGUMENT = -1, /* Bad argument passed (NULL, out of range, invalid config)*/
    STATUS_TIMEOUT          = -2,          /* Operation exceed timeout */
    STATUS_BUSY             = -3,              /* Already in use */
    STATUS_NOT_SUPPORTED      = -4, /* Not supported */
    STATUS_NOT_INITIALIZED  = -5,
    STATUS_ERROR            = -6
} status_t;



#endif