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
        .width                = 700,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 250},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_RED,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      },
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 700,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 200},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_RED,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      },
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 700,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 150},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_RED,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      },
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 700,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 100},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_RED,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      },
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 700,
        .height               = 50,
        .pos                  = {.x_pos = 30, .y_pos = 50},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_RED,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      },
    }
  }
};