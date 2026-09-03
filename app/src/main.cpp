#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define HEARTBEAT_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec heartbeat_led = GPIO_DT_SPEC_GET(HEARTBEAT_NODE, gpios);

int main(void)
{
	if (!gpio_is_ready_dt(&heartbeat_led)) {
		return 0;
	}

	gpio_pin_configure_dt(&heartbeat_led, GPIO_OUTPUT_ACTIVE);

	while (1) {
		gpio_pin_toggle_dt(&heartbeat_led);
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
	}
}