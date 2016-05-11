/*
 * lcd.c
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "lcd.h"

extern SRAM_HandleTypeDef hsram1;

#define LCD_REG        ((uint32_t *)(FSMC_BASE))
#define LCD_DATA       ((uint32_t *)(FSMC_BASE + 0x00020000U))  //See p.620 of STM32L162VD ref. manual

#define LCD_X_SIZE     (uint16_t) 800
#define LCD_Y_SIZE     (uint16_t) 480

/* LCD init parameters */
uint16_t lcd_param_lcd_mode[]                   = {0x0000, 0x0020, 0x0003, 0x001F, 0x0001, 0x00DF, 0x0000};
uint16_t lcd_param_pixel_data_interface[]       = {0x0003};

uint16_t lcd_param_address_mode[]               = {0x0000};
uint16_t lcd_param_column_address[]             = {0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_page_address[]               = {0x0000, 0x0000, 0x0000, 0x0000};

uint16_t lcd_param_gamma_curve[]                = {0x0001};

uint16_t lcd_param_partial_area[]               = {0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_scroll_area[]                = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
uint16_t lcd_param_scroll_start[]               = {0x0000, 0x0000};

uint16_t lcd_param_pwm_conf[]                   = {0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0001};
uint16_t lcd_param_dbc_conf[]                   = {0x0042};
uint16_t lcd_param_post_proc[]                  = {0x0040, 0x0080, 0x0040, 0x0000};

/**
  * @brief  Initializes LCD module
  * @retval None
*/
void lcd_init()
{
  uint16_t lcd_device_descriptor[5];
  uint8_t device_descriptor_ok;
  
  /* Reset LCD controller */
  lcd_write_command(LCD_SOFT_RESET);
  HAL_Delay(10);

  /* Check device descriptor */
  lcd_read_reg(LCD_READ_DDB, lcd_device_descriptor, 5);
  device_descriptor_ok = (lcd_device_descriptor[0] == 0x0001) &&        //Supplier ID (high byte)
                         (lcd_device_descriptor[1] == 0x0057) &&        //Supplier ID (low byte)
                         (lcd_device_descriptor[2] == 0x0061) &&        //Product ID
                         (lcd_device_descriptor[3] == 0x0001) &&        //Revision code
                         (lcd_device_descriptor[4] == 0x00FF);
  
  if (device_descriptor_ok)
  {
    /* Turn display off and set TE signal off */
    lcd_write_command(LCD_SET_DISPLAY_OFF);
    lcd_write_command(LCD_SET_TEAR_OFF);

    /* Set LCD modes and pixel data interface */
    lcd_write_reg(LCD_SET_LCD_MODE, lcd_param_lcd_mode, 7);
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

  /* Send command */
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
  * @brief  Draws a pixel on a specific location.
  * @param  x_pos: Pixel horizontal position.
  * @param  y_pos: Pixel vertical position.
  * @param  param_buffer_size: Number of parameters to write.
  * @retval None
*/
void lcd_draw_pixel(uint16_t x_pos, uint16_t y_pos, color_t *color)
{
  uint16_t pixel_column_address[4], pixel_page_address[4];

  /* Prepare x_pos to be sent as SET_COLUMN_ADDRESS parameters */
  pixel_column_address[0] = pixel_column_address[2] = x_pos >> 8 & 0x00FF;
  pixel_column_address[1] = pixel_column_address[3] = x_pos      & 0x00FF;

  /* Prepare y_pos to be sent as SET_PAGE_ADDRESS parameters */
  pixel_page_address[0]   = pixel_page_address[2]   = y_pos >> 8 & 0x00FF;
  pixel_page_address[1]   = pixel_page_address[3]   = y_pos      & 0x00FF;

  /* Send pixel location */
  lcd_write_reg(LCD_SET_COLUMN_ADDRESS, pixel_column_address, 4);
  lcd_write_reg(LCD_SET_PAGE_ADDRESS, pixel_page_address, 4);

  /* Send pixel color */
  lcd_write_command(LCD_WRITE_MEMORY_START);
  lcd_write_data(color->color565);
}


void lcd_clean_screen(color_t *color)
{
  uint16_t pixel_column_address[4], pixel_page_address[4];

  /* Prepare x_pos to be sent as SET_COLUMN_ADDRESS parameters */
  pixel_column_address[0] = 0;
  pixel_column_address[1] = 0;
  pixel_column_address[2] = (LCD_X_SIZE >> 8) & 0x00FF;
  pixel_column_address[3] = LCD_X_SIZE        & 0x00FF;

  /* Prepare y_pos to be sent as SET_PAGE_ADDRESS parameters */
  pixel_page_address[0] = 0;
  pixel_page_address[1] = 0;
  pixel_page_address[2] = (LCD_Y_SIZE >> 8) & 0x00FF;
  pixel_page_address[3] = LCD_Y_SIZE        & 0x00FF;

  /* Send screen pixel limits */
  lcd_write_reg(LCD_SET_COLUMN_ADDRESS, pixel_column_address, 4);
  lcd_write_reg(LCD_SET_PAGE_ADDRESS, pixel_page_address, 4);

  /* Write pixels */
  lcd_write_command(LCD_WRITE_MEMORY_START);
  for (int i = 0; i < LCD_X_SIZE*LCD_Y_SIZE; ++i)
  {
    lcd_write_data(color->color565);
  }
}