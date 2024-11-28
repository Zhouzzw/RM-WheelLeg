#ifndef __PID__
#define __PID__

#include "main.h"


/*===| PID结构体定义 |===*/
typedef struct
{
    uint32_t DWT_Count;
    
    float Kp;
    float Ki;
    float Kd;
    float Kf;
    
    float Target;
    float Target_Last;
    float Feedback;
    
    float Error;
    float Error_Last;   
    float Error_Last_Last;     
    
    float K_Output;
    float I_Output;
    float D_Output;
    float F_Output;
    
    float Output;
    
    float I_Output_Max;
    float Output_Max;
} PID_Struct_TypeDef;

void PID_Init(PID_Struct_TypeDef *PID_Struct, float Kp, float Ki, float Kd, float Kf, float I_Output_Max, float Output_Max);

void PID_Position_Calculate(PID_Struct_TypeDef *PID_Struct, float Target, float Feedback);
void PID_Incress_Calculate(PID_Struct_TypeDef *PID_Struct, float Target, float Feedback);




#endif
