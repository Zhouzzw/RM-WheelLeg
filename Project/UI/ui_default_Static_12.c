//
// Created by RM UI Designer
//

#include "ui_default_Static_12.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 18
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Static_12;
ui_interface_arc_t *ui_default_Static_SuperCap_1 = (ui_interface_arc_t *)&(ui_default_Static_12.data[0]);
ui_interface_arc_t *ui_default_Static_SuperCap_2 = (ui_interface_arc_t *)&(ui_default_Static_12.data[1]);
ui_interface_arc_t *ui_default_Static_SuperCap_3 = (ui_interface_arc_t *)&(ui_default_Static_12.data[2]);
ui_interface_arc_t *ui_default_Static_SuperCap_4 = (ui_interface_arc_t *)&(ui_default_Static_12.data[3]);
ui_interface_arc_t *ui_default_Static_SuperCap_5 = (ui_interface_arc_t *)&(ui_default_Static_12.data[4]);
ui_interface_arc_t *ui_default_Static_Pitch_5 = (ui_interface_arc_t *)&(ui_default_Static_12.data[5]);
ui_interface_arc_t *ui_default_Static_Pitch_4 = (ui_interface_arc_t *)&(ui_default_Static_12.data[6]);

void _ui_init_default_Static_12() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_12.data[i].figure_name[0] = FRAME_ID;
        ui_default_Static_12.data[i].figure_name[1] = GROUP_ID;
        ui_default_Static_12.data[i].figure_name[2] = i + START_ID;
        ui_default_Static_12.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Static_12.data[i].operate_tpyel = 0;
    }

    ui_default_Static_SuperCap_1->figure_tpye = 4;
    ui_default_Static_SuperCap_1->layer = 0;
    ui_default_Static_SuperCap_1->rx = 380;
    ui_default_Static_SuperCap_1->ry = 380;
    ui_default_Static_SuperCap_1->start_x = 960;
    ui_default_Static_SuperCap_1->start_y = 540;
    ui_default_Static_SuperCap_1->color = 8;
    ui_default_Static_SuperCap_1->width = 16;
    ui_default_Static_SuperCap_1->start_angle = 49;
    ui_default_Static_SuperCap_1->end_angle = 50;

    ui_default_Static_SuperCap_2->figure_tpye = 4;
    ui_default_Static_SuperCap_2->layer = 0;
    ui_default_Static_SuperCap_2->rx = 380;
    ui_default_Static_SuperCap_2->ry = 380;
    ui_default_Static_SuperCap_2->start_x = 960;
    ui_default_Static_SuperCap_2->start_y = 540;
    ui_default_Static_SuperCap_2->color = 8;
    ui_default_Static_SuperCap_2->width = 16;
    ui_default_Static_SuperCap_2->start_angle = 130;
    ui_default_Static_SuperCap_2->end_angle = 131;

    ui_default_Static_SuperCap_3->figure_tpye = 4;
    ui_default_Static_SuperCap_3->layer = 0;
    ui_default_Static_SuperCap_3->rx = 360;
    ui_default_Static_SuperCap_3->ry = 360;
    ui_default_Static_SuperCap_3->start_x = 960;
    ui_default_Static_SuperCap_3->start_y = 540;
    ui_default_Static_SuperCap_3->color = 8;
    ui_default_Static_SuperCap_3->width = 16;
    ui_default_Static_SuperCap_3->start_angle = 90;
    ui_default_Static_SuperCap_3->end_angle = 91;

    ui_default_Static_SuperCap_4->figure_tpye = 4;
    ui_default_Static_SuperCap_4->layer = 0;
    ui_default_Static_SuperCap_4->rx = 360;
    ui_default_Static_SuperCap_4->ry = 360;
    ui_default_Static_SuperCap_4->start_x = 960;
    ui_default_Static_SuperCap_4->start_y = 540;
    ui_default_Static_SuperCap_4->color = 8;
    ui_default_Static_SuperCap_4->width = 16;
    ui_default_Static_SuperCap_4->start_angle = 115;
    ui_default_Static_SuperCap_4->end_angle = 116;

    ui_default_Static_SuperCap_5->figure_tpye = 4;
    ui_default_Static_SuperCap_5->layer = 0;
    ui_default_Static_SuperCap_5->rx = 360;
    ui_default_Static_SuperCap_5->ry = 360;
    ui_default_Static_SuperCap_5->start_x = 960;
    ui_default_Static_SuperCap_5->start_y = 540;
    ui_default_Static_SuperCap_5->color = 8;
    ui_default_Static_SuperCap_5->width = 16;
    ui_default_Static_SuperCap_5->start_angle = 65;
    ui_default_Static_SuperCap_5->end_angle = 66;

    ui_default_Static_Pitch_5->figure_tpye = 4;
    ui_default_Static_Pitch_5->layer = 0;
    ui_default_Static_Pitch_5->rx = 360;
    ui_default_Static_Pitch_5->ry = 360;
    ui_default_Static_Pitch_5->start_x = 960;
    ui_default_Static_Pitch_5->start_y = 540;
    ui_default_Static_Pitch_5->color = 8;
    ui_default_Static_Pitch_5->width = 16;
    ui_default_Static_Pitch_5->start_angle = 245;
    ui_default_Static_Pitch_5->end_angle = 246;

    ui_default_Static_Pitch_4->figure_tpye = 4;
    ui_default_Static_Pitch_4->layer = 0;
    ui_default_Static_Pitch_4->rx = 360;
    ui_default_Static_Pitch_4->ry = 360;
    ui_default_Static_Pitch_4->start_x = 960;
    ui_default_Static_Pitch_4->start_y = 540;
    ui_default_Static_Pitch_4->color = 8;
    ui_default_Static_Pitch_4->width = 16;
    ui_default_Static_Pitch_4->start_angle = 295;
    ui_default_Static_Pitch_4->end_angle = 296;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_12);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_12, sizeof(ui_default_Static_12));
}

void _ui_update_default_Static_12() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_12.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_12);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_12, sizeof(ui_default_Static_12));
}

void _ui_remove_default_Static_12() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_12.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_12);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_12, sizeof(ui_default_Static_12));
}
