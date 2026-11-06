/*
 * Copyright (c) 2026 Cirrus Logic, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Shared types for Cirrus Logic CS40Lxx haptic drivers
 * @ingroup cs40lxx_interface
 */

#ifndef ZEPHYR_INCLUDE_DRIVERS_HAPTICS_CS40LXX_H_
#define ZEPHYR_INCLUDE_DRIVERS_HAPTICS_CS40LXX_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup cs40lxx_interface CS40Lxx
 * @ingroup haptics_interface_ext
 * @brief Shared types for CS40Lxx haptic drivers
 * @{
 */

/**
 * @brief PWLE section definition
 * @details Provide array of sections to @ref cs40l5x_upload_pwle().
 */
struct cs40lxx_pwle_section {
	/** Section duration in unsigned Q14.2 format (time) or Q16.0 format (half-cycles) */
	uint16_t duration;
	/** Section frequency in unsigned Q10.2 format */
	uint16_t frequency;
	/** Section level in unsigned Q0.11 format */
	uint16_t level;
	/** Section flags in unsigned Q4.0 format */
	uint8_t flags;
};

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ZEPHYR_INCLUDE_DRIVERS_HAPTICS_CS40LXX_H_ */
