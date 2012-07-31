#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

#include "sensor.hpp"
#include "button_ids.hpp"

class AbstractButton : public Sensor<bool>
{
public:
	virtual ~AbstractButton();
	virtual void setPressed(bool pressed) = 0;
	inline bool isPressed() const { return value(); };
	inline bool isNotPressed() const { return !isPressed(); }
	inline bool isClicked() const
	{
		const bool ret = isPressed();
		waitUntilReleased();
		return ret;
	}
	virtual void waitUntilReleased() const;
};

class AbstractTextButton : public AbstractButton
{
public:
	virtual ~AbstractTextButton();
	virtual void setText(const char *text) = 0;
	virtual const char *text() const = 0;
	virtual bool isTextDirty() const = 0;
	virtual void resetText() = 0;
};

class IdButton : public AbstractTextButton
{
public:
	IdButton(const Button::Type::Id& id, const char *defaultText);
	~IdButton();
	
	virtual void setText(const char *text);
	virtual const char *text() const;
	virtual bool isTextDirty() const;
	virtual void setPressed(bool pressed);
	virtual bool value() const;
	virtual void resetText();
	
private:
	Button::Type::Id m_id;
	char *m_defaultText;
};

class ExtraButtons
{
public:
	static inline void show() { setShown(true); }
	static inline void hide() { setShown(false); }
	static void setShown(const bool& shown);
	static bool isShown();
	static bool isShownDirty();
};

namespace Button
{
	extern IdButton A;
	extern IdButton B;
	extern IdButton C;
	
	extern IdButton X;
	extern IdButton Y;
	extern IdButton Z;
	
	extern IdButton *const all[6];
}

#endif
