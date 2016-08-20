#include "menus.h"

menu_t menu_welcome =
{
  .item_num = 4,
  .items =
  {
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 200,
        .height               = 50,
        .pos                  = {.x_pos = 300, .y_pos = 215},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "Persimmon Access Device",
        .font                 = myriad_pro_semibold17x23,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
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
        .height               = 30,
        .pos                  = {.x_pos = 300, .y_pos = 265},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "H2H Main Board v1.1",
        .font                 = myriad_pro_semibold11x16,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 160,
        .height               = 30,
        .pos                  = {.x_pos = 20, .y_pos = 420},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "Samuel López Asunción",
        .font                 = myriad_pro_semibold11x16,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = LEFT,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 160,
        .height               = 30,
        .pos                  = {.x_pos = 620, .y_pos = 420},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = "Tomás Valencia Noheda",
        .font                 = myriad_pro_semibold11x16,
        .string_width         = 0,
        .string_height        = 0,
        .string_h_align       = RIGHT,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLACK,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    }
  }
};