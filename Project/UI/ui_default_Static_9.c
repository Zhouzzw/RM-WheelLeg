//
// Created by RM UI Designer
//

#include "ui_default_Static_9.h"
#include "string.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 9

ui_string_frame_t ui_default_Static_9;

ui_interface_string_t* ui_default_Static_Star_Text_4 = &ui_default_Static_9.option;

void _ui_init_default_Static_9() {
    ui_default_Static_9.option.figure_name[0] = FRAME_ID;
    ui_default_Static_9.option.figure_name[1] = GROUP_ID;
    ui_default_Static_9.option.figure_name[2] = START_ID;
    ui_default_Static_9.option.operate_tpyel = 1;
    ui_default_Static_9.option.figure_tpye = 7;
    ui_default_Static_9.option.layer = 0;
    ui_default_Static_9.option.font_size = 15;
    ui_default_Static_9.option.start_x = 738;
    ui_default_Static_9.option.start_y = 399;
    ui_default_Static_9.option.color = 8;
    ui_default_Static_9.option.str_length = 1;
    ui_default_Static_9.option.width = 2;
    strcpy(ui_default_Static_Star_Text_4->string, "4");

    ui_proc_string_frame(&ui_default_Static_9);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_9, sizeof(ui_default_Static_9));
}

void _ui_update_default_Static_9() {
    ui_default_Static_9.option.operate_tpyel = 2;

    ui_proc_string_frame(&ui_default_Static_9);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_9, sizeof(ui_default_Static_9));
}

void _ui_remove_default_Static_9() {
    ui_default_Static_9.option.operate_tpyel = 3;

    ui_proc_string_frame(&ui_default_Static_9);
    SEND_MESSAGE((uint8_t *) &ui_default_Static_9, sizeof(ui_default_Static_9));
}