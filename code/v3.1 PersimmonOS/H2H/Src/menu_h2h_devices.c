#include "menu.h"

menu_t menu_h2h_devices =
{
  .item_num = 6,
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
        .is_active            = GUI_ACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_h2h_connect_1
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
        .is_active            = GUI_ACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_h2h_connect_2
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
        .is_active            = GUI_ACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_h2h_connect_3
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
        .is_active            = GUI_ACTIVE,
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_BLACK,
        .bg_color             = COLOR_CYAN,
        .border_color         = COLOR_CYAN,
        .event                = fsm_h2h_connect_4
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
        .string               = L"\xF064",
        .font                 = fontawesome_back,
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
        .string               = L"Connect",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_h2h_start_connect
      }
    }
  }
};