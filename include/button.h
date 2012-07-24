#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

void a_button_set_text(const char *text);
void b_button_set_text(const char *text);
void z_button_set_text(const char *text);

int a_button();
int b_button();
int z_button();

#ifdef __cplusplus
}
#endif

#endif
