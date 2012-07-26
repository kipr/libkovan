#include "button.h"
#include "button.hpp"

using namespace Button;

void a_button_set_text(const char *text)
{
	A.setText(text);
}

void b_button_set_text(const char *text)
{
	B.setText(text);
}

void c_button_set_text(const char *text)
{
	C.setText(text);
}

void x_button_set_text(const char *text)
{
	X.setText(text);
}

void y_button_set_text(const char *text)
{
	Y.setText(text);
}

void z_button_set_text(const char *text)
{
	Z.setText(text);
}

int button(AbstractButton *button)
{
	return button->isClicked() ? 1 : 0;
}

int a_button()
{
	return button(&A);
}

int b_button()
{
	return button(&B);
}

int c_button()
{
	return button(&C);
}

int x_button()
{
	return button(&X);
}

int y_button()
{
	return button(&Y);
}

int z_button()
{
	return button(&Z);
}

int button_pressed(AbstractButton *button)
{
	return button->isPressed() ? 1 : 0;
}

int a_button_pressed()
{
	return button_pressed(&A);
}

int b_button_pressed()
{
	return button_pressed(&B);
}

int c_button_pressed()
{
	return button_pressed(&C);
}

int x_button_pressed()
{
	return button_pressed(&X);
}

int y_button_pressed()
{
	return button_pressed(&Y);
}

int z_button_pressed()
{
	return button_pressed(&Z);
}

int any_button()
{
	for(unsigned char i = 0; i < 6; ++i) if(all[i]->isPressed()) return 1;
	return 0;
}

void extra_buttons_show()
{
	extra_buttons_set_visible(1);
}

void extra_buttons_hide()
{
	extra_buttons_set_visible(0);
}

int extra_buttons_get_visible()
{
	return ExtraButtons::isShown() ? 1 : 0;
}

void extra_buttons_set_visible(int visible)
{
	ExtraButtons::setShown(visible == 0 ? false : true);
}