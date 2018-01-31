/* main.c - (c) 2017 Blocks Wearables Ltd. */
#include "vendor.h"
#include "ppg.h"
#include <stdbool.h>

static float heartRate = 0.f;
static bool dataReady = false;
static bool initRes = true;
static bool running = false;


static blocks_errorcode_t get_rate(uint8_t* rate_out) {
	*rate_out = ppg_getRate();
	return BLOCKS_ERROR_NONE;
}

static blocks_errorcode_t reset(void) {
	dataReady = false;
	running = false;
	return ppg_reset() ? BLOCKS_ERROR_NONE : BLOCKS_ERROR_HARDWARE;
}

static blocks_errorcode_t set_enabled(bool enabled) {
	bool result = enabled ? ppg_enable() : ppg_disable();
	running = result && enabled;
	dataReady = false;
	return result ? BLOCKS_ERROR_NONE : BLOCKS_ERROR_HARDWARE;
}

static blocks_errorcode_t get_heart_rate(float* rate) {
	*rate = heartRate;
	return dataReady ? BLOCKS_ERROR_NONE : BLOCKS_ERROR_DATA_UNAVAILABLE;
}

void blocks_initializeModule(void) {
	if (!ppg_init()) {
		initRes = false;
		blocks_vendorNotify(NOTIFY_MODULE_ERROR);
	}
}

void blocks_main(void) {
	while (1)
	{
		if (!running) {
			module_vendor_idle();
			continue;
		}

		dataReady = ppg_run(&heartRate);
	}
}

const vendor_module_info_t blocks_module_info = {
	.label	= u"Heart Rate Module",
	.vendorID = u"Blocks Wearables Ltd.",
	.modelID  = {0x12, 0x34, 0x56, 0x78}
};

const vendor_array_handler_t blocks_module_functions = { .count = 4, {
	{STDFUNC_PPG_SET_ENABLED,		(blocks_standard_function)set_enabled},
	{STDFUNC_PPG_GET_HEARTRATE,		(blocks_standard_function)get_heart_rate},
	{STDFUNC_PPG_GET_RATE,			(blocks_standard_function)get_rate},
	{STDFUNC_PPG_RESET,				(blocks_standard_function)reset},
	{STDFUNC_PPG_GET_HEARTRATERAW, 0 /* TODO */}
}};
