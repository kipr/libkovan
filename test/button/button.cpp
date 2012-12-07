#include <kovan/kovan.hpp>

int main(int argc, char *argv[])
{
	Button::A.setPressed(0);
	ExtraButtons::show();
	Button::A.setText("Left");
	Button::B.setText("Right");
	Button::Z.setText("Enter");
}