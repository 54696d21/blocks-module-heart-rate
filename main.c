/* main.c - (c) 2017 Blocks Wearables Ltd. */
#include "vendor.h"
#include "ppg.h"
#include <stdbool.h>

static float heartRate = 0.f;
static bool dataReady = false;
static bool initRes = true;
static bool running = false;


static blocks_errorcode_t get_rate(uint8_t* rate_out)
{
    *rate_out = PPG_GetRate();
    return ERROR_NONE;
}

static blocks_errorcode_t reset(void)
{
    dataReady = false;
    running = false;
    return PPG_Reset() ? ERROR_NONE : ERROR_HARDWARE;
}

static blocks_errorcode_t set_enabled(bool enabled)
{
    bool result = enabled ? PPG_Enable() : PPG_Disable();
    running = result && enabled;
    dataReady = false;
    return result ? ERROR_NONE : ERROR_HARDWARE;
}

static blocks_errorcode_t get_heart_rate(float* rate)
{
    *rate = heartRate;
    return dataReady ? ERROR_NONE : ERROR_DATA_UNAVAILABLE;
}

void blocks_initializeModule(void)
{
    if (!PPG_Init()) {
        initRes = false;
        blocks_vendorNotify(0xFF); // ModuleError
    }
}

void blocks_main(void)
{
    while (1)
    {
        module_vendor_idle();
        if (!initRes) blocks_vendorNotify(0xADBA);

        if (!running) {
            HAL_Delay(100);
            continue;
        }

        float value = 0.f;
        HAL_Delay(47);
        uint8_t res = PPG_GetHR(&value);
        if (res != 0 && res != 0x22) {/* blocks_vendorNotify(res); */}
        dataReady = res == 0;
        if (dataReady) {
            if(heartRate != value) {
                heartRate = value;
                //blocks_vendorNotify(heartRate);
            }
        }
    }
}

const vendor_module_info_t blocks_module_info = {
    .label    = u"Heart Rate Module",
    .vendorID = u"Blocks Wearables Ltd.",
    .modelID  = {0x12, 0x34, 0x56, 0x78}
};

const vendor_array_handler_t blocks_module_functions = { .count = 4, {
    {FUNC_PPG_SET_ENABLED,     (blocks_standard_function)set_enabled},
    {FUNC_PPG_GET_HEARTRATE,   (blocks_standard_function)get_heart_rate},
    {FUNC_PPG_GET_RATE,        (blocks_standard_function)get_rate},
    {FUNC_PPG_RESET,           (blocks_standard_function)reset},
    {FUNC_PPG_GET_HEARTRATERAW, 0 /* TODO */}
}};
