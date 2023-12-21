#include <stdio.h>
#include <stddef.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#include "LEDMatrix.h"

#define DATAPIN 0
#define LATCHPIN 2
#define CLOCKPIN 3

void setupLEDMatrix()
{
    wiringPiSetup();
    pinMode(DATAPIN, OUTPUT);
    pinMode(CLOCKPIN, OUTPUT);
    pinMode(LATCHPIN, OUTPUT);

    digitalWrite(DATAPIN, LOW);
    digitalWrite(DATAPIN, LOW);
    digitalWrite(DATAPIN, LOW);
}

static void _shiftOut(int dPin,int cPin,int order,uint8_t val){
	int i;
    for(i = 0; i < 8; i++){
        digitalWrite(cPin,LOW);
        if(order == LSBFIRST){
            digitalWrite(dPin,((0x01&(val>>i)) == 0x01) ? HIGH : LOW);
            delayMicroseconds(10);
		}
        else {//if(order == MSBFIRST){
            digitalWrite(dPin,((0x80&(val<<i)) == 0x80) ? HIGH : LOW);
            delayMicroseconds(10);
		}
        digitalWrite(cPin,HIGH);
        delayMicroseconds(10);
	}
}

void writeRow(uint8_t row, int rowNum)
{
    digitalWrite(LATCHPIN, LOW);
    _shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, row);
    _shiftOut(DATAPIN, CLOCKPIN, LSBFIRST, (uint8_t)~(1 << rowNum));
    digitalWrite(LATCHPIN, HIGH);
}

void writeRows(const uint8_t *rows)
{
    for(int i = 0; i < 8; i++)
    {
        writeRow(rows[i], i);
        delay(1);
    }
}

void displayRows(uint8_t *rows)
{
    for(int i = 0; i < 100; i++)
    {
        for(int i = 0; i < 8; i++)
        {
            writeRow(rows[i], i);
            delay(1);
        }
    }
}

void cartesianToRowValues(int matrix[8][8], uint8_t *rows)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(matrix[i][j] == 1)
            {
                rows[i] |= (1 << j);
            }
            else if(matrix[i][j] == 0)
            {
                rows[i] &= ~(1 << j);
            }
        }
    }
}
