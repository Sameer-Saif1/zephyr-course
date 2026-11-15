#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set_level(const struct device *dev, int32_t level);
int our_driver_get_level(const struct device *dev, int32_t *level);

#ifdef __cplusplus
}
#endif

#endif