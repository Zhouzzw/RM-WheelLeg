//
// Created by RM UI Designer
//

#include "ui_default_Static_3.h"
#include "string.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 3

ui_string_frame_t ui_default_Static_3;

ui_interface_string_t* ui_default_Static_Speed_Text = &ui_default_Static_3.option;

void _ui_init_default_Static_3() {
    ui_default_Static_3.option.figure_name[0] = FRAME_ID;
    ui_default_Static_3.option.figure_name[1] = GROUP_ID;
    ui_default_Static_3.option.figure_name[2] = START_ID;
    ui_default_Static_3.option.operate_tpyel = 1;
    ui_default_Static_3.option.figure_tpye = 7;
    ui_default_Static_3.option.layer = 0;
    ui_default_Static_3.option.font_size = 20;
    ui_default_Static_3.option.start_x = 1594;
    ui_default_Static_3.option.start_y = 612 - 10;
    ui_default_Static_3.option.color = 8;
    ui_default_Static_3.option.str_length = 5;
    ui_default_Static_3.option.width = 2;
    strcpy(ui_default_Static_Speed_Text->string, "Speed");

    ui_proc_string_frame(&ui_default_Static_3);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_3, sizeof(ui_default_Static_3));
}

void _ui_update_default_Static_3() {
    ui_default_Static_3.option.operate_tpyel = 2;

    ui_proc_string_frame(&ui_default_Static_3);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_3, sizeof(ui_default_Static_3));
}

void _ui_remove_default_Static_3() {
    ui_default_Static_3.option.operate_tpyel = 3;

    ui_proc_string_frame(&ui_default_Static_3);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_3, sizeof(ui_default_Static_3));
}