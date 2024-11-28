//
// Created by RM UI Designer
//

#include "ui_default_Static_11.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 11
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Static_11;
ui_interface_line_t *ui_default_Static_Star_1 = (ui_interface_line_t *)&(ui_default_Static_11.data[0]);
ui_interface_line_t *ui_default_Static_Star_2 = (ui_interface_line_t *)&(ui_default_Static_11.data[1]);
ui_interface_line_t *ui_default_Static_Star_3 = (ui_interface_line_t *)&(ui_default_Static_11.data[2]);
ui_interface_line_t *ui_default_Static_Star_4 = (ui_interface_line_t *)&(ui_default_Static_11.data[3]);
ui_interface_line_t *ui_default_Static_Star_5 = (ui_interface_line_t *)&(ui_default_Static_11.data[4]);
ui_interface_line_t *ui_default_Static_Star_6 = (ui_interface_line_t *)&(ui_default_Static_11.data[5]);
ui_interface_line_t *ui_default_Static_Star_7 = (ui_interface_line_t *)&(ui_default_Static_11.data[6]);

void _ui_init_default_Static_11() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_11.data[i].figure_name[0] = FRAME_ID;
        ui_default_Static_11.data[i].figure_name[1] = GROUP_ID;
        ui_default_Static_11.data[i].figure_name[2] = i + START_ID;
        ui_default_Static_11.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Static_11.data[i].operate_tpyel = 0;
    }

    ui_default_Static_Star_1->figure_tpye = 0;
    ui_default_Static_Star_1->layer = 0;
    ui_default_Static_Star_1->start_x = 660;
    ui_default_Static_Star_1->start_y = 539;
    ui_default_Static_Star_1->end_x = 1261;
    ui_default_Static_Star_1->end_y = 540;
    ui_default_Static_Star_1->color = 8;
    ui_default_Static_Star_1->width = 3;

    ui_default_Static_Star_2->figure_tpye = 0;
    ui_default_Static_Star_2->layer = 0;
    ui_default_Static_Star_2->start_x = 910;
    ui_default_Static_Star_2->start_y = 500;
    ui_default_Static_Star_2->end_x = 1010;
    ui_default_Static_Star_2->end_y = 500;
    ui_default_Static_Star_2->color = 8;
    ui_default_Static_Star_2->width = 1;

    ui_default_Static_Star_3->figure_tpye = 0;
    ui_default_Static_Star_3->layer = 0;
    ui_default_Static_Star_3->start_x = 860;
    ui_default_Static_Star_3->start_y = 460;
    ui_default_Static_Star_3->end_x = 1061;
    ui_default_Static_Star_3->end_y = 460;
    ui_default_Static_Star_3->color = 8;
    ui_default_Static_Star_3->width = 1;

    ui_default_Static_Star_4->figure_tpye = 0;
    ui_default_Static_Star_4->layer = 0;
    ui_default_Static_Star_4->start_x = 810;
    ui_default_Static_Star_4->start_y = 420;
    ui_default_Static_Star_4->end_x = 1110;
    ui_default_Static_Star_4->end_y = 420;
    ui_default_Static_Star_4->color = 8;
    ui_default_Static_Star_4->width = 1;

    ui_default_Static_Star_5->figure_tpye = 0;
    ui_default_Static_Star_5->layer = 0;
    ui_default_Static_Star_5->start_x = 760;
    ui_default_Static_Star_5->start_y = 380;
    ui_default_Static_Star_5->end_x = 1160;
    ui_default_Static_Star_5->end_y = 380;
    ui_default_Static_Star_5->color = 8;
    ui_default_Static_Star_5->width = 1;

    ui_default_Static_Star_6->figure_tpye = 0;
    ui_default_Static_Star_6->layer = 0;
    ui_default_Static_Star_6->start_x = 710;
    ui_default_Static_Star_6->start_y = 340;
    ui_default_Static_Star_6->end_x = 1210;
    ui_default_Static_Star_6->end_y = 340;
    ui_default_Static_Star_6->color = 8;
    ui_default_Static_Star_6->width = 1;

    ui_default_Static_Star_7->figure_tpye = 0;
    ui_default_Static_Star_7->layer = 0;
    ui_default_Static_Star_7->start_x = 960;
    ui_default_Static_Star_7->start_y = 600;
    ui_default_Static_Star_7->end_x = 960;
    ui_default_Static_Star_7->end_y = 250;
    ui_default_Static_Star_7->color = 8;
    ui_default_Static_Star_7->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_11);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_11, sizeof(ui_default_Static_11));
}

void _ui_update_default_Static_11() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_11.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_11);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_11, sizeof(ui_default_Static_11));
}

void _ui_remove_default_Static_11() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Static_11.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Static_11);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_11, sizeof(ui_default_Static_11));
}
