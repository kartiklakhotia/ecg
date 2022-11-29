#ifndef _config_h___
#define _config_h___

typedef struct __Config {

	uint8_t band_pass_filter_order;
	int32_t band_pass_filter_coefficients[128];

	uint8_t moving_average_filter_order;

	REAL hF[NHERMITE * MEM_SIZE_PER_HF];	
} Config;

void sendConfigHF(REAL* hf);
void sendConfigurationToHardware(Config* cfg);
// return 0 if OK.
int verifyConfigurationFromHardware(Config* cfg);

#endif

