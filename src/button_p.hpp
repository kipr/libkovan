#ifndef _BUTTON_P_HPP_
#define _BUTTON_P_HPP_

namespace Private
{
	class Button
	{
	public:
		enum ButtonId {
			A,
			B,
			Z
		};
		
		void setText(ButtonId id, const char *text);
		bool isTextDirty(ButtonId id) const;
		const char *text(ButtonId id) const;
		void setPressed(ButtonId id, bool pressed);
		bool isPressed(ButtonId id) const;
		
		static Button *instance();
	private:
		Button();
		Button(const Button& rhs);
	};
}

#endif
