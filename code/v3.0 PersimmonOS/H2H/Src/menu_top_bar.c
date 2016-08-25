#include "menu.h"

menu_t menu_top_bar =
{
  .item_num = 3,
  .items =
  {
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 720,
        .height               = 30,
        .pos                  = {.x_pos = 0, .y_pos = 0},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_WHITE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 120,
        .height               = 30,
        .pos                  = {.x_pos = 590, .y_pos = 0},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = RIGHT,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_WHITE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 80,
        .height               = 30,
        .pos                  = {.x_pos = 720, .y_pos = 0},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_RED,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    }
  }
};