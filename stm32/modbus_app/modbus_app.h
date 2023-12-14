
#ifndef MODBUS_APP_H
#define MODBUS_APP_H

#include "tkc/types_def.h"
#include "tkc/thread.h"
#include "platforms/common/rtos.h"

ret_t modbus_service_start(const char* device);
ret_t modbus_service_stop(void);

#endif // MODBUS_APP_H

