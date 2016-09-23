#include "menu.h"

menu_t graph_ecg =
{
  .item_num = 1,
  .items =
  {
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