//
// Created by RM UI Designer
// Static Edition
//

#ifndef UI_g_H
#define UI_g_H

#include "ui_interface.h"

extern ui_interface_rect_t *ui_g_Dynamic_Aim;
extern ui_interface_arc_t *ui_g_Dynamic_Pitch;
extern ui_interface_line_t *ui_g_Dynamic_Armor;
extern ui_interface_number_t *ui_g_Dynamic_Speed;
extern ui_interface_number_t *ui_g_Dynamic_LegLength_Num;
extern ui_interface_line_t *ui_g_Dynamic_Right_down_Leg;
extern ui_interface_line_t *ui_g_Dynamic_Right_up_Leg;
extern ui_interface_line_t *ui_g_Dynamic_Left_up_Leg;
extern ui_interface_line_t *ui_g_Dynamic_Left_down_Leg;
extern ui_interface_arc_t *ui_g_Dynamic_Fric_R;
extern ui_interface_arc_t *ui_g_Dynamic_Fric_L;
extern ui_interface_rect_t *ui_g_Dynamic_LegLength_State;
extern ui_interface_arc_t *ui_g_Dynamic_SuperCap;
extern ui_interface_number_t *ui_g_Dynamic_Pitch_Num;
extern ui_interface_string_t *ui_g_Dynamic_Shoot_State;
extern ui_interface_string_t *ui_g_Dynamic_Chassis_State;

void ui_init_g_Dynamic();
void ui_update_g_Dynamic();
void ui_remove_g_Dynamic();

extern ui_interface_line_t *ui_g_Static_Star_1;
extern ui_interface_line_t *ui_g_Static_Star_7;
extern ui_interface_arc_t *ui_g_Static_SuperCap_1;
extern ui_interface_arc_t *ui_g_Static_SuperCap_2;
extern ui_interface_arc_t *ui_g_Static_SuperCap_3;
extern ui_interface_arc_t *ui_g_Static_SuperCap_4;
extern ui_interface_arc_t *ui_g_Static_SuperCap_5;
extern ui_interface_arc_t *ui_g_Static_Pitch_5;
extern ui_interface_arc_t *ui_g_Static_Pitch_4;
extern ui_interface_arc_t *ui_g_Static_Pitch_3;
extern ui_interface_arc_t *ui_g_Static_Pitch_2;
extern ui_interface_arc_t *ui_g_Static_Pitch_1;
extern ui_interface_round_t *ui_g_Static_Star_Round;
extern ui_interface_round_t *ui_g_Static_R1;
extern ui_interface_line_t *ui_g_Static_CoastLine1;
extern ui_interface_line_t *ui_g_Static_Line;
extern ui_interface_line_t *ui_g_Static_RightSign;
extern ui_interface_line_t *ui_g_Static_LeftSign;
extern ui_interface_line_t *ui_g_Static_RoadLine3;
extern ui_interface_line_t *ui_g_Static_CoastLine2;
extern ui_interface_number_t *ui_g_Static_Mid_LegLength1_1;
extern ui_interface_number_t *ui_g_Static_Mid_LegLength2_2;
extern ui_interface_string_t *ui_g_Static_Shoot_State;
extern ui_interface_string_t *ui_g_Static_Leg_Length;
extern ui_interface_string_t *ui_g_Static_cm;
extern ui_interface_string_t *ui_g_Static_Kawshiro;
extern ui_interface_string_t *ui_g_Static_Chassis_State;
extern ui_interface_string_t *ui_g_Static_Low_LegLength;
extern ui_interface_string_t *ui_g_Static_Mid_LegLength1_M;
extern ui_interface_string_t *ui_g_Static_WUST_RM;
extern ui_interface_string_t *ui_g_Static_High_LegLength;
extern ui_interface_string_t *ui_g_Static_Mid_LegLength2_M;

void ui_init_g_Static();
void ui_update_g_Static();
void ui_remove_g_Static();


#endif // UI_g_H
