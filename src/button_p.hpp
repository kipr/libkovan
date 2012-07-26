#ifndef _BUTTON_P_HPP_
#define _BUTTON_P_HPP_

#include "button_ids.hpp"
#include "private.hpp"

namespace Private
{
	class Button
	{
	public:
		
		
		void setText(::Button::Type::Id id, const char *text);
		bool isTextDirty(::Button::Type::Id id) const;
		const char *text(::Button::Type::Id id) const;
		void setPressed(::Button::Type::Id id, bool pressed);
		bool isPressed(::Button::Type::Id id) const;
		
		void setExtraShown(const bool& shown);
		bool isExtraShown() const;
		bool isExtraShownDirty() const;
		
		static Button *instance();
	private:
		Private::SharedButton *lookup(const ::Button::Type::Id& id, Private::SharedMemoryInterClient *shm) const;
		
		Button();
		Button(const Button& rhs);
	};
}

#endif
