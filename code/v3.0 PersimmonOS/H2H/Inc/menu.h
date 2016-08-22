/*
 * menu.h
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#ifndef MENU_H_
#define MENU_H_

#include "item.h"
#include "fonts.h"

#include "lcd.h"
#include "click.h"

#define MENU_MAX_ITEMS 20

typedef struct menu_t
{
  uint32_t item_num;
  item_action_t items[MENU_MAX_ITEMS];
}
menu_t;

int32_t menu_search_click(menu_t *menu, click_t *click, item_t *item);

/* Menus */
extern menu_t menu_welcome;
extern menu_t menu_top_bar;

extern menu_t menu_main;
extern menu_t menu_ecg;

#endif /* MENU_H_ */