#include "item_area.h"

gui_status_t item_area_init(item_area_t *area,
                            uint16_t width, uint16_t height,
                            uint16_t x_pos, uint16_t y_pos,
                            gui_border_t border,
                            gui_active_t is_active,
                            char *string, const uint8_t *font,
                            gui_h_align_t string_h_align, gui_v_align_t string_v_align,
                            color_t text_color, color_t bg_color, color_t border_color,
                            fsm_event_f event)
{
  gui_status_t status;

  /* Set area size */
  area->width = width;
  area->height = height;

  /* Set area position */
  area->pos.x_pos = x_pos;
  area->pos.y_pos = y_pos;

  /* Set area border */
  area->border = border;

  /* Set area text */
  area->font = font;
  status = item_area_set_text(area, string);

  /* Set text alignment */
  area->string_h_align = string_h_align;
  area->string_v_align = string_v_align;

  /* Set behaviour default parameters */
  area->is_active = is_active;

  /* Set colors */
  area->text_color = text_color;
  area->bg_color = bg_color;
  area->border_color = border_color;

  /* Set triggered event */
  area->event = event;

  return status;
}

gui_status_t item_area_set_text(item_area_t *area, char *string)
{
  uint16_t string_height = 0;
  uint16_t string_width = 0;

  uint8_t font_starting_char;           // Font array starting character (character number == 0)

  char character = 0;                   // Iterating char
  uint32_t character_index = 0;         // String iteration

  uint8_t character_number;             // Character number in font array
  uint16_t character_width = 0;         // Width for every character

  /*
   * Check if text_area fits into area
   */

  /* Calculate string height */
  string_height = area->font[6];
  /* Check text_area height */
  if (area->height < string_height)
  {
    return GUI_SIZE_ERROR;
  }

  /* Get starting character in font array */
  font_starting_char = area->font[2];
  /* Calculate string width */
  while ((character = string[character_index]) != 0)
  {
    /* Get character number */
    character_number = character - font_starting_char;
    /* Get character width */
    character_width = area->font[8 + 4*character_number];
    /* Add character width to the accumulated string width */
    string_width += character_width;

    /* Check text_area width */
    if (area->width < string_width)
    {
      /* If string does not fit into area, close string and return error */
      area->string[character_index] = 0;
      return GUI_SIZE_ERROR;
    }
    else
    {
      /* If string still fits into area
       * store character in area->string
       */
      if (character_index >= 49)
      {
        /* If string is too long for reserved memory space */
        /* Close string and return error */
        area->string[character_index] = 0;
        return GUI_SIZE_ERROR;
      }
      else
      {
        area->string[character_index] = character;
      }
    }

    /* Next character */
    character_index++;
  }
  /* Close string */
  area->string[character_index] = 0;

  /* Save height and width */
  area->string_height = string_height;
  area->string_width = string_width;

  return GUI_OK;
}