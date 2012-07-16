#ifndef _DRAW_H_
#define _DRAW_H_

#ifdef __cplusplus
extern "C" {
#endif

int draw_open();
void draw_point(int x, int y);
void draw_line(int sx, int sy, int ex, int ey);
void draw_clear();
void draw_close();

#ifdef __cplusplus
}
#endif


#endif
