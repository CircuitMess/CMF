#ifndef CMF_LVGL_H
#define CMF_LVGL_H

#include <lvgl.h>
#include "LVScreen.h"
#include "Devices/Display.h"
#include "Entity/AsyncEntity.h"

class LVGL : public AsyncEntity {
	GENERATED_BODY(LVGL, AsyncEntity, void)

public:
	LVGL() = default;
	LVGL(Display* display, std::function<lv_theme_t*(lv_disp_t*)> themeInit = {});

	void startScreen(std::function<std::unique_ptr<LVScreen>()> create, lv_screen_load_anim_t anim = LV_SCR_LOAD_ANIM_NONE);

	/** startScreen should be called immediately after this function. */
	void stopScreen();

	void resume();

	/** Draw .bin image to display */
	static void drawImage(const char* src);

	void resetDisplayRefreshTimer();

private:
	Display* display;

	lv_disp_t* lvDisplay;

	void tick(float deltaTime) noexcept override;

	static void flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);

	std::unique_ptr<LVScreen> currentScreen;
};

#endif //CMF_LVGL_H
