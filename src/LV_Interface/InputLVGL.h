#ifndef CLOCKSTAR_FIRMWARE_INPUTLVGL_H
#define CLOCKSTAR_FIRMWARE_INPUTLVGL_H

#include <lvgl.h>
#include <map>
#include "Entity/AsyncEntity.h"
#include "Misc/Enum.h"
#include "Services/ButtonInput.h"

class InputLVGL : public Object {
	GENERATED_BODY(InputLVGL, Object)
public:
	InputLVGL() = default;

	InputLVGL(ButtonInput* bi, std::map<Enum<int>, lv_key_t> keyMap, std::map<lv_key_t, lv_key_t> vertRemap = {});
	void postInitProperties() noexcept override;

	void read(lv_indev_t* drv, lv_indev_data_t* data);
	static InputLVGL* getInstance();

	[[nodiscard]] lv_indev_t* getIndev() const;

	void setVertNav(bool vertNav);

private:
	const std::map<Enum<int>, lv_key_t> keyMap;
	const std::map<lv_key_t, lv_key_t> vertRemap;

	ButtonInput* bi;
	lv_indev_t* inputDevice;

	void onButtonInput(Enum<int> btn, ButtonInput::Action btnAction);

	Enum<int> lastKey = 0;
	ButtonInput::Action action = ButtonInput::Action::Release;
	bool vertNav = false;

	static InputLVGL* instance;
};


#endif //CLOCKSTAR_FIRMWARE_INPUTLVGL_H
