#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <stdint.h>

void setupLEDMatrix();
void writeRow(uint8_t row, int rowNum);
void writeRows(const uint8_t *rows);
void displayRows(uint8_t *rows);
void cartesianToRowValues(int matrix[8][8], uint8_t *rows);

#endif
