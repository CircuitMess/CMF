#ifndef CMF_RM_IRMODULE_H
#define CMF_RM_IRMODULE_H

#include "Object/Class.h"
#include "Services/Modules/ModuleDevice.h"
#include <driver/gpio.h>
#include <hal/rmt_types.h>
#include <cstdint>

static constexpr uint16_t IRMaxSymbols = 128;
static constexpr uint16_t RMTMemBlockSymbols = 64; // 2 HW blocks × 48 words, fits most protocols

struct IRSignal {
    rmt_symbol_word_t symbols[IRMaxSymbols];
    uint16_t count = 0;
};

class RM_IRModule : public ModuleDevice {
	GENERATED_BODY(RM_IRModule, ModuleDevice, CONSTRUCTOR_PACK(const Modules::BusPins&))
public:
	RM_IRModule(const Modules::BusPins& busPins = {});

	void write(bool state);
	bool read();

	gpio_num_t getInputGPIO() const;
	gpio_num_t getOutputGPIO() const;

	IRSignal captureSignal(uint32_t timeoutMs);
	void replaySignal(const rmt_symbol_word_t* symbols, uint16_t count, uint32_t minDurationMs);
};


#endif //CMF_RM_IRMODULE_H
