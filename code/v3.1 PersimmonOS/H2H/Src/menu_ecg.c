#include "menu.h"

const menu_t menu_ecg =
{
  .item_num = 7,
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
        .string               = L"",
        .font                 = myriad_pro_semibold_16,
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
        .width                = 156,
        .height               = 100,
        .pos                  = {.x_pos = 0, .y_pos = 380},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"-?-",
        .font                 = myriad_pro_semibold_24,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_no_event
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 100,
        .pos                  = {.x_pos = 644, .y_pos = 380},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"Back",
        .font                 = myriad_pro_semibold_16,
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
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 161, .y_pos = 420},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"1 lead ECG",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_ecg_1_lead
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 322, .y_pos = 420},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"2 lead ECG",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_ecg_2_lead
      }
    },
    {
      .item_print_function = lcd_print_area,
      .item.area =
      {
        .width                = 156,
        .height               = 60,
        .pos                  = {.x_pos = 483, .y_pos = 420},
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_ACTIVE,
        .string               = L"6 lead ECG",
        .font                 = myriad_pro_semibold_16,
        .string_h_align       = CENTER,
        .string_v_align       = MID,
        .text_color           = COLOR_WHITE,
        .bg_color             = COLOR_BLUE,
        .border_color         = COLOR_BLACK,
        .event                = fsm_ecg_6_lead
      }
    },
    {
      .item_print_function = lcd_print_graph,
      .item.graph =
      {
        .width_legend         = 100,
        .width_graph          = 600,
        .height               = 280,
        .pos                  = {.x_pos = 50, .y_pos = 60},
        .legend_position      = LEFT,
        .border               = GUI_NO_BORDER,
        .is_active            = GUI_INACTIVE,
        .x_axis_full_scale    = 2000,
        .y_axis_full_scale    = 2000,
        .value_index          = 0,
        .second_to_last_value = 0,
        .last_value           = 0,
        .text_color           = COLOR_WHITE,
        .bg_legend_color      = COLOR_BLUE,
        .bg_graph_color       = COLOR_CYAN,
        .line_color           = COLOR_WHITE,
        .border_color         = COLOR_WHITE,
      }
    },
  }
};