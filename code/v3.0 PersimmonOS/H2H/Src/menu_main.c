#include "menus.h"

menu_t menu_main =
{
  .item_num = 3,
  .items =
  {
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 400,
        .height               = 120,
        .pos                  = {.x_pos = 400, .y_pos = 80},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "ECG",
        .font                 = myriad_pro_semibold17x23,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_ecg
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 400,
        .height               = 120,
        .pos                  = {.x_pos = 400, .y_pos = 220},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "H2H",
        .font                 = myriad_pro_semibold17x23,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_h2h
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 400,
        .height               = 120,
        .pos                  = {.x_pos = 400, .y_pos = 360},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "Settings",
        .font                 = myriad_pro_semibold17x23,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_settings
      }
    }
  }
};