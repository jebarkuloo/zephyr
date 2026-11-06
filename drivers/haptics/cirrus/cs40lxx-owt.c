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


int cs40lxx_pack_owt_header(const struct cs40lxx_owt_header *header, uint32_t *bytes)
{
    bytes[0] = FIELD_PREP(CS40LXX_WF_HEADER_BUZZ_OR_CLICK_MASK, header->buzz_or_click) |
            FIELD_PREP(CS40LXX_WF_HEADER_FS_MASK, header->fs) |
            FIELD_PREP(CS40LXX_WF_HEADER_DYNAMIC_F0_MASK, header->dynamic_f0) |
            FIELD_PREP(CS40LXX_WF_HEADER_METADATA_MASK, header->metadata) |
            FIELD_PREP(CS40LXX_WF_HEADER_DVL_MASK, header->dvl) |
            FIELD_PREP(CS40LXX_WF_HEADER_LF0T_MASK, header->lf0t) |
            FIELD_PREP(CS40LXX_WF_HEADER_WAVEFORM_TYPE_MASK, header->waveform_type);
    bytes[1] = FIELD_PREP(CS40LXX_WF_HEADER_OFFSET_MASK, header->offset);
    bytes[2] = FIELD_PREP(CS40LXX_WF_HEADER_LENGTH_MASK, header->word_length);
    return 0;
}

static int cs40lxx_owt_pack_composite_header(const struct cs40lxx_owt_composite_header *header,
				      uint32_t *bytes, uint32_t *len)
{
	bytes[0] = FIELD_PREP(CS40LXX_COMPOSITE_WF_LENGTH_MASK, header->wf_length);
	bytes[1] = FIELD_PREP(CS40LXX_COMPOSITE_NUM_WAVEFORMS_MASK, header->num_waveforms) |
		   FIELD_PREP(CS40LXX_COMPOSITE_REPEATS_MASK, header->repeats);
	*len = CS40LXX_COMPOSITE_HEADER_SIZE;
	return 0;
}

static int cs40lxx_owt_pack_composite_section(const struct cs40lxx_owt_composite_section *section,
				       uint32_t *bytes, uint32_t *len)
{
	bytes[0] = FIELD_PREP(CS40LXX_COMPOSITE_AMPLITUDE_MASK, section->amplitude) |
		   FIELD_PREP(CS40LXX_COMPOSITE_WAVEFORM_IDX_MASK, section->waveform_idx) |
		   FIELD_PREP(CS40LXX_COMPOSITE_NESTED_REPEATS_MASK, section->nested_repeats);

	bytes[1] = FIELD_PREP(CS40LXX_COMPOSITE_DELAY_MASK, section->delay);

	switch (section->source) {
	case CS40LXX_OWT_SUBWAVE_RAM:
		break;
	case CS40LXX_OWT_SUBWAVE_ROM:
		bytes[1] |= CS40LXX_COMPOSITE_ROM_SUBWAVE;
		break;
	case CS40LXX_OWT_SUBWAVE_OWT:
		bytes[1] |= CS40LXX_COMPOSITE_OWT_SUBWAVE;
		break;
	default:
		return -EINVAL;
	}

	if (!section->duration_present) {
		*len = CS40LXX_COMPOSITE_SECTION_SIZE_NO_DURATION;
		return 0;
	}

	bytes[1] |= CS40LXX_COMPOSITE_DURATION_PRESENT;
	bytes[2] = FIELD_PREP(CS40LXX_COMPOSITE_DURATION, section->duration);

	*len = CS40LXX_COMPOSITE_SECTION_SIZE;
	return 0;
}

int cs40lxx_owt_pack_composite(const struct cs40lxx_owt_composite_header *header, const struct cs40lxx_owt_composite_section **sections, uint32_t* bytes, uint32_t* len)
{
    uint32_t idx = 0;
	idx = cs40lxx_owt_pack_composite_header(header, composite_data);
    for(int i = 0, i < header->num_sections; i++)
    {
		uint32_t section_len;
        ret = cs40lxx_owt_pack_composite_section(sections[i], &composite_data[idx], &section_len);
		if (ret < 0) {
			LOG_INST_DBG(config->log, "Error packing OWT composite data");
		}
		idx += section_len;
	}
	*len = idx;
    return 0;
}

#endif /* CONFIG_HAPTICS_CS40LXX_OWT */
