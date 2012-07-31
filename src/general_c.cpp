#include "general.h"
#include "shm_p.hpp"

void set_auto_publish(int on)
{
	Private::SharedMemoryImpl::instance()->setAutoPublish(on == 0 ? false : true);
}

void publish()
{
	Private::SharedMemoryImpl::instance()->publish();
}