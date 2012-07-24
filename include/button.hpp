#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

class AbstractButton
{
public:
	virtual ~AbstractButton();
	virtual void setText(const char *text) = 0;
	virtual const char *text() const = 0;
	virtual bool isTextDirty() const = 0;
	virtual void setPressed(bool pressed) = 0;
	virtual bool isPressed() const = 0;
	inline bool isNotPressed() const { return !isPressed(); }
};

class AButton : public AbstractButton
{
public:
	virtual void setText(const char *text);
	virtual const char *text() const;
	virtual bool isTextDirty() const;
	virtual void setPressed(bool pressed);
	virtual bool isPressed() const;
};

class BButton : public AbstractButton
{
public:
	virtual void setText(const char *text);
	virtual const char *text() const;
	virtual bool isTextDirty() const;
	virtual void setPressed(bool pressed);
	virtual bool isPressed() const;
};

class ZButton : public AbstractButton
{
public:
	virtual void setText(const char *text);
	virtual const char *text() const;
	virtual bool isTextDirty() const;
	virtual void setPressed(bool pressed);
	virtual bool isPressed() const;
};

#endif
