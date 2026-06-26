//
// Created by RM UI Designer
//

#include "ui_default_Static_6.h"
#include "string.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 6

ui_string_frame_t ui_default_Static_6;

ui_interface_string_t* ui_default_Static_Star_Text_1 = &ui_default_Static_6.option;

void _ui_init_default_Static_6() {
    ui_default_Static_6.option.figure_name[0] = FRAME_ID;
    ui_default_Static_6.option.figure_name[1] = GROUP_ID;
    ui_default_Static_6.option.figure_name[2] = START_ID;
    ui_default_Static_6.option.operate_tpyel = 1;
    ui_default_Static_6.option.figure_tpye = 7;
    ui_default_Static_6.option.layer = 0;
    ui_default_Static_6.option.font_size = 15;
    ui_default_Static_6.option.start_x = 891;
    ui_default_Static_6.option.start_y = 521;
    ui_default_Static_6.option.color = 8;
    ui_default_Static_6.option.str_length = 1;
    ui_default_Static_6.option.width = 2;
    strcpy(ui_default_Static_Star_Text_1->string, "1");

    ui_proc_string_frame(&ui_default_Static_6);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_6, sizeof(ui_default_Static_6));
}

void _ui_update_default_Static_6() {
    ui_default_Static_6.option.operate_tpyel = 2;

    ui_proc_string_frame(&ui_default_Static_6);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_6, sizeof(ui_default_Static_6));
}

void _ui_remove_default_Static_6() {
    ui_default_Static_6.option.operate_tpyel = 3;

    ui_proc_string_frame(&ui_default_Static_6);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_6, sizeof(ui_default_Static_6));
}