#include "button_p.hpp"
#include "shm_p.hpp"

#include <cstring>
#include <pthread.h>

using namespace Private;

void Button::setText(ButtonId id, const char *text)
{
	size_t len = strlen(text);
	len = len > 16 ? 16 : len;
	
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	pthread_mutex_lock(&shm->interClientMutex);
	switch(id) {
	case A:
		memcpy(interClient->a.text, text, len);
		interClient->a.textDirty = true;
		break;
	case B:
		memcpy(interClient->b.text, text, len);
		interClient->b.textDirty = true;
		break;
	case Z:
		memcpy(interClient->z.text, text, len);
		interClient->z.textDirty = true;
		break;
	}
	pthread_mutex_unlock(&shm->interClientMutex);
}

bool Button::isTextDirty(ButtonId id) const
{
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	pthread_mutex_lock(&shm->interClientMutex);
	bool ret = false;
	switch(id) {
	case A: ret = interClient->a.textDirty;
		interClient->a.textDirty = false;
		break;
	case B: ret = interClient->b.textDirty;
		interClient->b.textDirty = false;
		break;
	case Z: ret = interClient->z.textDirty;
		interClient->z.textDirty = false;
		break;
	}
	pthread_mutex_unlock(&shm->interClientMutex);
	return ret;
}

const char *Button::text(ButtonId id) const
{
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	pthread_mutex_lock(&shm->interClientMutex);
	char *ret = 0;
	switch(id) {
	case A: ret = interClient->a.text;
		break;
	case B: ret = interClient->b.text;
		break;
	case Z: ret = interClient->z.text;
		break;
	}
	pthread_mutex_unlock(&shm->interClientMutex);
	return ret;
}

void Button::setPressed(ButtonId id, bool pressed)
{
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	pthread_mutex_lock(&shm->interClientMutex);
	switch(id) {
	case A: interClient->a.pressed = true;
	case B: interClient->b.pressed = true;
	case Z: interClient->z.pressed = true;
	}
	pthread_mutex_unlock(&shm->interClientMutex);
}

bool Button::isPressed(ButtonId id) const
{
	SharedMemoryInterClient *interClient = SharedMemoryImpl::instance()->sharedMemoryInterClient();
	switch(id) {
	case A: return interClient->a.pressed;
	case B: return interClient->b.pressed;
	case Z: return interClient->z.pressed;
	}
	
	return false;
}

Button *Button::instance()
{
	static Button s_button;
	return &s_button;
}

Button::Button()
{
	
}

Button::Button(const Button& rhs)
{
	
}