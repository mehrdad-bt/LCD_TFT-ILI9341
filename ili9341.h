#ifndef ILI9341_H
#define ILI9341_H

#include "stm32f1xx_hal.h"

// ILI9341 Command Definitions (from the provided table)
#define ILI9341_NOP             0x00 // No Operation
#define ILI9341_SWRESET         0x01 // Software Reset
#define ILI9341_RDDID           0x04 // Read Display Identification Information
#define ILI9341_RDDST           0x09 // Read Display Status
#define ILI9341_RDDPM           0x0A // Read Display Power Mode
#define ILI9341_RDDMADCTL       0x0B // Read Display MADCTL
#define ILI9341_RDDCOLMOD       0x0C // Read Display Pixel Format
#define ILI9341_RDDIM           0x0D // Read Display Image Format
#define ILI9341_RDDSM           0x0E // Read Display Signal Mode
#define ILI9341_RDDSDR          0x0F // Read Display Self-Diagnostic Result
#define ILI9341_SLPIN           0x10 // Enter Sleep Mode
#define ILI9341_SLPOUT          0x11 // Sleep Out
#define ILI9341_PTLON           0x12 // Partial Mode ON
#define ILI9341_NORON           0x13 // Normal Display Mode ON
#define ILI9341_INVOFF          0x20 // Display Inversion OFF
#define ILI9341_INVON           0x21 // Display Inversion ON
#define ILI9341_GAMMASET        0x26 // Gamma Set
#define ILI9341_DISPOFF         0x28 // Display OFF
#define ILI9341_DISPON          0x29 // Display ON
#define ILI9341_CASET           0x2A // Column Address Set
#define ILI9341_PASET           0x2B // Page Address Set
#define ILI9341_RAMWR           0x2C // Memory Write
#define ILI9341_RAMRD           0x2E // Memory Read
#define ILI9341_PLTAR           0x30 // Partial Area
#define ILI9341_VSCRDEF         0x33 // Vertical Scrolling Definition
#define ILI9341_TEOFF           0x34 // Tearing Effect Line OFF
#define ILI9341_TEON            0x35 // Tearing Effect Line ON
#define ILI9341_MADCTL          0x36 // Memory Access Control
#define ILI9341_VSCRSADD        0x37 // Vertical Scrolling Start Address
#define ILI9341_IDMOFF          0x38 // Idle Mode OFF
#define ILI9341_IDMON           0x39 // Idle Mode ON
#define ILI9341_PIXFMT          0x3A // Pixel Format Set
#define ILI9341_WRMEMC          0x3C // Write Memory Continue
#define ILI9341_RDMEMC          0x3E // Read Memory Continue
#define ILI9341_STE             0x44 // Set Tear Scanline
#define ILI9341_GSCAN           0x45 // Get Scanline
#define ILI9341_WRDISBV         0x51 // Write Display Brightness
#define ILI9341_RDDISBV         0x52 // Read Display Brightness
#define ILI9341_WRCTRLD         0x53 // Write CTRL Display
#define ILI9341_RDCTRLD         0x54 // Read CTRL Display
#define ILI9341_WRCABC          0x55 // Write Content Adaptive Brightness Control
#define ILI9341_RDCABC          0x56 // Read Content Adaptive Brightness Control
#define ILI9341_WRCABCMB        0x5E // Write CABC Minimum Brightness
#define ILI9341_RDCABCMB        0x5F // Read CABC Minimum Brightness
#define ILI9341_RDID1           0xDA // Read ID1
#define ILI9341_RDID2           0xDB // Read ID2
#define ILI9341_RDID3           0xDC // Read ID3
#define ILI9341_RDID4           0xD3 // Read ID4
#define ILI9341_PWCTR1          0xC0 // Power Control 1
#define ILI9341_PWCTR2          0xC1 // Power Control 2
#define ILI9341_VMCTR1          0xC5 // VCOM Control 1
#define ILI9341_VMCTR2          0xC7 // VCOM Control 2
#define ILI9341_PGAMMA          0xE0 // Positive Gamma Correction
#define ILI9341_NGAMMA          0xE1 // Negative Gamma Correction
#define ILI9341_DGAMCTRL1       0xE2 // Digital Gamma Control 1
#define ILI9341_DGAMCTRL2       0xE3 // Digital Gamma Control 2
#define ILI9341_IFCTL           0xF6 // Interface Control

// Color Definitions
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F

// Function Prototypes
void ILI9341_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef* cs_port, uint16_t cs_pin, GPIO_TypeDef* dc_port, uint16_t dc_pin, GPIO_TypeDef* reset_port, uint16_t reset_pin);
void ILI9341_WriteCommand(uint8_t cmd);
void ILI9341_WriteData(uint8_t data);
void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9341_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t fg_color, uint16_t bg_color, uint8_t size);
void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ILI9341_DrawChar(uint16_t x, uint16_t y, char c, uint16_t fg_color, uint16_t bg_color, uint8_t size);

#endif