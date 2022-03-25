#ifndef __MOVEP_H
#define __MOVEP_H

#include <inttypes.h>

void movep_plot_pixel(uint8_t *destination, uint32_t mask, uint32_t colour);
void movep_restore_pixel(uint8_t *source, uint8_t *destination);

#endif
