#include "general.h"
#include "shm_p.hpp"

void publish()
{
	Private::SharedMemoryImpl::instance()->publish();
}