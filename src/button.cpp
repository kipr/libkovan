#include "button.hpp"
#include "button_p.hpp"
#include <cstring>

AbstractButton::~AbstractButton() {}

void AButton::setText(const char *text)
{
	Private::Button::instance()->setText(Private::Button::A, text);
}

const char *AButton::text() const
{
	return Private::Button::instance()->text(Private::Button::A);
}

bool AButton::isTextDirty() const
{
	return Private::Button::instance()->isTextDirty(Private::Button::A);
}

void AButton::setPressed(bool pressed)
{
	Private::Button::instance()->setPressed(Private::Button::A, pressed);
}

bool AButton::isPressed() const
{
	return false;
}

void BButton::setText(const char *text)
{
	Private::Button::instance()->setText(Private::Button::B, text);
}

const char *BButton::text() const
{
	return Private::Button::instance()->text(Private::Button::B);
}

bool BButton::isTextDirty() const
{
	return Private::Button::instance()->isTextDirty(Private::Button::B);
}

void BButton::setPressed(bool pressed)
{
	Private::Button::instance()->setPressed(Private::Button::B, pressed);
}

bool BButton::isPressed() const
{
	return false;
}

void ZButton::setText(const char *text)
{
	Private::Button::instance()->setText(Private::Button::Z, text);
}

const char *ZButton::text() const
{
	return Private::Button::instance()->text(Private::Button::Z);
}

bool ZButton::isTextDirty() const
{
	return Private::Button::instance()->isTextDirty(Private::Button::Z);
}

void ZButton::setPressed(bool pressed)
{
	Private::Button::instance()->setPressed(Private::Button::Z, pressed);
}

bool ZButton::isPressed() const
{
	return false;
}
