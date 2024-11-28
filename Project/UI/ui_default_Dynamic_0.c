//
// Created by RM UI Designer
//

#include "ui_default_Dynamic_0.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 0
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Dynamic_0;
ui_interface_arc_t *ui_default_Dynamic_SuperCap = (ui_interface_arc_t *)&(ui_default_Dynamic_0.data[0]);
ui_interface_arc_t *ui_default_Dynamic_Pitch = (ui_interface_arc_t *)&(ui_default_Dynamic_0.data[1]);
ui_interface_line_t *ui_default_Dynamic_Fric_L = (ui_interface_line_t *)&(ui_default_Dynamic_0.data[2]);
ui_interface_line_t *ui_default_Dynamic_Fric_M = (ui_interface_line_t *)&(ui_default_Dynamic_0.data[3]);
ui_interface_line_t *ui_default_Dynamic_Fric_R = (ui_interface_line_t *)&(ui_default_Dynamic_0.data[4]);
ui_interface_line_t *ui_default_Dynamic_Speed = (ui_interface_line_t *)&(ui_default_Dynamic_0.data[5]);
ui_interface_arc_t *ui_default_Dynamic_Armor = (ui_interface_arc_t *)&(ui_default_Dynamic_0.data[6]);

void _ui_init_default_Dynamic_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Dynamic_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_Dynamic_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_Dynamic_0.data[i].figure_name[2] = i + START_ID;
        ui_default_Dynamic_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Dynamic_0.data[i].operate_tpyel = 0;
    }

    ui_default_Dynamic_SuperCap->figure_tpye = 4;
    ui_default_Dynamic_SuperCap->layer = 0;
    ui_default_Dynamic_SuperCap->rx = 380;
    ui_default_Dynamic_SuperCap->ry = 380;
    ui_default_Dynamic_SuperCap->start_x = 960;
    ui_default_Dynamic_SuperCap->start_y = 540;
    ui_default_Dynamic_SuperCap->color = 6;
    ui_default_Dynamic_SuperCap->width = 16;
    ui_default_Dynamic_SuperCap->start_angle = 50;
    ui_default_Dynamic_SuperCap->end_angle = 130;

    ui_default_Dynamic_Pitch->figure_tpye = 4;
    ui_default_Dynamic_Pitch->layer = 0;
    ui_default_Dynamic_Pitch->rx = 380;
    ui_default_Dynamic_Pitch->ry = 380;
    ui_default_Dynamic_Pitch->start_x = 960;
    ui_default_Dynamic_Pitch->start_y = 540;
    ui_default_Dynamic_Pitch->color = 6;
    ui_default_Dynamic_Pitch->width = 16;
    ui_default_Dynamic_Pitch->start_angle = 269;
    ui_default_Dynamic_Pitch->end_angle = 271;

    ui_default_Dynamic_Fric_L->figure_tpye = 0;
    ui_default_Dynamic_Fric_L->layer = 0;
    ui_default_Dynamic_Fric_L->start_x = 1601 + 8;
    ui_default_Dynamic_Fric_L->start_y = 638;
    ui_default_Dynamic_Fric_L->end_x = 1601 + 8;
    ui_default_Dynamic_Fric_L->end_y = 688;
    ui_default_Dynamic_Fric_L->color = 6;
    ui_default_Dynamic_Fric_L->width = 18;

    ui_default_Dynamic_Fric_M->figure_tpye = 0;
    ui_default_Dynamic_Fric_M->layer = 0;
    ui_default_Dynamic_Fric_M->start_x = 1633 + 8;
    ui_default_Dynamic_Fric_M->start_y = 638;
    ui_default_Dynamic_Fric_M->end_x = 1633 + 8;
    ui_default_Dynamic_Fric_M->end_y = 688;
    ui_default_Dynamic_Fric_M->color = 6;
    ui_default_Dynamic_Fric_M->width = 18;

    ui_default_Dynamic_Fric_R->figure_tpye = 0;
    ui_default_Dynamic_Fric_R->layer = 0;
    ui_default_Dynamic_Fric_R->start_x = 1665 + 8;
    ui_default_Dynamic_Fric_R->start_y = 638;
    ui_default_Dynamic_Fric_R->end_x = 1665 + 8;
    ui_default_Dynamic_Fric_R->end_y = 688;
    ui_default_Dynamic_Fric_R->color = 6;
    ui_default_Dynamic_Fric_R->width = 18;

    ui_default_Dynamic_Speed->figure_tpye = 0;
    ui_default_Dynamic_Speed->layer = 0;
    ui_default_Dynamic_Speed->start_x = 1570 + 2;
    ui_default_Dynamic_Speed->start_y = 609 + 8;
    ui_default_Dynamic_Speed->end_x = 1640 + 2;
    ui_default_Dynamic_Speed->end_y = 609 + 8;
    ui_default_Dynamic_Speed->color = 6;
    ui_default_Dynamic_Speed->width = 18;

    ui_default_Dynamic_Armor->figure_tpye = 4;
    ui_default_Dynamic_Armor->layer = 0;
    ui_default_Dynamic_Armor->rx = 132;
    ui_default_Dynamic_Armor->ry = 132;
    ui_default_Dynamic_Armor->start_x = 1639 + 3;
    ui_default_Dynamic_Armor->start_y = 668 + 2;
    ui_default_Dynamic_Armor->color = 6;
    ui_default_Dynamic_Armor->width = 10;
    ui_default_Dynamic_Armor->start_angle = 0;
    ui_default_Dynamic_Armor->end_angle = 360;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Dynamic_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Dynamic_0, sizeof(ui_default_Dynamic_0));
}

void _ui_update_default_Dynamic_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Dynamic_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Dynamic_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Dynamic_0, sizeof(ui_default_Dynamic_0));
}

void _ui_remove_default_Dynamic_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Dynamic_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Dynamic_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Dynamic_0, sizeof(ui_default_Dynamic_0));
}
