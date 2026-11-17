#include <stdlib.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include "our_driver.h"

static const struct device *get_sensor_dev(void)
{
	return DEVICE_DT_GET_ANY(our_driver);
}

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = get_sensor_dev();

	if (!dev || !device_is_ready(dev)) {
		shell_error(sh, "Sensor device not ready");
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(dev);

	if (ret < 0) {
		shell_error(sh, "sample_fetch failed: %d", ret);
		return ret;
	}

	shell_print(sh, "Sample fetched (LED ON)");
	return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = get_sensor_dev();

	if (!dev || !device_is_ready(dev)) {
		shell_error(sh, "Sensor device not ready");
		return -ENODEV;
	}

	struct sensor_value val;
	int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);

	if (ret < 0) {
		shell_error(sh, "channel_get failed: %d", ret);
		return ret;
	}

	shell_print(sh, "Channel value: %d (LED now OFF)", val.val1);
	return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = get_sensor_dev();

	if (!dev) {
		shell_error(sh, "Sensor device not found");
		return -ENODEV;
	}

	shell_print(sh, "Device: %s", dev->name);
	shell_print(sh, "Ready: %s", device_is_ready(dev) ? "yes" : "no");
	return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);

	const struct device *dev = get_sensor_dev();

	if (!dev || !device_is_ready(dev)) {
		shell_error(sh, "Sensor device not ready");
		return -ENODEV;
	}

	char *endptr;
	long value = strtol(argv[1], &endptr, 10);

	if (*endptr != '\0' || value < 0 || value > 100) {
		shell_error(sh, "Invalid value '%s': must be an integer 0-100", argv[1]);
		return -EINVAL;
	}

	int ret = our_driver_set_level(dev, (int32_t)value);

	if (ret < 0) {
		shell_error(sh, "Failed to set level: %d", ret);
		return ret;
	}

	shell_print(sh, "Level set to %ld", value);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
	SHELL_CMD(fetch, NULL, "Fetch a sample (turns LED ON)", cmd_sensor_fetch),
	SHELL_CMD(read, NULL, "Read last channel value (turns LED OFF)", cmd_sensor_read),
	SHELL_CMD(info, NULL, "Show device name and ready state", cmd_sensor_info),
	SHELL_CMD_ARG(set, NULL, "Set level: sensor set <0-100>", cmd_sensor_set, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Custom sensor driver commands", NULL);