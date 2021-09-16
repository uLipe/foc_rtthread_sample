#pragma once 

#include "include/esp_foc/esp_foc.h"

void devices_rtthread_init();
esp_foc_inverter_t *inverter_rtthread_new(void);
esp_foc_rotor_sensor_t *rotor_sensor_rtthread_new(void);