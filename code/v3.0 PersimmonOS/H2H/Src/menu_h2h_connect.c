#include "menu.h"

menu_t menu_h2h_connect =
{
  .item_num = 4,
  .items =
  {
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 800,
        .height               = 450,
        .pos                  = {.x_pos = 0, .y_pos = 30},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = "",
        .font                 = myriad_pro_semibold17x23,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 150,
        .height               = 100,
        .pos                  = {.x_pos = 650, .y_pos = 380},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "Back",
        .font                 = myriad_pro_semibold17x23,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_back
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 610,
        .height               = 100,
        .pos                  = {.x_pos = 20, .y_pos = 380},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "Connect",
        .font                 = myriad_pro_semibold17x23,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_h2h_connect
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 760,
        .height               = 300,
        .pos                  = {.x_pos = 20, .y_pos = 60},
        .border               = GUI_RECTANGLE,
        .is_active            = GUI_ACTIVE,
        .string               = "",
        .font                 = myriad_pro_semibold17x23,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_WHITE,
        .event                = fsm_no_event
      }
    }
  }
};