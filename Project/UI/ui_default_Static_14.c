//
// Created by RM UI Designer
//

#include "ui_default_Static_14.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 32
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Static_14;
ui_interface_round_t *ui_default_Static_Round = (ui_interface_round_t *)&(ui_default_Static_14.data[0]);
ui_interface_round_t *ui_default_Static_Star_Round = (ui_interface_round_t *)&(ui_default_Static_14.data[1]);

void _ui_init_default_Static_14() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_14.data[i].figure_name[0] = FRAME_ID;
        ui_default_Static_14.data[i].figure_name[1] = GROUP_ID;
        ui_default_Static_14.data[i].figure_name[2] = i + START_ID;
        ui_default_Static_14.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Static_14.data[i].operate_tpyel = 0;
    }

    ui_default_Static_Round->figure_tpye = 2;
    ui_default_Static_Round->layer = 0;
    ui_default_Static_Round->r = 120;
    ui_default_Static_Round->start_x = 1642;
    ui_default_Static_Round->start_y = 670;
    ui_default_Static_Round->color = 8;
    ui_default_Static_Round->width = 5;

    ui_default_Static_Star_Round->figure_tpye = 2;
    ui_default_Static_Star_Round->layer = 0;
    ui_default_Static_Star_Round->r = 20;
    ui_default_Static_Star_Round->start_x = 959;
    ui_default_Static_Star_Round->start_y = 541;
    ui_default_Static_Star_Round->color = 8;
    ui_default_Static_Star_Round->width = 2;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_14);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_14, sizeof(ui_default_Static_14));
}

void _ui_update_default_Static_14() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_14.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_14);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_14, sizeof(ui_default_Static_14));
}

void _ui_remove_default_Static_14() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_14.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_14);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_14, sizeof(ui_default_Static_14));
}
