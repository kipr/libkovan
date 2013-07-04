#include "kovan/botball.h"

#include "kovan/button.h"
#include "kovan/util.h"
#include "kovan/display.h"
#include "kovan/analog.h"
#include "kovan/audio.h"
#include "kovan/thread.hpp"
#include "kovan/general.h"
#include "kovan/create.hpp"

#include <cstdlib>
#include <iostream>

class ShutDownIn : public Thread
{
public:
	ShutDownIn(double s)
		: m_s(s)
	{
	}
	
	~ShutDownIn()
	{
	}
	
	virtual void run()
	{
		const double start = seconds();
		msleep(m_s * 1000.0);
		const double end = seconds();
		std::cout << std::endl << "Shutdown after " << (end - start) << " seconds" << std::endl;
		// Note: Might want to move this to botui in the future.
		Create::instance()->stop();
		_Exit(0);
	}
	
private:
	double m_s;
};

void shut_down_in(double s)
{
	static ShutDownIn *s_instance;
	if(s_instance) {
		std::cout << "Warning: shut_down_in already called once. Ignoring." << std::endl;
		return;
	}
	s_instance = new ShutDownIn(s);
	s_instance->start();
}

void wait_for_light(int light_port_)
{
	int xBut, l_on_, l_off_, l_mid_, t, OK = 0;
	float s;
	xBut = get_extra_buttons_visible();
	set_extra_buttons_visible(0);
	set_a_button_text("-");
	set_c_button_text("-");
	set_analog_pullup(light_port_, 1);
	while (!OK) {
		set_b_button_text("Light is On");
		display_clear();
		display_printf (0, 0, "CALIBRATE: sensor port #%d", light_port_);
		display_printf(0, 1, "   press button when light is on");
		while(b_button_clicked() == 0) {
			l_on_ = analog10 (light_port_);
			display_printf(0,1,"   light on value is = %d        ", l_on_);
			msleep(50);
		}
		l_on_ = analog10(light_port_); /* sensor value when light is on */

		set_b_button_text("Light is Off");

		display_printf(0,1,"   light on value is = %d        ", l_on_);
		msleep(200);
		beep();

		display_printf(0,2,"   press button when light off");
		while(b_button_clicked() == 0) {
			l_off_ = analog10(light_port_);
			display_printf(0,3,"   light off value is = %d         ", l_off_);
			msleep(50);
		}
		l_off_ = analog10(light_port_); /* sensor value when light is off */

		display_printf(0,3,"   light off value is = %d         ", l_off_);
		msleep(200);

		if((l_off_ - l_on_) >= 60) { /* bright = small values */
			OK = 1;
			l_mid_ = (l_on_ + l_off_) / 2;
			display_printf(0, 5, "Good Calibration!");
			display_printf(0, 7, "Diff = %d:  WAITING", l_off_ - l_on_);
			while(analog10(light_port_) > l_mid_);
		} else {
			s = seconds();
			display_printf(0,7,"BAD CALIBRATION");
			if(l_off_ < 512){
				display_printf(0,8,"   Add Shielding!!");
				msleep(5000);
			} else {
				display_printf(0,8,"   Aim sensor!!");
				msleep(5000);
			}
		}
	}
	set_extra_buttons_visible(xBut);
	set_a_button_text("A");
	set_b_button_text("B");
	set_c_button_text("C");
}