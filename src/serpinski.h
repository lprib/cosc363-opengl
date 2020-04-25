#ifndef SERPINSKI_H
#define SERPINSKI_H

void serpinski_init();
void serpinski_draw();
void serpinski_update(double delta);
void serpinski_keyboard_func(unsigned char key, int x, int y);

typedef struct {
    double x;
    double y;
    double z;
} Point_t;

typedef struct {
    Point_t from_points[4];
    Point_t to_points[4];
    double normalize_bottom;
    double normalize_top;
} Tetra_t;

#endif