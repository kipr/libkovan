#include "button.hpp"
#include "button_p.hpp"

#include <cstring>
#include <sched.h>

IdButton Button::A(Button::Type::A, "A");
IdButton Button::B(Button::Type::B, "B");
IdButton Button::C(Button::Type::C, "C");

IdButton Button::X(Button::Type::X, "X");
IdButton Button::Y(Button::Type::Y, "Y");
IdButton Button::Z(Button::Type::Z, "Z");

IdButton * const Button::all[6] = {
	&Button::A, &Button::B, &Button::C,
	&Button::X, &Button::Y, &Button::Z
};

AbstractButton::~AbstractButton() {}

void AbstractButton::waitUntilReleased() const
{
	while(isPressed()) sched_yield();
}

AbstractTextButton::~AbstractTextButton() {}

IdButton::IdButton(const Button::Type::Id& id, const char *defaultText)
	: m_id(id)
{
	size_t len = strlen(defaultText);
	m_defaultText = new char[len + 1];
	memcpy(m_defaultText, defaultText, len);
	m_defaultText[len] = 0;
}

IdButton::~IdButton()
{
	if(m_defaultText) delete[] m_defaultText;
}

void IdButton::setText(const char *text)
{
	Private::Button::instance()->setText(m_id, text);
}

const char *IdButton::text() const
{
	return Private::Button::instance()->text(m_id);
}

bool IdButton::isTextDirty() const
{
	return Private::Button::instance()->isTextDirty(m_id);
}

void IdButton::setPressed(bool pressed)
{
	Private::Button::instance()->setPressed(m_id, pressed);
}

bool IdButton::value() const
{
	return Private::Button::instance()->isPressed(m_id);
}

void IdButton::resetText()
{
	setText(m_defaultText);
}

void ExtraButtons::setShown(const bool& shown)
{
	Private::Button::instance()->setExtraShown(shown);
}

bool ExtraButtons::isShown()
{
	return Private::Button::instance()->isExtraShown();
}

bool ExtraButtons::isShownDirty()
{
	return Private::Button::instance()->isExtraShownDirty();
}