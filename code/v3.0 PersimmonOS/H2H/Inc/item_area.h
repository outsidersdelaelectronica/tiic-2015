/*
 * item_area.h
 *
 *  Created on: 27/05/2016
 *      Author: slopez
 */

#ifndef ITEM_AREA_H_
#define ITEM_AREA_H_

#include "item_gui.h"

#include "lcd_position.h"
#include "color.h"

typedef struct item_area_t
{
  uint16_t width;
  uint16_t height;

  lcd_pos_t pos;

  gui_border_t border;
  gui_active_t is_active;

  char string[50];
  const uint8_t *font;

  uint16_t string_width;
  uint16_t string_height;

  gui_h_align_t string_h_align;
  gui_v_align_t string_v_align;

  color_t text_color;
  color_t bg_color;
  color_t border_color;
}
item_area_t;

gui_status_t item_area_init(item_area_t *area,
                            uint16_t width, uint16_t height,
                            uint16_t x_pos, uint16_t y_pos,
                            gui_border_t border,
                            gui_active_t is_active,
                            char *string, const uint8_t *font,
                            gui_h_align_t string_h_align, gui_v_align_t string_v_align,
                            color_t text_color, color_t bg_color, color_t border_color);

/*
 * Getters
 */
gui_active_t  item_area_get_is_active(item_area_t *area);

/*
 * Setters
 */
void          item_area_set_is_active(item_area_t *area, gui_active_t is_active);
gui_status_t  item_area_set_text(item_area_t *area, char *string);

#endif /* ITEM_AREA_H_ */