#include "menus.h"

const lcd_area_t menu_main_ecg =
{
  .width                = 0,
  .height               = 0,
  .pos                  = {.x_pos = 0, .y_pos = 0},
  .border               = NO_BORDER,
  .is_active            = ACTIVE,
  .is_clicked           = GUI_NOT_CLICKED,
  .string               = "",
  .font                 = myriad_pro_semibold11x16,
  .string_width         = 0,
  .string_height        = 0,
  .string_h_align       = CENTER,
  .string_v_align       = MID,
  .text_color           = COLOR_WHITE,
  .bg_color             = COLOR_BLACK,
  .border_color         = COLOR_BLACK
};