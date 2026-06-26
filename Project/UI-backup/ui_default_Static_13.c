//
// Created by RM UI Designer
//

#include "ui_default_Static_13.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 25
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Static_13;
ui_interface_arc_t *ui_default_Static_Pitch_3 = (ui_interface_arc_t *)&(ui_default_Static_13.data[0]);
ui_interface_arc_t *ui_default_Static_Pitch_2 = (ui_interface_arc_t *)&(ui_default_Static_13.data[1]);
ui_interface_arc_t *ui_default_Static_Pitch_1 = (ui_interface_arc_t *)&(ui_default_Static_13.data[2]);
ui_interface_rect_t *ui_default_Static_Fric_L_Rect = (ui_interface_rect_t *)&(ui_default_Static_13.data[3]);
ui_interface_rect_t *ui_default_Static_Fric_M_Rect = (ui_interface_rect_t *)&(ui_default_Static_13.data[4]);
ui_interface_rect_t *ui_default_Static_Fric_R_Rect = (ui_interface_rect_t *)&(ui_default_Static_13.data[5]);
ui_interface_rect_t *ui_default_Static_Speed_Rect = (ui_interface_rect_t *)&(ui_default_Static_13.data[6]);

void _ui_init_default_Static_13() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_13.data[i].figure_name[0] = FRAME_ID;
        ui_default_Static_13.data[i].figure_name[1] = GROUP_ID;
        ui_default_Static_13.data[i].figure_name[2] = i + START_ID;
        ui_default_Static_13.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Static_13.data[i].operate_tpyel = 0;
    }

    ui_default_Static_Pitch_3->figure_tpye = 4;
    ui_default_Static_Pitch_3->layer = 0;
    ui_default_Static_Pitch_3->rx = 360;
    ui_default_Static_Pitch_3->ry = 360;
    ui_default_Static_Pitch_3->start_x = 960;
    ui_default_Static_Pitch_3->start_y = 540;
    ui_default_Static_Pitch_3->color = 8;
    ui_default_Static_Pitch_3->width = 16;
    ui_default_Static_Pitch_3->start_angle = 270;
    ui_default_Static_Pitch_3->end_angle = 271;

    ui_default_Static_Pitch_2->figure_tpye = 4;
    ui_default_Static_Pitch_2->layer = 0;
    ui_default_Static_Pitch_2->rx = 380;
    ui_default_Static_Pitch_2->ry = 380;
    ui_default_Static_Pitch_2->start_x = 960;
    ui_default_Static_Pitch_2->start_y = 540;
    ui_default_Static_Pitch_2->color = 8;
    ui_default_Static_Pitch_2->width = 16;
    ui_default_Static_Pitch_2->start_angle = 310;
    ui_default_Static_Pitch_2->end_angle = 311;

    ui_default_Static_Pitch_1->figure_tpye = 4;
    ui_default_Static_Pitch_1->layer = 0;
    ui_default_Static_Pitch_1->rx = 380;
    ui_default_Static_Pitch_1->ry = 380;
    ui_default_Static_Pitch_1->start_x = 960;
    ui_default_Static_Pitch_1->start_y = 540;
    ui_default_Static_Pitch_1->color = 8;
    ui_default_Static_Pitch_1->width = 16;
    ui_default_Static_Pitch_1->start_angle = 229;
    ui_default_Static_Pitch_1->end_angle = 230;

    ui_default_Static_Fric_L_Rect->figure_tpye = 1;
    ui_default_Static_Fric_L_Rect->layer = 0;
    ui_default_Static_Fric_L_Rect->start_x = 1598;
    ui_default_Static_Fric_L_Rect->start_y = 636;
    ui_default_Static_Fric_L_Rect->color = 8;
    ui_default_Static_Fric_L_Rect->width = 3;
    ui_default_Static_Fric_L_Rect->end_x = 1621;
    ui_default_Static_Fric_L_Rect->end_y = 736;

    ui_default_Static_Fric_M_Rect->figure_tpye = 1;
    ui_default_Static_Fric_M_Rect->layer = 0;
    ui_default_Static_Fric_M_Rect->start_x = 1630;
    ui_default_Static_Fric_M_Rect->start_y = 636;
    ui_default_Static_Fric_M_Rect->color = 8;
    ui_default_Static_Fric_M_Rect->width = 3;
    ui_default_Static_Fric_M_Rect->end_x = 1653;
    ui_default_Static_Fric_M_Rect->end_y = 736;

    ui_default_Static_Fric_R_Rect->figure_tpye = 1;
    ui_default_Static_Fric_R_Rect->layer = 0;
    ui_default_Static_Fric_R_Rect->start_x = 1662;
    ui_default_Static_Fric_R_Rect->start_y = 637;
    ui_default_Static_Fric_R_Rect->color = 8;
    ui_default_Static_Fric_R_Rect->width = 3;
    ui_default_Static_Fric_R_Rect->end_x = 1685;
    ui_default_Static_Fric_R_Rect->end_y = 737;

    ui_default_Static_Speed_Rect->figure_tpye = 1;
    ui_default_Static_Speed_Rect->layer = 0;
    ui_default_Static_Speed_Rect->start_x = 1570;
    ui_default_Static_Speed_Rect->start_y = 607;
    ui_default_Static_Speed_Rect->color = 8;
    ui_default_Static_Speed_Rect->width = 3;
    ui_default_Static_Speed_Rect->end_x = 1710;
    ui_default_Static_Speed_Rect->end_y = 627;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_13);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_13, sizeof(ui_default_Static_13));
}

void _ui_update_default_Static_13() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_13.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_13);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_13, sizeof(ui_default_Static_13));
}

void _ui_remove_default_Static_13() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_13.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_13);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_13, sizeof(ui_default_Static_13));
}
