#ifndef SKINNING_LIB_H
#define SKINNING_LIB_H
// constants used for circle/arc computation
#define SIN45  46341  // sin(45) * 2^16)
#define ONEP25 81920  // 1.25 * 2^16
int Arc(int xL, int yT, int xR, int yB, int r1, int r2, int octant);
void Bevel(int x1, int y1, int x2, int y2, int rad);
int FillBevel(int x1, int y1, int x2, int y2, int rad);
#endif
