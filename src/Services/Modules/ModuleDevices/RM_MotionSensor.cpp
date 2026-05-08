#include "RM_MotionSensor.h"

#include "Memory/ObjectMemory.h"
#include "Services/Modules/ModuleService.h"

DEFINE_LOG(RM_MotionSensor);

RM_MotionSensor::RM_MotionSensor(const Modules::BusPins& busPins) : Super(Modules::Type::RM_Motion, busPins){
	pin = static_cast<gpio_num_t>(busPins.subAddressPins[0].inputPort);
	active = gpio_get_level(pin);

	sem = xSemaphoreCreateBinary();
	thread = newObject<Threaded>(this, [this]() {
		if(!debouncing){
			xSemaphoreTake(sem, portMAX_DELAY);
			debouncing = true;
			return;
		}

		if(xSemaphoreTake(sem, pdMS_TO_TICKS(DebounceMs)) == pdTRUE){
			CMF_LOG(RM_MotionSensor, Info, "ignored debounce");
			return;
		}

		debouncing = false;
		bool newActive = gpio_get_level(pin);
		if(newActive == active.load()) return;

		active = newActive;

		ModuleService* parent = cast<ModuleService>(getOwner());
		if(parent == nullptr){
			CMF_LOG(RM_MotionSensor, Error, "Parent isn't ModuleService!");
			return;
		}

		parent->OnRM_Motion.broadcast(active.load());
	}, "RM_MotionSensor", 0, 3 * 1024);


	const gpio_config_t io_conf = {
		.pin_bit_mask = 1ULL << pin,
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE,
		.pull_down_en = GPIO_PULLDOWN_ENABLE,
		.intr_type = GPIO_INTR_ANYEDGE
	};

	gpio_intr_enable(pin);
	gpio_isr_handler_add(pin, isr, this);
	gpio_config(&io_conf);

	setLEDs(false);
}

RM_MotionSensor::~RM_MotionSensor() noexcept{
	const auto pin = static_cast<gpio_num_t>(pins.subAddressPins[0].inputPort);
	gpio_isr_handler_remove(pin);

	thread->stop(0);
	xSemaphoreGive(sem);
	thread->stop();
	delete thread.get();

	vSemaphoreDelete(sem);
}

void RM_MotionSensor::setLEDs(bool state){
	if(pins.subAddressPins[1].outputDriver){
		pins.subAddressPins[1].outputDriver->write(pins.subAddressPins[1].outputPort, state);
	}
}

bool RM_MotionSensor::getState() const{
	return active;
}

void IRAM_ATTR RM_MotionSensor::isr(void* arg){
	if(arg == nullptr){
		return;
	}

	RM_MotionSensor* motion = static_cast<RM_MotionSensor*>(arg);
	BaseType_t higherPrioTask = pdFALSE;
	xSemaphoreGiveFromISR(motion->sem, &higherPrioTask);
}
