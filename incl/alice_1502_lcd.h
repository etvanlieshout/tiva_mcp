/* alice_1502_lcd.h
 *
 * header file for alice_1502_lcd.c
 */

#ifndef _ALICE_LCD_H
#define _ALICE_LCD_H

#include <mcp.h>
#include <gpio.h>
#include <spi.h>

#define	LCD_CMD   0
#define	LCD_DATA  1

void lcd_procmon_start(tiva_spi *spi);
void lcd_procmon_update(tiva_spi *spi, uint8_t pcount);
void lcd1502_init(tiva_spi *spi);
void lcd1502_spi_send(tiva_spi *spi, char* data, int dtype);
static void hct595_outen();
static void hct595_outdis();
static void lcd_delay();

#endif
