#ifndef JULIA_H
#define JULIA_H

/** This module draws a surface whose height is based on the value of a certain julia set.
 * The complex addition component c (used in f(x) = z^2 + c) is constantly varied using sine/cosing harmonics
 */

// Update the surface using a frame delta time
void julia_update(double delta);
// Draw the surface
void julia_draw();


#endif