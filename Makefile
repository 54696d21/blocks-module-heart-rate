BUILD_DIR  ?= build/
TARGET     ?= STM32L4X6
VERSION    ?= 2

STM_DIR     = lib/stm/
BLOCKS_DIR  = lib/blocks/

include $(STM_DIR)init.mk
include $(BLOCKS_DIR)init.mk

CFILES += main.c ppg.c pah8001/pah8001.c kxtj2/kxtj2.c
INCLUDE_PATHS += .
STATIC_LIBS += lib/pixart/libpaw8001motion3603641\#0_m4f_soft_gcc.a

include $(STM_DIR)stm.mk
include $(BLOCKS_DIR)blocks.mk
