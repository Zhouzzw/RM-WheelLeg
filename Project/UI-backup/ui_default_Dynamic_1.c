//
// Created by RM UI Designer
//

#include "ui_default_Dynamic_1.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 7
#define OBJ_NUM 4
#define FRAME_OBJ_NUM 5

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Dynamic_1;
ui_interface_number_t *ui_default_Dynamic_Chassis_1 = (ui_interface_number_t *)&(ui_default_Dynamic_1.data[0]);
ui_interface_number_t *ui_default_Dynamic_Chassis_2 = (ui_interface_number_t *)&(ui_default_Dynamic_1.data[1]);
ui_interface_number_t *ui_default_Dynamic_Chassis_3 = (ui_interface_number_t *)&(ui_default_Dynamic_1.data[2]);
ui_interface_number_t *ui_default_Dynamic_Chassis_4 = (ui_interface_number_t *)&(ui_default_Dynamic_1.data[3]);

void _ui_init_default_Dynamic_1() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Dynamic_1.data[i].figure_name[0] = FRAME_ID;
        ui_default_Dynamic_1.data[i].figure_name[1] = GROUP_ID;
        ui_default_Dynamic_1.data[i].figure_name[2] = i + START_ID;
        ui_default_Dynamic_1.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Dynamic_1.data[i].operate_tpyel = 0;
    }

    ui_default_Dynamic_Chassis_1->figure_tpye = 5;
    ui_default_Dynamic_Chassis_1->layer = 0;
    ui_default_Dynamic_Chassis_1->font_size = 10;
    ui_default_Dynamic_Chassis_1->start_x = 1556;
    ui_default_Dynamic_Chassis_1->start_y = 743 - 10;
    ui_default_Dynamic_Chassis_1->color = 6;
    ui_default_Dynamic_Chassis_1->number = 1110;
    ui_default_Dynamic_Chassis_1->width = 1;

    ui_default_Dynamic_Chassis_2->figure_tpye = 5;
    ui_default_Dynamic_Chassis_2->layer = 0;
    ui_default_Dynamic_Chassis_2->font_size = 10;
    ui_default_Dynamic_Chassis_2->start_x = 1689;
    ui_default_Dynamic_Chassis_2->start_y = 745 - 10;
    ui_default_Dynamic_Chassis_2->color = 6;
    ui_default_Dynamic_Chassis_2->number = 1110;
    ui_default_Dynamic_Chassis_2->width = 1;

    ui_default_Dynamic_Chassis_3->figure_tpye = 5;
    ui_default_Dynamic_Chassis_3->layer = 0;
    ui_default_Dynamic_Chassis_3->font_size = 10;
    ui_default_Dynamic_Chassis_3->start_x = 1555;
    ui_default_Dynamic_Chassis_3->start_y = 657 - 10;
    ui_default_Dynamic_Chassis_3->color = 6;
    ui_default_Dynamic_Chassis_3->number = 1110;
    ui_default_Dynamic_Chassis_3->width = 1;

    ui_default_Dynamic_Chassis_4->figure_tpye = 5;
    ui_default_Dynamic_Chassis_4->layer = 0;
    ui_default_Dynamic_Chassis_4->font_size = 10;
    ui_default_Dynamic_Chassis_4->start_x = 1689;
    ui_default_Dynamic_Chassis_4->start_y = 658 - 10;
    ui_default_Dynamic_Chassis_4->color = 6;
    ui_default_Dynamic_Chassis_4->number = 1110;
    ui_default_Dynamic_Chassis_4->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Dynamic_1);
    SEND_MESSAGE((uint8_t *) &ui_default_Dynamic_1, sizeof(ui_default_Dynamic_1));
}

void _ui_update_default_Dynamic_1() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Dynamic_1.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Dynamic_1);
    SEND_MESSAGE((uint8_t *) &ui_default_Dynamic_1, sizeof(ui_default_Dynamic_1));
}

void _ui_remove_default_Dynamic_1() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Dynamic_1.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Dynamic_1);
    SEND_MESSAGE((uint8_t *) &ui_default_Dynamic_1, sizeof(ui_default_Dynamic_1));
}
