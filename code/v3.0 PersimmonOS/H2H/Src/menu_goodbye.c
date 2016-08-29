#include "menu.h"

menu_t menu_goodbye =
{
  .item_num = 2,
  .items =
  {
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 800,
        .height               = 480,
        .pos                  = {.x_pos = 0, .y_pos = 0},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_24,
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
        .width                = 200,
        .height               = 50,
        .pos                  = {.x_pos = 300, .y_pos = 215},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Bye!",
        .font                 = myriad_pro_semibold_24,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    }
  }
};