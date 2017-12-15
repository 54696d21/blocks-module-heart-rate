#include "vendor.h"

static blocks_errorcode_t set_enabled (bool enabled) {
	return ERROR_NONE;
}

static blocks_errorcode_t get_heart_rate (uint8_t* rate) {
	return ERROR_NONE;
}

void blocks_initializeModule (void) {
	/* Enter your hardware initialisation code here, e.g. GPIO setup */
}

void blocks_main (void) {
	/* Enter your main run loop implementation here */
	while (1) {
		module_vendor_idle();
	}
}

const vendor_module_info_t blocks_module_info = { 
	.label    = u"Heart Rate Module - PPG", 
	.vendorID = u"Blocks Wearables Ltd.", 
	.modelID  = {0x12, 0x34, 0x56, 0x78} 
};
	
const vendor_array_handler_t blocks_module_functions = { .count = 0, {
	{FUNC_PPG_SET_ENABLED,     (blocks_standard_function)set_enabled},
	{FUNC_PPG_GET_HEARTRATE,   (blocks_standard_function)get_heart_rate},
	{FUNC_PPG_GET_HEARTRATERAW, 0 /* TODO */},
	{FUNC_PPG_GET_RATE,         0 /* TODO */},
	{FUNC_PPG_RESET,            0 /* TODO */}
}};
