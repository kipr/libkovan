/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

VH #include "kovan/button.h"
VH #include "kovan/button.hpp"

using namespace Button;

VI void set_a_button_text(const char *text)
{
	A.setText(text);
}

VI void set_b_button_text(const char *text)
{
	B.setText(text);
}

VI void set_c_button_text(const char *text)
{
	C.setText(text);
}

VI void set_x_button_text(const char *text)
{
	X.setText(text);
}

VI void set_y_button_text(const char *text)
{
	Y.setText(text);
}

VI void set_z_button_text(const char *text)
{
	Z.setText(text);
}

int button(AbstractButton *button)
{
	return button->isPressed() ? 1 : 0;
}

VI int a_button()
{
	return button(&A);
}

VI int b_button()
{
	return button(&B);
}

VI int c_button()
{
	return button(&C);
}

VI int x_button()
{
	return button(&X);
}

VI int y_button()
{
	return button(&Y);
}

VI int z_button()
{
	return button(&Z);
}

VI int side_button()
{
	return button(&Side);
}

VI int black_button()
{
	return side_button();
}

int button_clicked(AbstractButton *button)
{
	return button->isClicked() ? 1 : 0;
}

VI int a_button_clicked()
{
	return button_clicked(&A);
}

VI int b_button_clicked()
{
	return button_clicked(&B);
}

VI int c_button_clicked()
{
	return button_clicked(&C);
}

VI int x_button_clicked()
{
	return button_clicked(&X);
}

VI int y_button_clicked()
{
	return button_clicked(&Y);
}

VI int z_button_clicked()
{
	return button_clicked(&Z);
}

VI int side_button_clicked()
{
	return button_clicked(&Side);
}

VI int any_button()
{
	for(unsigned char i = 0; i < 7; ++i) if(all[i]->isPressed()) return 1;
	return 0;
}

VI void extra_buttons_show()
{
	set_extra_buttons_visible(1);
}

VI void extra_buttons_hide()
{
	set_extra_buttons_visible(0);
}

VI int get_extra_buttons_visible()
{
	return ExtraButtons::isShown() ? 1 : 0;
}

VI void set_extra_buttons_visible(int visible)
{
	ExtraButtons::setShown(visible == 0 ? false : true);
}