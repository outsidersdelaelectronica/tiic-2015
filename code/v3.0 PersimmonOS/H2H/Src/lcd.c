/*
 * lcd.c
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "lcd.h"

/* LCD init parameters */
uint16_t lcd_param_pll_mn[]                     = {0x0023, 0x0022, 0x0004};     //M=35, N=2, PLL freq=120 Mhz
uint16_t lcd_param_pll[]                        = {0x0001};
uint16_t lcd_param_pll_lock[]                   = {0x0003};

uint16_t lcd_param_lshift_freq[]                = {0x0003, 0x0033, 0x0033};

uint16_t lcd_param_lcd_mode[]                   = {0x0020, 0x0000, 0x0003, 0x001F, 0x0001, 0x00DF, 0x0000};
uint16_t lcd_param_hori_period[]                = {0x0004, 0x001F, 0x0000, 0x00D2, 0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_vert_period[]                = {0x0002, 0x000C, 0x0000, 0x0022, 0x0000, 0x0000, 0x0000};

uint16_t lcd_param_gpio_conf[]                  = {0x000F, 0x0001};
uint16_t lcd_param_gpio_value[]                 = {0x0001};

uint16_t lcd_param_pixel_data_interface[]       = {0x0003};

uint16_t lcd_param_address_mode[]               = {0x0008};
uint16_t lcd_param_column_address[]             = {0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_page_address[]               = {0x0000, 0x0000, 0x0000, 0x0000};

uint16_t lcd_param_gamma_curve[]                = {0x0001};

uint16_t lcd_param_partial_area[]               = {0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_scroll_area[]                = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_scroll_start[]               = {0x0000, 0x0000};

uint16_t lcd_param_pwm_conf[]                   = {0x000C, 0x0000, 0x0001, 0x0000, 0x0000, 0x0001};
uint16_t lcd_param_dbc_conf[]                   = {0x0042};
uint16_t lcd_param_post_proc[]                  = {0x0040, 0x0080, 0x0040, 0x0001};

/**
  * @brief  Initializes LCD module
  * @retval None
*/
void lcd_init(lcd_t *lcd, SRAM_HandleTypeDef *hsram,
              uint32_t *lcd_reg, uint32_t *lcd_data,
              uint16_t lcd_x_size, uint16_t lcd_y_size)
{
  uint16_t lcd_device_descriptor[5];
  uint8_t device_descriptor_ok;
  color_t bg_color;

  lcd_hal_init(lcd, hsram, lcd_reg, lcd_data);

  lcd->lcd_x_size = lcd_x_size;
  lcd->lcd_y_size = lcd_y_size;
  bg_color = (color_t) COLOR_BLACK;

  /* Check device descriptor */
  lcd_hal_read_reg(lcd, LCD_READ_DDB, lcd_device_descriptor, 5);
  device_descriptor_ok = (lcd_device_descriptor[0] == 0x0001) &&        // Supplier ID (high byte)
                         (lcd_device_descriptor[1] == 0x0057) &&        // Supplier ID (low byte)
                         (lcd_device_descriptor[2] == 0x0061) &&        // Product ID
                         (lcd_device_descriptor[3] == 0x0001) &&        // Revision code
                         (lcd_device_descriptor[4] == 0x00FF);          // End code

  if (device_descriptor_ok)
  {
    color_t bg_color = (color_t) COLOR_BLACK;

    /* Set PLL */
    lcd_hal_write_reg(lcd, LCD_SET_PLL_MN, lcd_param_pll_mn, 3);
    /* Enable PLL */
    lcd_hal_write_reg(lcd, LCD_SET_PLL, lcd_param_pll, 1);
    /* Wait for PLL to start up */
    HAL_Delay(1);
    /* Lock PLL */
    lcd_hal_write_reg(lcd, LCD_SET_PLL, lcd_param_pll_lock, 1);

    /* Soft-reset LCD controller */
    lcd_hal_write_command(lcd, LCD_SOFT_RESET);
    HAL_Delay(10);

    /* Set the pixel clock frequency */
    lcd_hal_write_reg(lcd, LCD_SET_LSHIFT_FREQ, lcd_param_lshift_freq, 3);

    /* Set LCD modes, horizontal and vertical sync, and pixel data interface */
    lcd_hal_write_reg(lcd, LCD_SET_LCD_MODE, lcd_param_lcd_mode, 7);
    lcd_hal_write_reg(lcd, LCD_SET_HORI_PERIOD, lcd_param_hori_period, 8);
    lcd_hal_write_reg(lcd, LCD_SET_VERT_PERIOD, lcd_param_vert_period, 7);

    /* Set GPIO configuration */
    lcd_hal_write_reg(lcd, LCD_SET_GPIO_CONF, lcd_param_gpio_conf, 2);
    lcd_hal_write_reg(lcd, LCD_SET_GPIO_VALUE, lcd_param_gpio_value, 1);

    /* Set pixel data format */
    lcd_hal_write_reg(lcd, LCD_SET_PIXEL_DATA_INTERFACE, lcd_param_pixel_data_interface, 1);

    /* Set address scan mode and column and row starting pixel */
    lcd_hal_write_reg(lcd, LCD_SET_ADDRESS_MODE, lcd_param_address_mode, 1);
    lcd_hal_write_reg(lcd, LCD_SET_COLUMN_ADDRESS, lcd_param_column_address, 4);
    lcd_hal_write_reg(lcd, LCD_SET_PAGE_ADDRESS, lcd_param_page_address, 4);

    /* Set gamma curve */
    lcd_hal_write_reg(lcd, LCD_SET_GAMMA_CURVE, lcd_param_gamma_curve, 1);

    /* Set partial area and scrolling configuration */
    lcd_hal_write_reg(lcd, LCD_SET_PARTIAL_AREA, lcd_param_partial_area, 4);
    lcd_hal_write_reg(lcd, LCD_SET_SCROLL_AREA, lcd_param_scroll_area, 6);
    lcd_hal_write_reg(lcd, LCD_SET_SCROLL_START, lcd_param_scroll_start, 2);

    /* Set backlight brightness and image post processing defaults */
    lcd_hal_write_reg(lcd, LCD_SET_PWM_CONF, lcd_param_pwm_conf, 6);
    lcd_hal_write_reg(lcd, LCD_SET_DBC_CONF, lcd_param_dbc_conf, 1);
    lcd_hal_write_reg(lcd, LCD_SET_POST_PROC, lcd_param_post_proc, 4);

    /* Turn display off and set TE signal off */
    lcd_hal_write_command(lcd, LCD_SET_TEAR_OFF);

    /* Clean frame buffer */
    lcd_draw_background(lcd, &bg_color);

    /* Initialize lcd structure */
    lcd->lcd_x_size = LCD_X_SIZE;
    lcd->lcd_y_size = LCD_Y_SIZE;
    lcd->backlight_level = 100;
    
    /* Exit sleep mode and turn display on */
    lcd_hal_write_command(lcd, LCD_EXIT_SLEEP_MODE);
    HAL_Delay(10);
    //lcd_hal_write_command(lcd, LCD_SET_DISPLAY_ON);    //LCD_EXIT_SLEEP_MODE triggers LCD_SET_DISPLAY_ON
  }
}

gui_status_t lcd_print_area(lcd_t *lcd, void *item)
{
  uint16_t x_pos, x_pos_width;
  uint16_t y_pos, y_pos_height;

  uint16_t string_x_pos, string_y_pos;

  color_t text_color;
  color_t bg_color;
  color_t border_color;

  /* Cast pointer */
  lcd_area_t *area = (lcd_area_t *) item;

  /* Check if area is clicked */
  if (area->is_clicked == GUI_CLICKED)
  {
    /* Invert colors */
    color_get_inverse(&(area->text_color), &text_color);
    color_get_inverse(&(area->bg_color), &bg_color);
    color_get_inverse(&(area->border_color), &border_color);
  }
  else
  {
    text_color = area->text_color;
    bg_color = area->bg_color;
    border_color = area->border_color;
  }

  /* Check if area lies within lcd boundaries */
  if (area->pos.x_pos + area->width > lcd->lcd_x_size)
  {
    return GUI_SIZE_ERROR;
  }
  if (area->pos.y_pos + area->height > lcd->lcd_y_size)
  {
    return GUI_SIZE_ERROR;
  }

  /* Draw area rectangle */
  lcd_draw_rectangle(lcd,
                     area->pos.x_pos, area->width,
                     area->pos.y_pos, area->height,
                     &bg_color);

  /* Draw border if present */
  if (area->border == RECTANGLE)
  {
    x_pos = area->pos.x_pos;
    y_pos = area->pos.y_pos;
    x_pos_width = area->pos.x_pos + area->width - 1;
    y_pos_height = area->pos.y_pos + area->height - 1;

    /* Top line */
    lcd_draw_line(lcd,
                  x_pos, y_pos,
                  x_pos_width, y_pos,
                  &border_color);
    /* Bottom line */
    lcd_draw_line(lcd,
                  x_pos, y_pos_height,
                  x_pos_width, y_pos_height,
                  &border_color);
    /* Left line */
    lcd_draw_line(lcd,
                  x_pos, y_pos,
                  x_pos, y_pos_height,
                  &border_color);
    /* Right line */
    lcd_draw_line(lcd,
                  x_pos_width, y_pos,
                  x_pos_width, y_pos_height,
                  &border_color);
  }

  /* Draw text */
  switch (area->string_h_align)
  {
    case LEFT:
      string_x_pos = area->pos.x_pos;
      break;
    case CENTER:
      string_x_pos = area->pos.x_pos + (area->width >> 1) - (area->string_width >> 1);
      break;
    case RIGHT:
      string_x_pos = area->pos.x_pos + area->width - area->string_width;
      break;
    default:
      string_x_pos = area->pos.x_pos;
      break;
  }

  switch (area->string_v_align)
  {
    case TOP:
      string_y_pos = area->pos.y_pos;
      break;
    case MID:
      string_y_pos = area->pos.y_pos + (area->height >> 1) - (area->string_height >> 1);
      break;
    case RIGHT:
      string_y_pos = area->pos.y_pos + area->height - area->string_height;
      break;
    default:
      string_y_pos = area->pos.y_pos;
      break;
  }

  lcd_draw_string(lcd,
                  area->string,
                  area->font,
                  &text_color, &bg_color,
                  string_x_pos, string_y_pos);

  return GUI_OK;
}

gui_status_t lcd_print_graph(lcd_t *lcd, void *item)
{
  uint16_t x_pos, x_pos_width;
  uint16_t y_pos, y_pos_height;

  color_t text_color;
  color_t bg_legend_color;
  color_t border_color;

  /* Cast pointer */
  lcd_graph_t *graph = (lcd_graph_t *) item;

  /* Check if area is clicked */
  if (graph->is_clicked == GUI_CLICKED)
  {
    /* Invert colors */
    color_get_inverse(&(graph->text_color), &text_color);
    color_get_inverse(&(graph->bg_legend_color), &bg_legend_color);
    color_get_inverse(&(graph->border_color), &border_color);
  }
  else
  {
    text_color = graph->text_color;
    bg_legend_color = graph->bg_legend_color;
    border_color = graph->border_color;
  }

  /* Check if area lies within lcd boundaries */
  if (graph->pos.x_pos + graph->width_legend + graph->width_graph > lcd->lcd_x_size)
  {
    return GUI_SIZE_ERROR;
  }
  if (graph->pos.y_pos + graph->height > lcd->lcd_y_size)
  {
    return GUI_SIZE_ERROR;
  }

  /* Draw legend rectangle */
  lcd_draw_rectangle(lcd,
                     graph->pos.x_pos, graph->width_legend,
                     graph->pos.y_pos, graph->height,
                     &bg_legend_color);

  /* Draw graph rectangle */
  lcd_draw_rectangle(lcd,
                     graph->pos.x_pos + graph->width_legend, graph->width_graph,
                     graph->pos.y_pos, graph->height,
                     &(graph->bg_graph_color));

  /* Draw border if present */
  if (graph->border == RECTANGLE)
  {
    x_pos = graph->pos.x_pos;
    y_pos = graph->pos.y_pos;
    x_pos_width = graph->pos.x_pos + graph->width_legend + graph->width_graph - 1;
    y_pos_height = graph->pos.y_pos + graph->height - 1;

    /* Top line */
    lcd_draw_line(lcd,
                  x_pos, y_pos,
                  x_pos_width, y_pos,
                  &border_color);
    /* Bottom line */
    lcd_draw_line(lcd,
                  x_pos, y_pos_height,
                  x_pos_width, y_pos_height,
                  &border_color);
    /* Left line */
    lcd_draw_line(lcd,
                  x_pos, y_pos,
                  x_pos, y_pos_height,
                  &border_color);
    /* Right line */
    lcd_draw_line(lcd,
                  x_pos_width, y_pos,
                  x_pos_width, y_pos_height,
                  &border_color);
  }

  return GUI_OK;
}

gui_status_t lcd_update_graph(lcd_t *lcd, void *item)
{
  uint32_t y_pos_second_to_last, y_pos_last;

  /* Cast pointer */
  lcd_graph_t *graph = (lcd_graph_t *) item;

  /* Calculate graph points */
  y_pos_second_to_last = (graph->second_to_last_value * (graph->height >> 1))
                         / graph->y_axis_full_scale;
  y_pos_last           = (graph->last_value * (graph->height >> 1))
                         / graph->y_axis_full_scale;

  if (graph->value_index < graph->width_graph - 1)
  {
    lcd_draw_line(lcd,
                  graph->pos.x_pos + graph->width_legend + graph->value_index,
                  graph->pos.y_pos + (graph->height >> 1) + y_pos_second_to_last,
                  graph->pos.x_pos + graph->width_legend + graph->value_index + 1,
                  graph->pos.y_pos + (graph->height >> 1) + y_pos_last,
                  &(graph->line_color));
    graph->value_index++;
  }
  else
  {
    graph->value_index = 0;
  }

  return GUI_OK;
}
/**
  * @brief  Sets the backlight brightness level.
  * @param  level: Backlight PWM duty cycle (0 - 255).
  * @retval None
*/
gui_status_t lcd_set_config(lcd_t *lcd, void *item)
{
  /*
   * Set backlight PWM level
   */
  uint16_t param_pwm_conf_tmp[7];

  /* Cast pointer */
  lcd_config_t *config = (lcd_config_t *) item;

  /* Read previous configuration */
  lcd_hal_read_reg(lcd, LCD_GET_PWM_CONF, param_pwm_conf_tmp, 7);

  /* Change PWM value */
  param_pwm_conf_tmp[1] = config->backlight_level;

  /* Send new values */
  lcd_hal_write_reg(lcd, LCD_SET_PWM_CONF, param_pwm_conf_tmp, 6);

  return GUI_OK;
}