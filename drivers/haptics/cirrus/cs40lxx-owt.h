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

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @cond INTERNAL_HIDDEN */


/* Generic waveform header word 1 */
#define CS40LXX_WF_HEADER_WAVEFORM_TYPE_MASK    GENMASK(7, 0)
#define CS40LXX_WF_HEADER_LF0T_MASK            	BIT(16)
#define CS40LXX_WF_HEADER_DVL_MASK             	BIT(17)
#define CS40LXX_WF_HEADER_METADATA_MASK        	BIT(18)
#define CS40LXX_WF_HEADER_DYNAMIC_F0_MASK      	BIT(20)
#define CS40LXX_WF_HEADER_FS_MASK         		GENMASK(22, 21)
#define CS40LXX_WF_HEADER_BUZZ_OR_CLICK_MASK    BIT(23)

/* Generic waveform header word 2 */
#define CS40LXX_WF_HEADER_OFFSET_MASK     		GENMASK(23, 0)

/* Generic waveform header word 3 */
#define CS40LXX_WF_HEADER_LENGTH_MASK     		GENMASK(23, 0)

/* Composite header word 1 */
#define CS40LXX_COMPOSITE_WF_LENGTH_MASK 		GENMASK(23, 0)

/* Composite header word 2 */
#define CS40LXX_COMPOSITE_REPEATS_MASK         	GENMASK(7, 0)
#define CS40LXX_COMPOSITE_NUM_WAVEFORMS_MASK   	GENMASK(15, 8)

/* Composite section word 1 */
#define CS40LXX_COMPOSITE_NESTED_REPEATS_MASK  	GENMASK(7, 0)
#define CS40LXX_COMPOSITE_WAVEFORM_IDX_MASK    	GENMASK(15, 8)
#define CS40LXX_COMPOSITE_AMPLITUDE_MASK       	GENMASK(23, 16)

/* Composite section word 2 */
#define CS40LXX_COMPOSITE_DELAY_MASK           	GENMASK(15, 0)
#define CS40LXX_COMPOSITE_OWT_SUBWAVE          	BIT(21)
#define CS40LXX_COMPOSITE_ROM_SUBWAVE          	BIT(22)
#define CS40LXX_COMPOSITE_DURATION_PRESENT     	BIT(23)

/* Composite section word 3 */
#define CS40LXX_COMPOSITE_DURATION              GENMASK(23, 0)

/* Waveform types */
#define CS40LXX_OWT_TYPE_PCM       0x8U
#define CS40LXX_OWT_TYPE_COMPOSITE 0xAU
#define CS40LXX_OWT_TYPE_PWLE      0xCU

/* OWT specifications */
#define CS40LXX_OWT_HEADER_SIZE                    3U
#define CS40LXX_OWT_METADATA_END_SIZE              1U
#define CS40LXX_OWT_DATA_OFFSET                    (CS40LXX_OWT_HEADER_SIZE + CS40LXX_OWT_METADATA_END_SIZE)
#define CS40LXX_OWT_METADATA_END                   0x00FFFFFFU
#define CS40LXX_OWT_NEXT_FLAG                      BIT(23)
#define CS40LXX_COMPOSITE_HEADER_SIZE              2U
#define CS40LXX_COMPOSITE_SECTION_SIZE             3U
#define CS40LXX_COMPOSITE_SECTION_SIZE_NO_DURATION 2U
#define CS40LXX_COMPOSITE_WF_LENGTH_DEFAULT        0x3FFFFFU

/**
 * @defgroup cs40lxx_owt_interface CS40Lxx OWT
 * @ingroup haptics_interface_ext
 * @brief Open wavetable types for CS40Lxx haptic drivers
 * @{
 */

/**
 * @brief Wavetable containing a composite subwaveform
 */
enum cs40lxx_owt_subwave_source {
	CS40LXX_OWT_SUBWAVE_RAM, /**< Subwaveform is in the RAM wavetable */
	CS40LXX_OWT_SUBWAVE_ROM, /**< Subwaveform is in the ROM wavetable */
	CS40LXX_OWT_SUBWAVE_OWT, /**< Subwaveform is in the open wavetable */
};

/**
 * @brief Generic OWT header definition
 * @details Provide generic OWT header to all OWT waveform pack functions.
 */
struct cs40lxx_owt_header{
	uint8_t feature_bitmap;
	uint8_t waveform_type;
	uint32_t offset;
	uint32_t word_length;
}

/**
 * @brief PCM header definition
 * @details Provide header to @ref cs40l5x_pack_pcm().
 */
struct cs40lxx_pcm_header {
	uint32_t waveform_length;
	uint16_t f0;
	uint16_t scaled_redc;
}

/**
 * @brief PWLE header definition
 * @details Provide array of sections to @ref cs40l5x_pack_pwle().
 */
struct cs40lxx_pwle_header {
	/** Waveform length in samples at 8kHz (0 - 4194303) */
	uint32_t waveform_length;
	/** Wait time after a waveform has finished in 0 - 1023.75 ms (0.25 ms resolution), wait time = stored value * 4 */
	uint16_t wait_time;
	/** Number of repeats of sections (0 - 255). Set to 255 for indefinite playback */
	uint8_t repeats;
	/** Number of sections (2 - 255)  */
	uint8_t num_sections;
}

/**
 * @brief PWLE section definition
 * @details Provide array of sections to @ref cs40l5x_pack_pwle().
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

/**
 * @brief Composite waveform header definition
 * @details Provide a header of an owt composite to an upload function, e.g.,
 * @ref cs40l5x_upload_owt_composite().
 */
struct cs40lxx_owt_composite_header {
	/** Section duration (24 bits), only used if @c duration_present is set */
	uint32_t duration;
	/** Delay following the section (16 bits) */
	uint16_t delay;
	/** Number of times the section is repeated */
	uint8_t nested_repeats;
	/** Index of the subwaveform within the wavetable selected by @c source */
	uint8_t waveform_idx;
	/** Subwaveform amplitude */
	uint8_t amplitude;
	/** Wavetable containing the subwaveform */
	enum cs40lxx_owt_subwave_source source;
	/** The section is followed by a duration word */
	bool duration_present;
};

/**
 * @brief Composite waveform section definition
 * @details Provide an array of sections to the driver's composite upload function, e.g.,
 * @ref cs40l5x_upload_owt_composite().
 */
struct cs40lxx_owt_composite_section {
	/** Section duration (24 bits), only used if @c duration_present is set */
	uint32_t duration;
	/** Delay following the section (16 bits) */
	uint16_t delay;
	/** Number of times the section is repeated */
	uint8_t nested_repeats;
	/** Index of the subwaveform within the wavetable selected by @c source */
	uint8_t waveform_idx;
	/** Subwaveform amplitude */
	uint8_t amplitude;
	/** Wavetable containing the subwaveform */
	enum cs40lxx_owt_subwave_source source;
	/** The section is followed by a duration word */
	bool duration_present;
};

int cs40l5xx_owt_pack_pcm(const struct device *const dev, const int index, const uint16_t redc,
		       const uint16_t f0, const int8_t *const samples, const uint16_t num_samples);

int cs40l5xx_owt_pack_composite(const struct device *const dev, const uint8_t repeats, const uint8_t num_sections, const struct cs40l5xx_owt_composite_section *sections);

int cs40l5xx_owt_pack_pwle(const struct device *const dev, const uint32_t wf_length, const uint8_t repeats, const uint16_t wait_time, const uint8_t num_sections, const struct cs40lxx_owt_pwle_section *sections);

/** @endcond */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CONFIG_HAPTICS_CS40LXX_OWT */
