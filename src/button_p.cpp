#include "button_p.hpp"
#include "shm_p.hpp"

#include <cstring>
#include <pthread.h>
#include <cstdio>

using namespace Private;

void Private::Button::setText(::Button::Type::Id id, const char *text)
{
	size_t len = text ? strlen(text) : 0;
	len = len > MAX_BUTTON_TEXT_SIZE ? MAX_BUTTON_TEXT_SIZE : len;
	
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	Private::SharedButton *button = lookup(id, interClient);
	if(!button) return;
	pthread_mutex_lock(&shm->interClientMutex);
	if(text) memcpy(button->text, text, len);
	button->text[len] = 0;
	button->textDirty = true;
	pthread_mutex_unlock(&shm->interClientMutex);
}

bool Private::Button::isTextDirty(::Button::Type::Id id) const
{
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	Private::SharedButton *button = lookup(id, interClient);
	if(!button) return false;
	bool ret = true;
	pthread_mutex_lock(&shm->interClientMutex);
	ret = button->textDirty;
	button->textDirty = false;
	pthread_mutex_unlock(&shm->interClientMutex);
	return ret;
}

const char *Private::Button::text(::Button::Type::Id id) const
{
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	Private::SharedButton *button = lookup(id, interClient);
	if(!button) return 0;
	char *ret = 0;
	pthread_mutex_lock(&shm->interClientMutex);
	ret = button->text;
	pthread_mutex_unlock(&shm->interClientMutex);
	return ret;
}

void Private::Button::setPressed(::Button::Type::Id id, bool pressed)
{
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	Private::SharedButton *button = lookup(id, interClient);
	if(!button) return;
	pthread_mutex_lock(&shm->interClientMutex);
	button->pressed = pressed;
	printf("Setting button %d pressed = %d\n", id, pressed);
	pthread_mutex_unlock(&shm->interClientMutex);
}

bool Private::Button::isPressed(::Button::Type::Id id) const
{
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	Private::SharedButton *button = lookup(id, interClient);
	// printf("Button %d is pressed? %d\n", id, button->pressed);
	return button ? button->pressed : false;
}

void Private::Button::setExtraShown(const bool& shown)
{
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	if(interClient->showExtraButtons == shown) return;
	pthread_mutex_lock(&shm->interClientMutex);
	interClient->showExtraButtons = shown;
	interClient->isShowExtraButtonsDirty = true;
	pthread_mutex_unlock(&shm->interClientMutex);
}

bool Private::Button::isExtraShown() const
{
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	return interClient->showExtraButtons;
}

bool Private::Button::isExtraShownDirty() const
{
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	pthread_mutex_lock(&shm->interClientMutex);
	const bool ret = interClient->isShowExtraButtonsDirty;
	interClient->isShowExtraButtonsDirty = false;
	pthread_mutex_unlock(&shm->interClientMutex);
	return ret;
}

Private::Button *Private::Button::instance()
{
	static Button s_button;
	return &s_button;
}

Private::SharedButton *Private::Button::lookup(const ::Button::Type::Id& id, Private::SharedMemoryInterClient *shm) const
{
	switch(id) {
	case ::Button::Type::A: return &shm->a;
	case ::Button::Type::B: return &shm->b;
	case ::Button::Type::C: return &shm->c;
	case ::Button::Type::X: return &shm->x;
	case ::Button::Type::Y: return &shm->y;
	case ::Button::Type::Z: return &shm->z;
	}
	return 0;
}

Private::Button::Button()
{
	
}

Private::Button::Button(const Button& rhs)
{
	
}