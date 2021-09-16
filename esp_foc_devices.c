#include <rtthread.h>
#include <rtdevice.h>
#include "esp_foc_devices.h"

typedef struct {
	struct rt_device_pwm *pwm0;
	struct rt_device_pwm *pwm1;
	float dc_link_voltage;
    esp_foc_inverter_t interface;
}esp_foc_rtthread_inverter;

typedef struct {
    float current_count;
    float counts_per_revolution;
    esp_foc_rotor_sensor_t interface;
}esp_foc_rtthread_sensor_t;

static esp_foc_rtthread_inverter inverter;
static esp_foc_rtthread_sensor_t sensor;

static float get_dc_link_voltage (esp_foc_inverter_t *self)
{
    esp_foc_rtthread_inverter *obj = 
        rt_container_of(self, esp_foc_rtthread_inverter, interface);

    return obj->dc_link_voltage;
}

static void set_voltages(esp_foc_inverter_t *self,
                    float v_u,
                    float v_v,
                    float v_w)
{
	const float period_ns = 62500.0f;
	rt_uint32_t dcu, dcv, dcw;

    esp_foc_rtthread_inverter *obj = 
        rt_container_of(self, esp_foc_rtthread_inverter, interface);

    if(v_u > obj->dc_link_voltage) {
        v_u = obj->dc_link_voltage;
    } else if (v_u < 0.0f) {
        v_u = 0.0f;
    }

    if(v_v > obj->dc_link_voltage) {
        v_v = obj->dc_link_voltage;
    } else if (v_v < 0.0f) {
        v_v = 0.0f;
    }

    if(v_w > obj->dc_link_voltage) {
        v_w = obj->dc_link_voltage;
    } else if (v_w < 0.0f) {
        v_w = 0.0f;
    }

	dcu = (rt_uint32_t)(period_ns - (v_u / obj->dc_link_voltage * (period_ns - 1)));
	dcv = (rt_uint32_t)(period_ns - (v_v / obj->dc_link_voltage * (period_ns - 1)));
	dcw = (rt_uint32_t)(period_ns - (v_w / obj->dc_link_voltage * (period_ns - 1)));

    rt_pwm_set(obj->pwm0, 1, (rt_uint32_t)period_ns, dcu);
    rt_pwm_set(obj->pwm0, 2, (rt_uint32_t)period_ns, dcv);
    rt_pwm_set(obj->pwm1, 1, (rt_uint32_t)period_ns, dcw);

    rt_pwm_enable(obj->pwm0, 1);
    rt_pwm_enable(obj->pwm0, 2);
    rt_pwm_enable(obj->pwm1, 1);
}

static void set_to_zero(esp_foc_rotor_sensor_t *self)
{
    esp_foc_rtthread_sensor_t *obj =
        rt_container_of(self,esp_foc_rtthread_sensor_t, interface);

    obj->current_count = 0.0f;
}

static float get_counts_per_revolution(esp_foc_rotor_sensor_t *self)
{
    esp_foc_rtthread_sensor_t *obj =
        rt_container_of(self,esp_foc_rtthread_sensor_t, interface);

    return obj->counts_per_revolution;
}

static float read_counts(esp_foc_rotor_sensor_t *self)
{
    esp_foc_rtthread_sensor_t *obj =
        rt_container_of(self,esp_foc_rtthread_sensor_t, interface);
	float simulated = obj->current_count;
	obj->current_count += 10.0f;
    return obj->current_count;
}

static void  delay_ms(esp_foc_rotor_sensor_t *self, int ms)
{
    (void)self;
    rt_thread_mdelay(ms);    
}

void devices_rtthread_init() 
{
	inverter.pwm0 = (struct rt_device_pwm *)rt_device_find("pwm0");
	inverter.pwm1 = (struct rt_device_pwm *)rt_device_find("pwm1");
	inverter.interface.get_dc_link_voltage = get_dc_link_voltage;
	inverter.interface.set_voltages = set_voltages;

	sensor.interface.set_to_zero = set_to_zero;
	sensor.interface.get_counts_per_revolution = get_counts_per_revolution;
	sensor.interface.read_counts = read_counts;
	sensor.counts_per_revolution = 4096.0f;
}

esp_foc_inverter_t *inverter_rtthread_new(void)
{
	return &inverter.interface;
}

esp_foc_rotor_sensor_t *rotor_sensor_rtthread_new(void)
{
	return &sensor.interface;
}
