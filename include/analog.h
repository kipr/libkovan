#ifndef _ANALOG_H_
#define _ANALOG_H_

#ifdef __cplusplus
extern "C" {
#endif

int analog10(int port);
int analog(int port);

void analog_set_pullup(int port, int pullup);
int analog_get_pullup(int port);

unsigned short backemf(int port);

#ifdef __cplusplus
}
#endif

#endif
