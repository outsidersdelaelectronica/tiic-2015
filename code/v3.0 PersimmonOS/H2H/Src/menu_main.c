#include "menus.h"

const item_t menu_main_ecg =
{
  .area =
  {
    .width                = 200,
    .height               = 100,
    .pos                  = {.x_pos = 0, .y_pos = 0},
    .border               = GUI_NO_BORDER,
    .is_active            = GUI_ACTIVE,
    .string               = "Test",
    .font                 = myriad_pro_semibold11x16,
    .string_width         = 0,
    .string_height        = 0,
    .string_h_align       = CENTER,
    .string_v_align       = MID,
    .text_color           = COLOR_WHITE,
    .bg_color             = COLOR_BLUE,
    .border_color         = COLOR_BLACK
  }
};