/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-29     supperthomas first version
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "include/esp_foc/esp_foc.h"
#include "esp_foc_devices.h"

static esp_foc_inverter_t *inverter;
static esp_foc_rotor_sensor_t *sensor;
static esp_foc_axis_t axis;
static float now = 0;


int main(void)
{
	/* initialize rtthread specific devices */
	devices_rtthread_init();
	inverter = inverter_rtthread_new();
	sensor = rotor_sensor_rtthread_new();

	esp_foc_initialize_axis(&axis, inverter, sensor, 8);
    while (1)
    {
		float now = ((float)rt_tick_get_millisecond()) * 0.001f;
		esp_foc_run(&axis, now);
		rt_thread_mdelay(1);
    }
    return RT_EOK;
}

