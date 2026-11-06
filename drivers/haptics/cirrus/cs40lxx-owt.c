/*
 * Copyright (c) 2026 Cirrus Logic, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Open Wave Table (OWT) functions for Cirrus Logic haptic drivers
 */

#ifdef CONFIG_HAPTICS_CS40LXX_OWT
#include "cs40lxx.h"
#include "cs40lxx-owt.h"

static int cs40lxx_pack_pcm_header

int cs40lxx_owt_pack_pcm(const struct device *const dev, const int index, 
    const uint16_t redc, const uint16_t f0, 
    const int8_t *const samples, const uint16_t num_samples)
{

}