#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

void a_button_set_text(const char *text);
void b_button_set_text(const char *text);
void c_button_set_text(const char *text);
void x_button_set_text(const char *text);
void y_button_set_text(const char *text);
void z_button_set_text(const char *text);

int a_button();
int b_button();
int c_button();
int x_button();
int y_button();
int z_button();

int a_button_pressed();
int b_button_pressed();
int c_button_pressed();
int x_button_pressed();
int y_button_pressed();
int z_button_pressed();

int any_button();

void extra_buttons_show();
void extra_buttons_hide();
int extra_buttons_get_visible();
void extra_buttons_set_visible(int visible);

#ifdef __cplusplus
}
#endif

#endif
