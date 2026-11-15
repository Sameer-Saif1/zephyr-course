#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include "our_driver.h"

#define HEARTBEAT_NODE DT_ALIAS(app_led)
static const struct gpio_dt_spec heartbeat_led = GPIO_DT_SPEC_GET(HEARTBEAT_NODE, gpios);

int main(void)
{
	const struct device *sensor_dev = DEVICE_DT_GET_ANY(our_driver);

	if (sensor_dev && device_is_ready(sensor_dev)) {
		sensor_sample_fetch(sensor_dev);
		struct sensor_value val;
		sensor_channel_get(sensor_dev, SENSOR_CHAN_ALL, &val);
		printk("Sensor reported state: %d\n", val.val1);
		our_driver_set_level(sensor_dev, 42);
		int32_t lvl = 0;
		our_driver_get_level(sensor_dev, &lvl);
		printk("Level is now: %d\n", (int)lvl);

	}

	if (!gpio_is_ready_dt(&heartbeat_led)) {
		return 0;
	}
	gpio_pin_configure_dt(&heartbeat_led, GPIO_OUTPUT_ACTIVE);

		while (1) {
		gpio_pin_toggle_dt(&heartbeat_led);
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
	}
}