#include "menu.h"

menu_t menu_h2h_devices =
{
  .item_num = 5,
  .items =
  {
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 730,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 80},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_CYAN,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 730,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 140},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_CYAN,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 730,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 200},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_CYAN,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 730,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 260},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_CYAN,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 730,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 320},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_CYAN,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_no_event
      }
    },
  }
};