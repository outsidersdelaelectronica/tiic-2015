/*
 * lcd.c
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "lcd.h"

extern SRAM_HandleTypeDef hsram1;
color_t background_color;

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
void lcd_init()
{
  uint16_t lcd_device_descriptor[5];
  uint8_t device_descriptor_ok;
  
  /* Check device descriptor */
  lcd_read_reg(LCD_READ_DDB, lcd_device_descriptor, 5);
  device_descriptor_ok = (lcd_device_descriptor[0] == 0x0001) &&        //Supplier ID (high byte)
                         (lcd_device_descriptor[1] == 0x0057) &&        //Supplier ID (low byte)
                         (lcd_device_descriptor[2] == 0x0061) &&        //Product ID
                         (lcd_device_descriptor[3] == 0x0001) &&        //Revision code
                         (lcd_device_descriptor[4] == 0x00FF);
  
  if (device_descriptor_ok)
  {
    /* Set PLL */
    lcd_write_reg(LCD_SET_PLL_MN, lcd_param_pll_mn, 3);
    /* Enable PLL */
    lcd_write_reg(LCD_SET_PLL, lcd_param_pll, 1);
    /* Wait for PLL to start up */
    HAL_Delay(1);
    /* Lock PLL */
    lcd_write_reg(LCD_SET_PLL, lcd_param_pll_lock, 1);

    /* Soft-reset LCD controller */
    lcd_write_command(LCD_SOFT_RESET);
    HAL_Delay(10);
    
    /* Set the pixel clock frequency */
    lcd_write_reg(LCD_SET_LSHIFT_FREQ, lcd_param_lshift_freq, 3);

    /* Set LCD modes, horizontal and vertical sync, and pixel data interface */
    lcd_write_reg(LCD_SET_LCD_MODE, lcd_param_lcd_mode, 7);
    lcd_write_reg(LCD_SET_HORI_PERIOD, lcd_param_hori_period, 8);
    lcd_write_reg(LCD_SET_VERT_PERIOD, lcd_param_vert_period, 7);
    
    /* Set GPIO configuration */
    lcd_write_reg(LCD_SET_GPIO_CONF, lcd_param_gpio_conf, 2);
    lcd_write_reg(LCD_SET_GPIO_VALUE, lcd_param_gpio_value, 1);

    /* Set pixel data format */
    lcd_write_reg(LCD_SET_PIXEL_DATA_INTERFACE, lcd_param_pixel_data_interface, 1);
    
    /* Set address scan mode and column and row starting pixel */
    lcd_write_reg(LCD_SET_ADDRESS_MODE, lcd_param_address_mode, 1);
    lcd_write_reg(LCD_SET_COLUMN_ADDRESS, lcd_param_column_address, 4);
    lcd_write_reg(LCD_SET_PAGE_ADDRESS, lcd_param_page_address, 4);

    /* Set gamma curve */
    lcd_write_reg(LCD_SET_GAMMA_CURVE, lcd_param_gamma_curve, 1);
    
    /* Set partial area and scrolling configuration */
    lcd_write_reg(LCD_SET_PARTIAL_AREA, lcd_param_partial_area, 4);
    lcd_write_reg(LCD_SET_SCROLL_AREA, lcd_param_scroll_area, 6);
    lcd_write_reg(LCD_SET_SCROLL_START, lcd_param_scroll_start, 2);
    
    /* Set backlight brightness and image post processing defaults */
    lcd_write_reg(LCD_SET_PWM_CONF, lcd_param_pwm_conf, 6);
    lcd_write_reg(LCD_SET_DBC_CONF, lcd_param_dbc_conf, 1);
    lcd_write_reg(LCD_SET_POST_PROC, lcd_param_post_proc, 4);
    
    /* Turn display off and set TE signal off */
    lcd_write_command(LCD_SET_TEAR_OFF);
    
    /* Clean frame buffer */
    lcd_clean_screen(&background_color);
    
    /* Exit sleep mode and turn display on */
    lcd_write_command(LCD_EXIT_SLEEP_MODE);
    HAL_Delay(10);
    //lcd_write_command(LCD_SET_DISPLAY_ON);    //LCD_EXIT_SLEEP_MODE triggers LCD_SET_DISPLAY_ON
  }
}

/**
  * @brief  Writes one command.
  * @param  reg: Command to send.
  * @retval None
*/
void lcd_write_command(uint16_t command)
{
  /* Send command */
  HAL_SRAM_Write_16b(&(hsram1), LCD_REG, &command, 1);
}

/**
  * @brief  Writes one word of data.
  * @param  reg: Data to send.
  * @retval None
*/
void lcd_write_data(uint16_t data)
{
  /* Send data */
  HAL_SRAM_Write_16b(&(hsram1), LCD_DATA, &data, 1);
}

/**
  * @brief  Reads n params of a register.
  * @param  reg: Address of the lcd register to read.
  * @param  param_buffer: Array to store all parameters.
  * @param  param_buffer_size: Number of parameters to read.
  * @retval None
*/
void lcd_read_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size)
{
  /* Send register */
  HAL_SRAM_Write_16b(&(hsram1), LCD_REG, &reg, 1);
  
  /* Send parameters */
  HAL_SRAM_Read_16b(&(hsram1), LCD_DATA, param_buffer, param_buffer_size);
}

/**
  * @brief  Writes n params of a register.
  * @param  reg: Address of the lcd register to read.
  * @param  param_buffer: Array to store all parameters.
  * @param  param_buffer_size: Number of parameters to write.
  * @retval None
*/
void lcd_write_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size)
{
  /* Send register */
  HAL_SRAM_Write_16b(&(hsram1), LCD_REG, &reg, 1);
  
  /* Send parameters */
  HAL_SRAM_Write_16b(&(hsram1), LCD_DATA, param_buffer, param_buffer_size);
}

/**
  * @brief  Sets the portion of the frame buffer to be accessed sequentially.
  * @param  start_col: Start column (x axis).
  * @param  end_col: End column (x axis).
  * @param  start_row: Start row (y axis).
  * @param  end_row: End row (y axis).
  * @retval None
*/
void lcd_set_drawing_address(uint16_t start_col, uint16_t end_col,
                             uint16_t start_row, uint16_t end_row)
{
  uint16_t pixel_column_address[4], pixel_page_address[4];

  /* Prepare x_pos to be sent as SET_COLUMN_ADDRESS parameters */
  pixel_column_address[0] = (start_col >> 8) & 0x00FF;
  pixel_column_address[1] =  start_col       & 0x00FF;
  pixel_column_address[2] = (end_col >> 8)   & 0x00FF;
  pixel_column_address[3] =  end_col         & 0x00FF;

  /* Prepare y_pos to be sent as SET_PAGE_ADDRESS parameters */
  pixel_page_address[0]   = (start_row >> 8) & 0x00FF;
  pixel_page_address[1]   =  start_row       & 0x00FF;
  pixel_page_address[2]   = (end_row >> 8)   & 0x00FF;
  pixel_page_address[3]   =  end_row         & 0x00FF;

  /* Send boundary addresses */
  lcd_write_reg(LCD_SET_COLUMN_ADDRESS, pixel_column_address, 4);
  lcd_write_reg(LCD_SET_PAGE_ADDRESS, pixel_page_address, 4);
}

/**
  * @brief  Draws a pixel on a specific location.
  * @param  x_pos: Pixel horizontal position.
  * @param  y_pos: Pixel vertical position.
  * @param  param_buffer_size: Number of parameters to write.
  * @retval None
*/
void lcd_draw_pixel(uint16_t x_pos, uint16_t y_pos, color_t *color)
{
  lcd_set_drawing_address(x_pos, x_pos, y_pos, y_pos);
  
  /* Send pixel color */
  lcd_write_command(LCD_WRITE_MEMORY_START);
  lcd_write_data(color->color565);
}

/**
  * @brief  Draws a char on a specific location.
  * @param  character: Character to display.
  * @param  current_font: Font.
  * @param  char_color: Character color.
  * @param  x_pos: Col position.
  * @param  y_pos: Row position.
  * @retval None
*/
void lcd_draw_char(char character,
                   const uint8_t *current_font, 
                   color_t *char_color,
                   uint16_t *x_pos, uint16_t *y_pos)
{
  // Hardcoded -> TO-DO: build font structure
  
  uint32_t line = 0x00000000;
  uint32_t mask = 0x00000001;  
  
  uint8_t font_starting_char;           // Font array starting character (character number == 0)
  uint8_t character_number;             // Character number in font array
  
  uint8_t character_properties;         // Character properties position in font array
  uint8_t character_height;             // Character height in pixels
  uint8_t character_width;              // Character width in pixels
  uint32_t character_position;          // Character drawing position in font array
  
  int8_t width_calc;                    // Variables used to calculate number of bytes per horizontal line
  uint8_t bytes_per_line = 0;           //
  
  /* Search character in font array */
  font_starting_char = current_font[2];
  character_number = character - font_starting_char;          

  /* Get character properties */
  character_properties = 8 + 4*character_number;
  character_height = current_font[6];
  character_width = current_font[character_properties];
  character_position = ((current_font[character_properties + 3]) << 16) |
                       ((current_font[character_properties + 2]) << 8)  |
                        (current_font[character_properties + 1]);
  
  /* Set character drawing zone */
  lcd_set_drawing_address(*x_pos,
                          *x_pos + character_width - 1,
                          *y_pos,
                          *y_pos + character_height - 1);
  
  /* Send character pixels */
  lcd_write_command(LCD_WRITE_MEMORY_START);
  
  /* Iterate: for each line... */
  for (int i = 0; i < character_height; i++)
  {
    /* Retrieve char line bits */
    line = 0x00000000;    // Reset line
    bytes_per_line = 0;
    for (width_calc = character_width; width_calc > 0; width_calc -= 8)
    {
      line = (line) | (current_font[character_position++] << (8*bytes_per_line));
      bytes_per_line++;
    }
    
    /* Read font array and send pixels of the line*/
    mask = 0x00000001;
    for (int j = 0; j < character_width; j++)
    {
      if(line & mask)
      {
        lcd_write_data(char_color->color565);
      }
      else
      {
        lcd_write_data(background_color.color565);   
      }
      mask = mask << 1;
    }
  }
  
  /* Update x position */
  *x_pos += character_width;  
}

/**
  * @brief  Paints the whole screen with a specific color.
  * @param  color: Screen color.
  * @retval None
*/
void lcd_clean_screen(color_t *color)
{
  /* Send screen pixel limits */
  lcd_set_drawing_address(0,
                          LCD_X_SIZE - 1,
                          0,
                          LCD_Y_SIZE - 1);

  /* Write pixels */
  lcd_write_command(LCD_WRITE_MEMORY_START);
  for (int i = 0; i < LCD_Y_SIZE; i++)
  {
    for (int j = 0; j < LCD_X_SIZE; j++)
    {
      HAL_SRAM_Write_16b(&(hsram1), LCD_DATA, &(color->color565), 1);
    }
  }
}

/**
  * @brief  Sets the backlight brightness level.
  * @param  level: Backlight PWM duty cycle (0 - 255).
  * @retval None
*/
void lcd_set_brightness(uint8_t level)
{
  uint16_t param_pwm_conf_tmp[7];
  
  /* Read previous configuration */
  lcd_read_reg(LCD_GET_PWM_CONF, param_pwm_conf_tmp, 7);

  /* Change PWM value */
  param_pwm_conf_tmp[1] = level;
  
  /* Send new values */
  lcd_write_reg(LCD_SET_PWM_CONF, param_pwm_conf_tmp, 6);
}