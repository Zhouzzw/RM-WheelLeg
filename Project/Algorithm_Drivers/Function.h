#ifndef __Function__
#define __Function__

#include "main.h"

void Limit_float(float *Input, float Up, float Down);
void Limit_int(int *Input, int Up, int Down);

void Ignor_float(float *Input,  float Up, float Down);

void Deadline_Float(float *Input,  float Up, float Down);

void Deadline_Zero_Float(float *Input,  float DeadLine);

float Caculate_Distence(float X, float Y);

void Limit180(float *data);
void Zero_Crossing(float *Target_Angle,float *Now_Angle);
void Ignor_Int16(int16_t *Input,  int16_t Up, int16_t Down);
float Avarage_Fliter(float *Array,uint8_t Length);
void Update_Data(float *Array,uint8_t Length,float New_Data);
void Acc_Slow(float Target, float *Output, float Acc, float Dec, float Dt);
float Caculate_Included_Angle(float Angle1, float Angle2);

#endif
