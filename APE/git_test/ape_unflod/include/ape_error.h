#ifndef _APEERROR_H
#define _APEERROR_H

typedef enum
{
    
    /** common error codes */
    success             = 0x00000000,
    not_implemented      = 0x00000001,
    not_supported        = 0x00000002,
    not_initialized      = 0x00000003,
    bad_parameter        = 0x00000004,
    timeout             = 0x00000005,
    insufficient_memory  = 0x00000006,
    readonly_attribute   = 0x00000007,
    invalid_state        = 0x00000008,
    invalid_address      = 0x00000009,
    invalid_size         = 0x0000000A,
    bad_value            = 0x0000000B,
    already_allocated    = 0x0000000D,
    busy                = 0x0000000E,
    module_not_present    = 0x000a000E,
    resource_error       = 0x0000000F,
    count_mismatch       = 0x00000010,
    overflow            = 0x00000011,
    disconnected        = 0x00000012,
    file_not_found        = 0x00000013,
    test_application_failed  = 0x00000014,
    device_not_found      = 0x00000015,
    ape_success         = success

}ape_error;

#endif // INCLUDED_APEERROR_H
