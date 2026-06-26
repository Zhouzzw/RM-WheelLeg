//
// Created by RM UI Designer
// Static Edition
//

#include <string.h>

#include "ui_interface.h"

ui_7_frame_t ui_g_Dynamic_0;

ui_interface_rect_t *ui_g_Dynamic_Aim = (ui_interface_rect_t*)&(ui_g_Dynamic_0.data[0]);
ui_interface_arc_t *ui_g_Dynamic_Pitch = (ui_interface_arc_t*)&(ui_g_Dynamic_0.data[1]);
ui_interface_line_t *ui_g_Dynamic_Armor = (ui_interface_line_t*)&(ui_g_Dynamic_0.data[2]);
ui_interface_number_t *ui_g_Dynamic_Speed = (ui_interface_number_t*)&(ui_g_Dynamic_0.data[3]);
ui_interface_number_t *ui_g_Dynamic_LegLength_Num = (ui_interface_number_t*)&(ui_g_Dynamic_0.data[4]);
ui_interface_line_t *ui_g_Dynamic_Right_down_Leg = (ui_interface_line_t*)&(ui_g_Dynamic_0.data[5]);
ui_interface_line_t *ui_g_Dynamic_Right_up_Leg = (ui_interface_line_t*)&(ui_g_Dynamic_0.data[6]);

void _ui_init_g_Dynamic_0() {
    for (int i = 0; i < 7; i++) {
        ui_g_Dynamic_0.data[i].figure_name[0] = 0;
        ui_g_Dynamic_0.data[i].figure_name[1] = 0;
        ui_g_Dynamic_0.data[i].figure_name[2] = i + 0;
        ui_g_Dynamic_0.data[i].operate_type = 1;
    }
    for (int i = 7; i < 7; i++) {
        ui_g_Dynamic_0.data[i].operate_type = 0;
    }

    ui_g_Dynamic_Aim->figure_type = 1;
    ui_g_Dynamic_Aim->operate_type = 1;
    ui_g_Dynamic_Aim->layer = 0;
    ui_g_Dynamic_Aim->color = 5;
    ui_g_Dynamic_Aim->start_x = 687;
    ui_g_Dynamic_Aim->start_y = 299;
    ui_g_Dynamic_Aim->width = 3;
    ui_g_Dynamic_Aim->end_x = 1233;
    ui_g_Dynamic_Aim->end_y = 744;

    ui_g_Dynamic_Pitch->figure_type = 4;
    ui_g_Dynamic_Pitch->operate_type = 1;
    ui_g_Dynamic_Pitch->layer = 0;
    ui_g_Dynamic_Pitch->color = 6;
    ui_g_Dynamic_Pitch->start_x = 962;
    ui_g_Dynamic_Pitch->start_y = 541;
    ui_g_Dynamic_Pitch->width = 16;
    ui_g_Dynamic_Pitch->start_angle = 269;
    ui_g_Dynamic_Pitch->end_angle = 271;
    ui_g_Dynamic_Pitch->rx = 380;
    ui_g_Dynamic_Pitch->ry = 380;

    ui_g_Dynamic_Armor->figure_type = 0;
    ui_g_Dynamic_Armor->operate_type = 1;
    ui_g_Dynamic_Armor->layer = 0;
    ui_g_Dynamic_Armor->color = 6;
    ui_g_Dynamic_Armor->start_x = 1600;
    ui_g_Dynamic_Armor->start_y = 847;
    ui_g_Dynamic_Armor->width = 5;
    ui_g_Dynamic_Armor->end_x = 1674;
    ui_g_Dynamic_Armor->end_y = 847;

    ui_g_Dynamic_Speed->figure_type = 6;
    ui_g_Dynamic_Speed->operate_type = 1;
    ui_g_Dynamic_Speed->layer = 0;
    ui_g_Dynamic_Speed->color = 6;
    ui_g_Dynamic_Speed->start_x = 1622;
    ui_g_Dynamic_Speed->start_y = 610;
    ui_g_Dynamic_Speed->width = 3;
    ui_g_Dynamic_Speed->font_size = 30;
    ui_g_Dynamic_Speed->number = 1;

    ui_g_Dynamic_LegLength_Num->figure_type = 6;
    ui_g_Dynamic_LegLength_Num->operate_type = 1;
    ui_g_Dynamic_LegLength_Num->layer = 0;
    ui_g_Dynamic_LegLength_Num->color = 2;
    ui_g_Dynamic_LegLength_Num->start_x = 309;
    ui_g_Dynamic_LegLength_Num->start_y = 707;
    ui_g_Dynamic_LegLength_Num->width = 2;
    ui_g_Dynamic_LegLength_Num->font_size = 20;
    ui_g_Dynamic_LegLength_Num->number = 45;

    ui_g_Dynamic_Right_down_Leg->figure_type = 0;
    ui_g_Dynamic_Right_down_Leg->operate_type = 1;
    ui_g_Dynamic_Right_down_Leg->layer = 0;
    ui_g_Dynamic_Right_down_Leg->color = 1;
    ui_g_Dynamic_Right_down_Leg->start_x = 1598;
    ui_g_Dynamic_Right_down_Leg->start_y = 646;
    ui_g_Dynamic_Right_down_Leg->width = 3;
    ui_g_Dynamic_Right_down_Leg->end_x = 1709;
    ui_g_Dynamic_Right_down_Leg->end_y = 744;

    ui_g_Dynamic_Right_up_Leg->figure_type = 0;
    ui_g_Dynamic_Right_up_Leg->operate_type = 1;
    ui_g_Dynamic_Right_up_Leg->layer = 0;
    ui_g_Dynamic_Right_up_Leg->color = 1;
    ui_g_Dynamic_Right_up_Leg->start_x = 1626;
    ui_g_Dynamic_Right_up_Leg->start_y = 807;
    ui_g_Dynamic_Right_up_Leg->width = 3;
    ui_g_Dynamic_Right_up_Leg->end_x = 1709;
    ui_g_Dynamic_Right_up_Leg->end_y = 740;


    ui_proc_7_frame(&ui_g_Dynamic_0);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_0, sizeof(ui_g_Dynamic_0));
}

void _ui_update_g_Dynamic_0() {
    for (int i = 0; i < 7; i++) {
        ui_g_Dynamic_0.data[i].operate_type = 2;
    }

    ui_proc_7_frame(&ui_g_Dynamic_0);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_0, sizeof(ui_g_Dynamic_0));
}

void _ui_remove_g_Dynamic_0() {
    for (int i = 0; i < 7; i++) {
        ui_g_Dynamic_0.data[i].operate_type = 3;
    }

    ui_proc_7_frame(&ui_g_Dynamic_0);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_0, sizeof(ui_g_Dynamic_0));
}
ui_7_frame_t ui_g_Dynamic_1;

ui_interface_line_t *ui_g_Dynamic_Left_up_Leg = (ui_interface_line_t*)&(ui_g_Dynamic_1.data[0]);
ui_interface_line_t *ui_g_Dynamic_Left_down_Leg = (ui_interface_line_t*)&(ui_g_Dynamic_1.data[1]);
ui_interface_arc_t *ui_g_Dynamic_Fric_R = (ui_interface_arc_t*)&(ui_g_Dynamic_1.data[2]);
ui_interface_arc_t *ui_g_Dynamic_Fric_L = (ui_interface_arc_t*)&(ui_g_Dynamic_1.data[3]);
ui_interface_rect_t *ui_g_Dynamic_LegLength_State = (ui_interface_rect_t*)&(ui_g_Dynamic_1.data[4]);
ui_interface_arc_t *ui_g_Dynamic_SuperCap = (ui_interface_arc_t*)&(ui_g_Dynamic_1.data[5]);
ui_interface_number_t *ui_g_Dynamic_Pitch_Num = (ui_interface_number_t*)&(ui_g_Dynamic_1.data[6]);

void _ui_init_g_Dynamic_1() {
    for (int i = 0; i < 7; i++) {
        ui_g_Dynamic_1.data[i].figure_name[0] = 0;
        ui_g_Dynamic_1.data[i].figure_name[1] = 0;
        ui_g_Dynamic_1.data[i].figure_name[2] = i + 7;
        ui_g_Dynamic_1.data[i].operate_type = 1;
    }
    for (int i = 7; i < 7; i++) {
        ui_g_Dynamic_1.data[i].operate_type = 0;
    }

    ui_g_Dynamic_Left_up_Leg->figure_type = 0;
    ui_g_Dynamic_Left_up_Leg->operate_type = 1;
    ui_g_Dynamic_Left_up_Leg->layer = 0;
    ui_g_Dynamic_Left_up_Leg->color = 3;
    ui_g_Dynamic_Left_up_Leg->start_x = 1624;
    ui_g_Dynamic_Left_up_Leg->start_y = 806;
    ui_g_Dynamic_Left_up_Leg->width = 3;
    ui_g_Dynamic_Left_up_Leg->end_x = 1696;
    ui_g_Dynamic_Left_up_Leg->end_y = 739;

    ui_g_Dynamic_Left_down_Leg->figure_type = 0;
    ui_g_Dynamic_Left_down_Leg->operate_type = 1;
    ui_g_Dynamic_Left_down_Leg->layer = 0;
    ui_g_Dynamic_Left_down_Leg->color = 3;
    ui_g_Dynamic_Left_down_Leg->start_x = 1597;
    ui_g_Dynamic_Left_down_Leg->start_y = 652;
    ui_g_Dynamic_Left_down_Leg->width = 3;
    ui_g_Dynamic_Left_down_Leg->end_x = 1697;
    ui_g_Dynamic_Left_down_Leg->end_y = 739;

    ui_g_Dynamic_Fric_R->figure_type = 4;
    ui_g_Dynamic_Fric_R->operate_type = 1;
    ui_g_Dynamic_Fric_R->layer = 0;
    ui_g_Dynamic_Fric_R->color = 6;
    ui_g_Dynamic_Fric_R->start_x = 1769;
    ui_g_Dynamic_Fric_R->start_y = 586;
    ui_g_Dynamic_Fric_R->width = 5;
    ui_g_Dynamic_Fric_R->start_angle = 30;
    ui_g_Dynamic_Fric_R->end_angle = 330;
    ui_g_Dynamic_Fric_R->rx = 21;
    ui_g_Dynamic_Fric_R->ry = 21;

    ui_g_Dynamic_Fric_L->figure_type = 4;
    ui_g_Dynamic_Fric_L->operate_type = 1;
    ui_g_Dynamic_Fric_L->layer = 0;
    ui_g_Dynamic_Fric_L->color = 6;
    ui_g_Dynamic_Fric_L->start_x = 1494;
    ui_g_Dynamic_Fric_L->start_y = 586;
    ui_g_Dynamic_Fric_L->width = 5;
    ui_g_Dynamic_Fric_L->start_angle = 30;
    ui_g_Dynamic_Fric_L->end_angle = 330;
    ui_g_Dynamic_Fric_L->rx = 21;
    ui_g_Dynamic_Fric_L->ry = 21;

    ui_g_Dynamic_LegLength_State->figure_type = 1;
    ui_g_Dynamic_LegLength_State->operate_type = 1;
    ui_g_Dynamic_LegLength_State->layer = 0;
    ui_g_Dynamic_LegLength_State->color = 2;
    ui_g_Dynamic_LegLength_State->start_x = 703;
    ui_g_Dynamic_LegLength_State->start_y = 311;
    ui_g_Dynamic_LegLength_State->width = 2;
    ui_g_Dynamic_LegLength_State->end_x = 753;
    ui_g_Dynamic_LegLength_State->end_y = 361;

    ui_g_Dynamic_SuperCap->figure_type = 4;
    ui_g_Dynamic_SuperCap->operate_type = 1;
    ui_g_Dynamic_SuperCap->layer = 0;
    ui_g_Dynamic_SuperCap->color = 6;
    ui_g_Dynamic_SuperCap->start_x = 960;
    ui_g_Dynamic_SuperCap->start_y = 539;
    ui_g_Dynamic_SuperCap->width = 16;
    ui_g_Dynamic_SuperCap->start_angle = 50;
    ui_g_Dynamic_SuperCap->end_angle = 130;
    ui_g_Dynamic_SuperCap->rx = 380;
    ui_g_Dynamic_SuperCap->ry = 380;

    ui_g_Dynamic_Pitch_Num->figure_type = 6;
    ui_g_Dynamic_Pitch_Num->operate_type = 1;
    ui_g_Dynamic_Pitch_Num->layer = 0;
    ui_g_Dynamic_Pitch_Num->color = 6;
    ui_g_Dynamic_Pitch_Num->start_x = 481;
    ui_g_Dynamic_Pitch_Num->start_y = 640;
    ui_g_Dynamic_Pitch_Num->width = 3;
    ui_g_Dynamic_Pitch_Num->font_size = 30;
    ui_g_Dynamic_Pitch_Num->number = 45;


    ui_proc_7_frame(&ui_g_Dynamic_1);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_1, sizeof(ui_g_Dynamic_1));
}

void _ui_update_g_Dynamic_1() {
    for (int i = 0; i < 7; i++) {
        ui_g_Dynamic_1.data[i].operate_type = 2;
    }

    ui_proc_7_frame(&ui_g_Dynamic_1);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_1, sizeof(ui_g_Dynamic_1));
}

void _ui_remove_g_Dynamic_1() {
    for (int i = 0; i < 7; i++) {
        ui_g_Dynamic_1.data[i].operate_type = 3;
    }

    ui_proc_7_frame(&ui_g_Dynamic_1);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_1, sizeof(ui_g_Dynamic_1));
}

ui_string_frame_t ui_g_Dynamic_2;
ui_interface_string_t* ui_g_Dynamic_Shoot_State = &(ui_g_Dynamic_2.option);

void _ui_init_g_Dynamic_2() {
    ui_g_Dynamic_2.option.figure_name[0] = 0;
    ui_g_Dynamic_2.option.figure_name[1] = 0;
    ui_g_Dynamic_2.option.figure_name[2] = 14;
    ui_g_Dynamic_2.option.operate_type = 1;

    ui_g_Dynamic_Shoot_State->figure_type = 7;
    ui_g_Dynamic_Shoot_State->operate_type = 1;
    ui_g_Dynamic_Shoot_State->layer = 0;
    ui_g_Dynamic_Shoot_State->color = 6;
    ui_g_Dynamic_Shoot_State->start_x = 309;
    ui_g_Dynamic_Shoot_State->start_y = 781;
    ui_g_Dynamic_Shoot_State->width = 2;
    ui_g_Dynamic_Shoot_State->font_size = 20;
    ui_g_Dynamic_Shoot_State->str_length = 11;
    strcpy(ui_g_Dynamic_Shoot_State->string, "Shoot_Ready");


    ui_proc_string_frame(&ui_g_Dynamic_2);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_2, sizeof(ui_g_Dynamic_2));
}

void _ui_update_g_Dynamic_2() {
    ui_g_Dynamic_2.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Dynamic_2);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_2, sizeof(ui_g_Dynamic_2));
}

void _ui_remove_g_Dynamic_2() {
    ui_g_Dynamic_2.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Dynamic_2);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_2, sizeof(ui_g_Dynamic_2));
}
ui_string_frame_t ui_g_Dynamic_3;
ui_interface_string_t* ui_g_Dynamic_Chassis_State = &(ui_g_Dynamic_3.option);

void _ui_init_g_Dynamic_3() {
    ui_g_Dynamic_3.option.figure_name[0] = 0;
    ui_g_Dynamic_3.option.figure_name[1] = 0;
    ui_g_Dynamic_3.option.figure_name[2] = 15;
    ui_g_Dynamic_3.option.operate_type = 1;

    ui_g_Dynamic_Chassis_State->figure_type = 7;
    ui_g_Dynamic_Chassis_State->operate_type = 1;
    ui_g_Dynamic_Chassis_State->layer = 0;
    ui_g_Dynamic_Chassis_State->color = 6;
    ui_g_Dynamic_Chassis_State->start_x = 308;
    ui_g_Dynamic_Chassis_State->start_y = 855;
    ui_g_Dynamic_Chassis_State->width = 2;
    ui_g_Dynamic_Chassis_State->font_size = 20;
    ui_g_Dynamic_Chassis_State->str_length = 11;
    strcpy(ui_g_Dynamic_Chassis_State->string, "Chassis_OFF");


    ui_proc_string_frame(&ui_g_Dynamic_3);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_3, sizeof(ui_g_Dynamic_3));
}

void _ui_update_g_Dynamic_3() {
    ui_g_Dynamic_3.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Dynamic_3);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_3, sizeof(ui_g_Dynamic_3));
}

void _ui_remove_g_Dynamic_3() {
    ui_g_Dynamic_3.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Dynamic_3);
    SEND_MESSAGE((uint8_t *) &ui_g_Dynamic_3, sizeof(ui_g_Dynamic_3));
}

void ui_init_g_Dynamic() {
    _ui_init_g_Dynamic_0();
    _ui_init_g_Dynamic_1();
    _ui_init_g_Dynamic_2();
    _ui_init_g_Dynamic_3();
}

void ui_update_g_Dynamic() {
    _ui_update_g_Dynamic_0();
    _ui_update_g_Dynamic_1();
    _ui_update_g_Dynamic_2();
    _ui_update_g_Dynamic_3();
}

void ui_remove_g_Dynamic() {
    _ui_remove_g_Dynamic_0();
    _ui_remove_g_Dynamic_1();
    _ui_remove_g_Dynamic_2();
    _ui_remove_g_Dynamic_3();
}

ui_7_frame_t ui_g_Static_0;

ui_interface_line_t *ui_g_Static_Star_1 = (ui_interface_line_t*)&(ui_g_Static_0.data[0]);
ui_interface_line_t *ui_g_Static_Star_7 = (ui_interface_line_t*)&(ui_g_Static_0.data[1]);
ui_interface_arc_t *ui_g_Static_SuperCap_1 = (ui_interface_arc_t*)&(ui_g_Static_0.data[2]);
ui_interface_arc_t *ui_g_Static_SuperCap_2 = (ui_interface_arc_t*)&(ui_g_Static_0.data[3]);
ui_interface_arc_t *ui_g_Static_SuperCap_3 = (ui_interface_arc_t*)&(ui_g_Static_0.data[4]);
ui_interface_arc_t *ui_g_Static_SuperCap_4 = (ui_interface_arc_t*)&(ui_g_Static_0.data[5]);
ui_interface_arc_t *ui_g_Static_SuperCap_5 = (ui_interface_arc_t*)&(ui_g_Static_0.data[6]);

void _ui_init_g_Static_0() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_0.data[i].figure_name[0] = 0;
        ui_g_Static_0.data[i].figure_name[1] = 1;
        ui_g_Static_0.data[i].figure_name[2] = i + 0;
        ui_g_Static_0.data[i].operate_type = 1;
    }
    for (int i = 7; i < 7; i++) {
        ui_g_Static_0.data[i].operate_type = 0;
    }

    ui_g_Static_Star_1->figure_type = 0;
    ui_g_Static_Star_1->operate_type = 1;
    ui_g_Static_Star_1->layer = 0;
    ui_g_Static_Star_1->color = 8;
    ui_g_Static_Star_1->start_x = 660;
    ui_g_Static_Star_1->start_y = 538;
    ui_g_Static_Star_1->width = 3;
    ui_g_Static_Star_1->end_x = 1260;
    ui_g_Static_Star_1->end_y = 540;

    ui_g_Static_Star_7->figure_type = 0;
    ui_g_Static_Star_7->operate_type = 1;
    ui_g_Static_Star_7->layer = 0;
    ui_g_Static_Star_7->color = 8;
    ui_g_Static_Star_7->start_x = 960;
    ui_g_Static_Star_7->start_y = 599;
    ui_g_Static_Star_7->width = 1;
    ui_g_Static_Star_7->end_x = 960;
    ui_g_Static_Star_7->end_y = 249;

    ui_g_Static_SuperCap_1->figure_type = 4;
    ui_g_Static_SuperCap_1->operate_type = 1;
    ui_g_Static_SuperCap_1->layer = 0;
    ui_g_Static_SuperCap_1->color = 8;
    ui_g_Static_SuperCap_1->start_x = 960;
    ui_g_Static_SuperCap_1->start_y = 539;
    ui_g_Static_SuperCap_1->width = 16;
    ui_g_Static_SuperCap_1->start_angle = 49;
    ui_g_Static_SuperCap_1->end_angle = 50;
    ui_g_Static_SuperCap_1->rx = 380;
    ui_g_Static_SuperCap_1->ry = 380;

    ui_g_Static_SuperCap_2->figure_type = 4;
    ui_g_Static_SuperCap_2->operate_type = 1;
    ui_g_Static_SuperCap_2->layer = 0;
    ui_g_Static_SuperCap_2->color = 8;
    ui_g_Static_SuperCap_2->start_x = 960;
    ui_g_Static_SuperCap_2->start_y = 539;
    ui_g_Static_SuperCap_2->width = 16;
    ui_g_Static_SuperCap_2->start_angle = 130;
    ui_g_Static_SuperCap_2->end_angle = 131;
    ui_g_Static_SuperCap_2->rx = 380;
    ui_g_Static_SuperCap_2->ry = 380;

    ui_g_Static_SuperCap_3->figure_type = 4;
    ui_g_Static_SuperCap_3->operate_type = 1;
    ui_g_Static_SuperCap_3->layer = 0;
    ui_g_Static_SuperCap_3->color = 8;
    ui_g_Static_SuperCap_3->start_x = 960;
    ui_g_Static_SuperCap_3->start_y = 539;
    ui_g_Static_SuperCap_3->width = 16;
    ui_g_Static_SuperCap_3->start_angle = 90;
    ui_g_Static_SuperCap_3->end_angle = 91;
    ui_g_Static_SuperCap_3->rx = 360;
    ui_g_Static_SuperCap_3->ry = 360;

    ui_g_Static_SuperCap_4->figure_type = 4;
    ui_g_Static_SuperCap_4->operate_type = 1;
    ui_g_Static_SuperCap_4->layer = 0;
    ui_g_Static_SuperCap_4->color = 8;
    ui_g_Static_SuperCap_4->start_x = 960;
    ui_g_Static_SuperCap_4->start_y = 539;
    ui_g_Static_SuperCap_4->width = 16;
    ui_g_Static_SuperCap_4->start_angle = 115;
    ui_g_Static_SuperCap_4->end_angle = 116;
    ui_g_Static_SuperCap_4->rx = 360;
    ui_g_Static_SuperCap_4->ry = 360;

    ui_g_Static_SuperCap_5->figure_type = 4;
    ui_g_Static_SuperCap_5->operate_type = 1;
    ui_g_Static_SuperCap_5->layer = 0;
    ui_g_Static_SuperCap_5->color = 8;
    ui_g_Static_SuperCap_5->start_x = 960;
    ui_g_Static_SuperCap_5->start_y = 539;
    ui_g_Static_SuperCap_5->width = 16;
    ui_g_Static_SuperCap_5->start_angle = 65;
    ui_g_Static_SuperCap_5->end_angle = 66;
    ui_g_Static_SuperCap_5->rx = 360;
    ui_g_Static_SuperCap_5->ry = 360;


    ui_proc_7_frame(&ui_g_Static_0);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_0, sizeof(ui_g_Static_0));
}

void _ui_update_g_Static_0() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_0.data[i].operate_type = 2;
    }

    ui_proc_7_frame(&ui_g_Static_0);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_0, sizeof(ui_g_Static_0));
}

void _ui_remove_g_Static_0() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_0.data[i].operate_type = 3;
    }

    ui_proc_7_frame(&ui_g_Static_0);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_0, sizeof(ui_g_Static_0));
}
ui_7_frame_t ui_g_Static_1;

ui_interface_arc_t *ui_g_Static_Pitch_5 = (ui_interface_arc_t*)&(ui_g_Static_1.data[0]);
ui_interface_arc_t *ui_g_Static_Pitch_4 = (ui_interface_arc_t*)&(ui_g_Static_1.data[1]);
ui_interface_arc_t *ui_g_Static_Pitch_3 = (ui_interface_arc_t*)&(ui_g_Static_1.data[2]);
ui_interface_arc_t *ui_g_Static_Pitch_2 = (ui_interface_arc_t*)&(ui_g_Static_1.data[3]);
ui_interface_arc_t *ui_g_Static_Pitch_1 = (ui_interface_arc_t*)&(ui_g_Static_1.data[4]);
ui_interface_round_t *ui_g_Static_Star_Round = (ui_interface_round_t*)&(ui_g_Static_1.data[5]);
ui_interface_round_t *ui_g_Static_R1 = (ui_interface_round_t*)&(ui_g_Static_1.data[6]);

void _ui_init_g_Static_1() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_1.data[i].figure_name[0] = 0;
        ui_g_Static_1.data[i].figure_name[1] = 1;
        ui_g_Static_1.data[i].figure_name[2] = i + 7;
        ui_g_Static_1.data[i].operate_type = 1;
    }
    for (int i = 7; i < 7; i++) {
        ui_g_Static_1.data[i].operate_type = 0;
    }

    ui_g_Static_Pitch_5->figure_type = 4;
    ui_g_Static_Pitch_5->operate_type = 1;
    ui_g_Static_Pitch_5->layer = 0;
    ui_g_Static_Pitch_5->color = 8;
    ui_g_Static_Pitch_5->start_x = 960;
    ui_g_Static_Pitch_5->start_y = 539;
    ui_g_Static_Pitch_5->width = 16;
    ui_g_Static_Pitch_5->start_angle = 245;
    ui_g_Static_Pitch_5->end_angle = 246;
    ui_g_Static_Pitch_5->rx = 360;
    ui_g_Static_Pitch_5->ry = 360;

    ui_g_Static_Pitch_4->figure_type = 4;
    ui_g_Static_Pitch_4->operate_type = 1;
    ui_g_Static_Pitch_4->layer = 0;
    ui_g_Static_Pitch_4->color = 8;
    ui_g_Static_Pitch_4->start_x = 960;
    ui_g_Static_Pitch_4->start_y = 539;
    ui_g_Static_Pitch_4->width = 16;
    ui_g_Static_Pitch_4->start_angle = 295;
    ui_g_Static_Pitch_4->end_angle = 296;
    ui_g_Static_Pitch_4->rx = 360;
    ui_g_Static_Pitch_4->ry = 360;

    ui_g_Static_Pitch_3->figure_type = 4;
    ui_g_Static_Pitch_3->operate_type = 1;
    ui_g_Static_Pitch_3->layer = 0;
    ui_g_Static_Pitch_3->color = 8;
    ui_g_Static_Pitch_3->start_x = 960;
    ui_g_Static_Pitch_3->start_y = 539;
    ui_g_Static_Pitch_3->width = 16;
    ui_g_Static_Pitch_3->start_angle = 270;
    ui_g_Static_Pitch_3->end_angle = 271;
    ui_g_Static_Pitch_3->rx = 360;
    ui_g_Static_Pitch_3->ry = 360;

    ui_g_Static_Pitch_2->figure_type = 4;
    ui_g_Static_Pitch_2->operate_type = 1;
    ui_g_Static_Pitch_2->layer = 0;
    ui_g_Static_Pitch_2->color = 8;
    ui_g_Static_Pitch_2->start_x = 960;
    ui_g_Static_Pitch_2->start_y = 539;
    ui_g_Static_Pitch_2->width = 16;
    ui_g_Static_Pitch_2->start_angle = 310;
    ui_g_Static_Pitch_2->end_angle = 311;
    ui_g_Static_Pitch_2->rx = 380;
    ui_g_Static_Pitch_2->ry = 380;

    ui_g_Static_Pitch_1->figure_type = 4;
    ui_g_Static_Pitch_1->operate_type = 1;
    ui_g_Static_Pitch_1->layer = 0;
    ui_g_Static_Pitch_1->color = 8;
    ui_g_Static_Pitch_1->start_x = 960;
    ui_g_Static_Pitch_1->start_y = 539;
    ui_g_Static_Pitch_1->width = 16;
    ui_g_Static_Pitch_1->start_angle = 229;
    ui_g_Static_Pitch_1->end_angle = 230;
    ui_g_Static_Pitch_1->rx = 380;
    ui_g_Static_Pitch_1->ry = 380;

    ui_g_Static_Star_Round->figure_type = 2;
    ui_g_Static_Star_Round->operate_type = 1;
    ui_g_Static_Star_Round->layer = 0;
    ui_g_Static_Star_Round->color = 8;
    ui_g_Static_Star_Round->start_x = 959;
    ui_g_Static_Star_Round->start_y = 540;
    ui_g_Static_Star_Round->width = 2;
    ui_g_Static_Star_Round->r = 20;

    ui_g_Static_R1->figure_type = 2;
    ui_g_Static_R1->operate_type = 1;
    ui_g_Static_R1->layer = 0;
    ui_g_Static_R1->color = 8;
    ui_g_Static_R1->start_x = 1633;
    ui_g_Static_R1->start_y = 702;
    ui_g_Static_R1->width = 5;
    ui_g_Static_R1->r = 140;


    ui_proc_7_frame(&ui_g_Static_1);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_1, sizeof(ui_g_Static_1));
}

void _ui_update_g_Static_1() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_1.data[i].operate_type = 2;
    }

    ui_proc_7_frame(&ui_g_Static_1);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_1, sizeof(ui_g_Static_1));
}

void _ui_remove_g_Static_1() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_1.data[i].operate_type = 3;
    }

    ui_proc_7_frame(&ui_g_Static_1);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_1, sizeof(ui_g_Static_1));
}
ui_7_frame_t ui_g_Static_2;

ui_interface_line_t *ui_g_Static_CoastLine1 = (ui_interface_line_t*)&(ui_g_Static_2.data[0]);
ui_interface_line_t *ui_g_Static_Line = (ui_interface_line_t*)&(ui_g_Static_2.data[1]);
ui_interface_line_t *ui_g_Static_RightSign = (ui_interface_line_t*)&(ui_g_Static_2.data[2]);
ui_interface_line_t *ui_g_Static_LeftSign = (ui_interface_line_t*)&(ui_g_Static_2.data[3]);
ui_interface_line_t *ui_g_Static_RoadLine3 = (ui_interface_line_t*)&(ui_g_Static_2.data[4]);
ui_interface_line_t *ui_g_Static_CoastLine2 = (ui_interface_line_t*)&(ui_g_Static_2.data[5]);
ui_interface_number_t *ui_g_Static_Mid_LegLength1_1 = (ui_interface_number_t*)&(ui_g_Static_2.data[6]);

void _ui_init_g_Static_2() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_2.data[i].figure_name[0] = 0;
        ui_g_Static_2.data[i].figure_name[1] = 1;
        ui_g_Static_2.data[i].figure_name[2] = i + 14;
        ui_g_Static_2.data[i].operate_type = 1;
    }
    for (int i = 7; i < 7; i++) {
        ui_g_Static_2.data[i].operate_type = 0;
    }

    ui_g_Static_CoastLine1->figure_type = 0;
    ui_g_Static_CoastLine1->operate_type = 1;
    ui_g_Static_CoastLine1->layer = 0;
    ui_g_Static_CoastLine1->color = 8;
    ui_g_Static_CoastLine1->start_x = 490;
    ui_g_Static_CoastLine1->start_y = 192;
    ui_g_Static_CoastLine1->width = 5;
    ui_g_Static_CoastLine1->end_x = 690;
    ui_g_Static_CoastLine1->end_y = 527;

    ui_g_Static_Line->figure_type = 0;
    ui_g_Static_Line->operate_type = 1;
    ui_g_Static_Line->layer = 0;
    ui_g_Static_Line->color = 8;
    ui_g_Static_Line->start_x = 1519;
    ui_g_Static_Line->start_y = 628;
    ui_g_Static_Line->width = 5;
    ui_g_Static_Line->end_x = 1754;
    ui_g_Static_Line->end_y = 628;

    ui_g_Static_RightSign->figure_type = 0;
    ui_g_Static_RightSign->operate_type = 1;
    ui_g_Static_RightSign->layer = 0;
    ui_g_Static_RightSign->color = 1;
    ui_g_Static_RightSign->start_x = 1636;
    ui_g_Static_RightSign->start_y = 628;
    ui_g_Static_RightSign->width = 10;
    ui_g_Static_RightSign->end_x = 1756;
    ui_g_Static_RightSign->end_y = 628;

    ui_g_Static_LeftSign->figure_type = 0;
    ui_g_Static_LeftSign->operate_type = 1;
    ui_g_Static_LeftSign->layer = 0;
    ui_g_Static_LeftSign->color = 3;
    ui_g_Static_LeftSign->start_x = 1636;
    ui_g_Static_LeftSign->start_y = 628;
    ui_g_Static_LeftSign->width = 10;
    ui_g_Static_LeftSign->end_x = 1519;
    ui_g_Static_LeftSign->end_y = 628;

    ui_g_Static_RoadLine3->figure_type = 0;
    ui_g_Static_RoadLine3->operate_type = 1;
    ui_g_Static_RoadLine3->layer = 0;
    ui_g_Static_RoadLine3->color = 1;
    ui_g_Static_RoadLine3->start_x = 525;
    ui_g_Static_RoadLine3->start_y = 241;
    ui_g_Static_RoadLine3->width = 3;
    ui_g_Static_RoadLine3->end_x = 1399;
    ui_g_Static_RoadLine3->end_y = 241;

    ui_g_Static_CoastLine2->figure_type = 0;
    ui_g_Static_CoastLine2->operate_type = 1;
    ui_g_Static_CoastLine2->layer = 0;
    ui_g_Static_CoastLine2->color = 8;
    ui_g_Static_CoastLine2->start_x = 1430;
    ui_g_Static_CoastLine2->start_y = 192;
    ui_g_Static_CoastLine2->width = 5;
    ui_g_Static_CoastLine2->end_x = 1230;
    ui_g_Static_CoastLine2->end_y = 527;

    ui_g_Static_Mid_LegLength1_1->figure_type = 6;
    ui_g_Static_Mid_LegLength1_1->operate_type = 1;
    ui_g_Static_Mid_LegLength1_1->layer = 0;
    ui_g_Static_Mid_LegLength1_1->color = 8;
    ui_g_Static_Mid_LegLength1_1->start_x = 750;
    ui_g_Static_Mid_LegLength1_1->start_y = 415;
    ui_g_Static_Mid_LegLength1_1->width = 2;
    ui_g_Static_Mid_LegLength1_1->font_size = 23;
    ui_g_Static_Mid_LegLength1_1->number = 1;


    ui_proc_7_frame(&ui_g_Static_2);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_2, sizeof(ui_g_Static_2));
}

void _ui_update_g_Static_2() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_2.data[i].operate_type = 2;
    }

    ui_proc_7_frame(&ui_g_Static_2);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_2, sizeof(ui_g_Static_2));
}

void _ui_remove_g_Static_2() {
    for (int i = 0; i < 7; i++) {
        ui_g_Static_2.data[i].operate_type = 3;
    }

    ui_proc_7_frame(&ui_g_Static_2);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_2, sizeof(ui_g_Static_2));
}
ui_1_frame_t ui_g_Static_3;

ui_interface_number_t *ui_g_Static_Mid_LegLength2_2 = (ui_interface_number_t*)&(ui_g_Static_3.data[0]);

void _ui_init_g_Static_3() {
    for (int i = 0; i < 1; i++) {
        ui_g_Static_3.data[i].figure_name[0] = 0;
        ui_g_Static_3.data[i].figure_name[1] = 1;
        ui_g_Static_3.data[i].figure_name[2] = i + 21;
        ui_g_Static_3.data[i].operate_type = 1;
    }
    for (int i = 1; i < 1; i++) {
        ui_g_Static_3.data[i].operate_type = 0;
    }

    ui_g_Static_Mid_LegLength2_2->figure_type = 6;
    ui_g_Static_Mid_LegLength2_2->operate_type = 1;
    ui_g_Static_Mid_LegLength2_2->layer = 0;
    ui_g_Static_Mid_LegLength2_2->color = 8;
    ui_g_Static_Mid_LegLength2_2->start_x = 844;
    ui_g_Static_Mid_LegLength2_2->start_y = 415;
    ui_g_Static_Mid_LegLength2_2->width = 2;
    ui_g_Static_Mid_LegLength2_2->font_size = 23;
    ui_g_Static_Mid_LegLength2_2->number = 2;


    ui_proc_1_frame(&ui_g_Static_3);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_3, sizeof(ui_g_Static_3));
}

void _ui_update_g_Static_3() {
    for (int i = 0; i < 1; i++) {
        ui_g_Static_3.data[i].operate_type = 2;
    }

    ui_proc_1_frame(&ui_g_Static_3);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_3, sizeof(ui_g_Static_3));
}

void _ui_remove_g_Static_3() {
    for (int i = 0; i < 1; i++) {
        ui_g_Static_3.data[i].operate_type = 3;
    }

    ui_proc_1_frame(&ui_g_Static_3);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_3, sizeof(ui_g_Static_3));
}

ui_string_frame_t ui_g_Static_4;
ui_interface_string_t* ui_g_Static_Shoot_State = &(ui_g_Static_4.option);

void _ui_init_g_Static_4() {
    ui_g_Static_4.option.figure_name[0] = 0;
    ui_g_Static_4.option.figure_name[1] = 1;
    ui_g_Static_4.option.figure_name[2] = 22;
    ui_g_Static_4.option.operate_type = 1;

    ui_g_Static_Shoot_State->figure_type = 7;
    ui_g_Static_Shoot_State->operate_type = 1;
    ui_g_Static_Shoot_State->layer = 0;
    ui_g_Static_Shoot_State->color = 8;
    ui_g_Static_Shoot_State->start_x = 60;
    ui_g_Static_Shoot_State->start_y = 781;
    ui_g_Static_Shoot_State->width = 2;
    ui_g_Static_Shoot_State->font_size = 20;
    ui_g_Static_Shoot_State->str_length = 12;
    strcpy(ui_g_Static_Shoot_State->string, "Shoot_State:");


    ui_proc_string_frame(&ui_g_Static_4);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_4, sizeof(ui_g_Static_4));
}

void _ui_update_g_Static_4() {
    ui_g_Static_4.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_4);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_4, sizeof(ui_g_Static_4));
}

void _ui_remove_g_Static_4() {
    ui_g_Static_4.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_4);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_4, sizeof(ui_g_Static_4));
}
ui_string_frame_t ui_g_Static_5;
ui_interface_string_t* ui_g_Static_Leg_Length = &(ui_g_Static_5.option);

void _ui_init_g_Static_5() {
    ui_g_Static_5.option.figure_name[0] = 0;
    ui_g_Static_5.option.figure_name[1] = 1;
    ui_g_Static_5.option.figure_name[2] = 23;
    ui_g_Static_5.option.operate_type = 1;

    ui_g_Static_Leg_Length->figure_type = 7;
    ui_g_Static_Leg_Length->operate_type = 1;
    ui_g_Static_Leg_Length->layer = 0;
    ui_g_Static_Leg_Length->color = 8;
    ui_g_Static_Leg_Length->start_x = 78;
    ui_g_Static_Leg_Length->start_y = 707;
    ui_g_Static_Leg_Length->width = 2;
    ui_g_Static_Leg_Length->font_size = 20;
    ui_g_Static_Leg_Length->str_length = 11;
    strcpy(ui_g_Static_Leg_Length->string, "Leg_Length:");


    ui_proc_string_frame(&ui_g_Static_5);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_5, sizeof(ui_g_Static_5));
}

void _ui_update_g_Static_5() {
    ui_g_Static_5.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_5);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_5, sizeof(ui_g_Static_5));
}

void _ui_remove_g_Static_5() {
    ui_g_Static_5.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_5);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_5, sizeof(ui_g_Static_5));
}
ui_string_frame_t ui_g_Static_6;
ui_interface_string_t* ui_g_Static_cm = &(ui_g_Static_6.option);

void _ui_init_g_Static_6() {
    ui_g_Static_6.option.figure_name[0] = 0;
    ui_g_Static_6.option.figure_name[1] = 1;
    ui_g_Static_6.option.figure_name[2] = 24;
    ui_g_Static_6.option.operate_type = 1;

    ui_g_Static_cm->figure_type = 7;
    ui_g_Static_cm->operate_type = 1;
    ui_g_Static_cm->layer = 0;
    ui_g_Static_cm->color = 2;
    ui_g_Static_cm->start_x = 367;
    ui_g_Static_cm->start_y = 709;
    ui_g_Static_cm->width = 2;
    ui_g_Static_cm->font_size = 20;
    ui_g_Static_cm->str_length = 2;
    strcpy(ui_g_Static_cm->string, "cm");


    ui_proc_string_frame(&ui_g_Static_6);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_6, sizeof(ui_g_Static_6));
}

void _ui_update_g_Static_6() {
    ui_g_Static_6.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_6);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_6, sizeof(ui_g_Static_6));
}

void _ui_remove_g_Static_6() {
    ui_g_Static_6.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_6);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_6, sizeof(ui_g_Static_6));
}
ui_string_frame_t ui_g_Static_7;
ui_interface_string_t* ui_g_Static_Kawshiro = &(ui_g_Static_7.option);

void _ui_init_g_Static_7() {
    ui_g_Static_7.option.figure_name[0] = 0;
    ui_g_Static_7.option.figure_name[1] = 1;
    ui_g_Static_7.option.figure_name[2] = 25;
    ui_g_Static_7.option.operate_type = 1;

    ui_g_Static_Kawshiro->figure_type = 7;
    ui_g_Static_Kawshiro->operate_type = 1;
    ui_g_Static_Kawshiro->layer = 0;
    ui_g_Static_Kawshiro->color = 6;
    ui_g_Static_Kawshiro->start_x = 1544;
    ui_g_Static_Kawshiro->start_y = 363;
    ui_g_Static_Kawshiro->width = 2;
    ui_g_Static_Kawshiro->font_size = 20;
    ui_g_Static_Kawshiro->str_length = 16;
    strcpy(ui_g_Static_Kawshiro->string, "Kawashiro  ZHOUS");


    ui_proc_string_frame(&ui_g_Static_7);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_7, sizeof(ui_g_Static_7));
}

void _ui_update_g_Static_7() {
    ui_g_Static_7.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_7);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_7, sizeof(ui_g_Static_7));
}

void _ui_remove_g_Static_7() {
    ui_g_Static_7.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_7);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_7, sizeof(ui_g_Static_7));
}
ui_string_frame_t ui_g_Static_8;
ui_interface_string_t* ui_g_Static_Chassis_State = &(ui_g_Static_8.option);

void _ui_init_g_Static_8() {
    ui_g_Static_8.option.figure_name[0] = 0;
    ui_g_Static_8.option.figure_name[1] = 1;
    ui_g_Static_8.option.figure_name[2] = 26;
    ui_g_Static_8.option.operate_type = 1;

    ui_g_Static_Chassis_State->figure_type = 7;
    ui_g_Static_Chassis_State->operate_type = 1;
    ui_g_Static_Chassis_State->layer = 0;
    ui_g_Static_Chassis_State->color = 8;
    ui_g_Static_Chassis_State->start_x = 20;
    ui_g_Static_Chassis_State->start_y = 855;
    ui_g_Static_Chassis_State->width = 2;
    ui_g_Static_Chassis_State->font_size = 20;
    ui_g_Static_Chassis_State->str_length = 14;
    strcpy(ui_g_Static_Chassis_State->string, "Chassis_State:");


    ui_proc_string_frame(&ui_g_Static_8);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_8, sizeof(ui_g_Static_8));
}

void _ui_update_g_Static_8() {
    ui_g_Static_8.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_8);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_8, sizeof(ui_g_Static_8));
}

void _ui_remove_g_Static_8() {
    ui_g_Static_8.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_8);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_8, sizeof(ui_g_Static_8));
}
ui_string_frame_t ui_g_Static_9;
ui_interface_string_t* ui_g_Static_Low_LegLength = &(ui_g_Static_9.option);

void _ui_init_g_Static_9() {
    ui_g_Static_9.option.figure_name[0] = 0;
    ui_g_Static_9.option.figure_name[1] = 1;
    ui_g_Static_9.option.figure_name[2] = 27;
    ui_g_Static_9.option.operate_type = 1;

    ui_g_Static_Low_LegLength->figure_type = 7;
    ui_g_Static_Low_LegLength->operate_type = 1;
    ui_g_Static_Low_LegLength->layer = 0;
    ui_g_Static_Low_LegLength->color = 8;
    ui_g_Static_Low_LegLength->start_x = 713;
    ui_g_Static_Low_LegLength->start_y = 358;
    ui_g_Static_Low_LegLength->width = 4;
    ui_g_Static_Low_LegLength->font_size = 35;
    ui_g_Static_Low_LegLength->str_length = 1;
    strcpy(ui_g_Static_Low_LegLength->string, "L");


    ui_proc_string_frame(&ui_g_Static_9);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_9, sizeof(ui_g_Static_9));
}

void _ui_update_g_Static_9() {
    ui_g_Static_9.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_9);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_9, sizeof(ui_g_Static_9));
}

void _ui_remove_g_Static_9() {
    ui_g_Static_9.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_9);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_9, sizeof(ui_g_Static_9));
}
ui_string_frame_t ui_g_Static_10;
ui_interface_string_t* ui_g_Static_Mid_LegLength1_M = &(ui_g_Static_10.option);

void _ui_init_g_Static_10() {
    ui_g_Static_10.option.figure_name[0] = 0;
    ui_g_Static_10.option.figure_name[1] = 1;
    ui_g_Static_10.option.figure_name[2] = 28;
    ui_g_Static_10.option.operate_type = 1;

    ui_g_Static_Mid_LegLength1_M->figure_type = 7;
    ui_g_Static_Mid_LegLength1_M->operate_type = 1;
    ui_g_Static_Mid_LegLength1_M->layer = 0;
    ui_g_Static_Mid_LegLength1_M->color = 8;
    ui_g_Static_Mid_LegLength1_M->start_x = 713;
    ui_g_Static_Mid_LegLength1_M->start_y = 428;
    ui_g_Static_Mid_LegLength1_M->width = 4;
    ui_g_Static_Mid_LegLength1_M->font_size = 35;
    ui_g_Static_Mid_LegLength1_M->str_length = 1;
    strcpy(ui_g_Static_Mid_LegLength1_M->string, "M");


    ui_proc_string_frame(&ui_g_Static_10);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_10, sizeof(ui_g_Static_10));
}

void _ui_update_g_Static_10() {
    ui_g_Static_10.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_10);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_10, sizeof(ui_g_Static_10));
}

void _ui_remove_g_Static_10() {
    ui_g_Static_10.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_10);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_10, sizeof(ui_g_Static_10));
}
ui_string_frame_t ui_g_Static_11;
ui_interface_string_t* ui_g_Static_WUST_RM = &(ui_g_Static_11.option);

void _ui_init_g_Static_11() {
    ui_g_Static_11.option.figure_name[0] = 0;
    ui_g_Static_11.option.figure_name[1] = 1;
    ui_g_Static_11.option.figure_name[2] = 29;
    ui_g_Static_11.option.operate_type = 1;

    ui_g_Static_WUST_RM->figure_type = 7;
    ui_g_Static_WUST_RM->operate_type = 1;
    ui_g_Static_WUST_RM->layer = 0;
    ui_g_Static_WUST_RM->color = 8;
    ui_g_Static_WUST_RM->start_x = 1582;
    ui_g_Static_WUST_RM->start_y = 444;
    ui_g_Static_WUST_RM->width = 4;
    ui_g_Static_WUST_RM->font_size = 40;
    ui_g_Static_WUST_RM->str_length = 7;
    strcpy(ui_g_Static_WUST_RM->string, "WUST_RM");


    ui_proc_string_frame(&ui_g_Static_11);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_11, sizeof(ui_g_Static_11));
}

void _ui_update_g_Static_11() {
    ui_g_Static_11.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_11);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_11, sizeof(ui_g_Static_11));
}

void _ui_remove_g_Static_11() {
    ui_g_Static_11.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_11);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_11, sizeof(ui_g_Static_11));
}
ui_string_frame_t ui_g_Static_12;
ui_interface_string_t* ui_g_Static_High_LegLength = &(ui_g_Static_12.option);

void _ui_init_g_Static_12() {
    ui_g_Static_12.option.figure_name[0] = 0;
    ui_g_Static_12.option.figure_name[1] = 1;
    ui_g_Static_12.option.figure_name[2] = 30;
    ui_g_Static_12.option.operate_type = 1;

    ui_g_Static_High_LegLength->figure_type = 7;
    ui_g_Static_High_LegLength->operate_type = 1;
    ui_g_Static_High_LegLength->layer = 0;
    ui_g_Static_High_LegLength->color = 8;
    ui_g_Static_High_LegLength->start_x = 713;
    ui_g_Static_High_LegLength->start_y = 496;
    ui_g_Static_High_LegLength->width = 4;
    ui_g_Static_High_LegLength->font_size = 35;
    ui_g_Static_High_LegLength->str_length = 1;
    strcpy(ui_g_Static_High_LegLength->string, "H");


    ui_proc_string_frame(&ui_g_Static_12);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_12, sizeof(ui_g_Static_12));
}

void _ui_update_g_Static_12() {
    ui_g_Static_12.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_12);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_12, sizeof(ui_g_Static_12));
}

void _ui_remove_g_Static_12() {
    ui_g_Static_12.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_12);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_12, sizeof(ui_g_Static_12));
}
ui_string_frame_t ui_g_Static_13;
ui_interface_string_t* ui_g_Static_Mid_LegLength2_M = &(ui_g_Static_13.option);

void _ui_init_g_Static_13() {
    ui_g_Static_13.option.figure_name[0] = 0;
    ui_g_Static_13.option.figure_name[1] = 1;
    ui_g_Static_13.option.figure_name[2] = 31;
    ui_g_Static_13.option.operate_type = 1;

    ui_g_Static_Mid_LegLength2_M->figure_type = 7;
    ui_g_Static_Mid_LegLength2_M->operate_type = 1;
    ui_g_Static_Mid_LegLength2_M->layer = 0;
    ui_g_Static_Mid_LegLength2_M->color = 8;
    ui_g_Static_Mid_LegLength2_M->start_x = 808;
    ui_g_Static_Mid_LegLength2_M->start_y = 428;
    ui_g_Static_Mid_LegLength2_M->width = 4;
    ui_g_Static_Mid_LegLength2_M->font_size = 35;
    ui_g_Static_Mid_LegLength2_M->str_length = 1;
    strcpy(ui_g_Static_Mid_LegLength2_M->string, "M");


    ui_proc_string_frame(&ui_g_Static_13);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_13, sizeof(ui_g_Static_13));
}

void _ui_update_g_Static_13() {
    ui_g_Static_13.option.operate_type = 2;

    ui_proc_string_frame(&ui_g_Static_13);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_13, sizeof(ui_g_Static_13));
}

void _ui_remove_g_Static_13() {
    ui_g_Static_13.option.operate_type = 3;

    ui_proc_string_frame(&ui_g_Static_13);
    SEND_MESSAGE((uint8_t *) &ui_g_Static_13, sizeof(ui_g_Static_13));
}

void ui_init_g_Static() {
    _ui_init_g_Static_0();
    _ui_init_g_Static_1();
    _ui_init_g_Static_2();
    _ui_init_g_Static_3();
    _ui_init_g_Static_4();
    _ui_init_g_Static_5();
    _ui_init_g_Static_6();
    _ui_init_g_Static_7();
    _ui_init_g_Static_8();
    _ui_init_g_Static_9();
    _ui_init_g_Static_10();
    _ui_init_g_Static_11();
    _ui_init_g_Static_12();
    _ui_init_g_Static_13();
}

void ui_update_g_Static() {
    _ui_update_g_Static_0();
    _ui_update_g_Static_1();
    _ui_update_g_Static_2();
    _ui_update_g_Static_3();
    _ui_update_g_Static_4();
    _ui_update_g_Static_5();
    _ui_update_g_Static_6();
    _ui_update_g_Static_7();
    _ui_update_g_Static_8();
    _ui_update_g_Static_9();
    _ui_update_g_Static_10();
    _ui_update_g_Static_11();
    _ui_update_g_Static_12();
    _ui_update_g_Static_13();
}

void ui_remove_g_Static() {
    _ui_remove_g_Static_0();
    _ui_remove_g_Static_1();
    _ui_remove_g_Static_2();
    _ui_remove_g_Static_3();
    _ui_remove_g_Static_4();
    _ui_remove_g_Static_5();
    _ui_remove_g_Static_6();
    _ui_remove_g_Static_7();
    _ui_remove_g_Static_8();
    _ui_remove_g_Static_9();
    _ui_remove_g_Static_10();
    _ui_remove_g_Static_11();
    _ui_remove_g_Static_12();
    _ui_remove_g_Static_13();
}

