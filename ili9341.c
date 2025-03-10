#include "ili9341.h"
#include "font.h"

static SPI_HandleTypeDef *hspi;
static GPIO_TypeDef* cs_port;
static uint16_t cs_pin;
static GPIO_TypeDef* dc_port;
static uint16_t dc_pin;
static GPIO_TypeDef* reset_port;
static uint16_t reset_pin;

// Write a command to the ILI9341
void ILI9341_WriteCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(dc_port, dc_pin, GPIO_PIN_RESET); // DC low for command
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET); // CS low to select
    HAL_SPI_Transmit(hspi, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET); // CS high to deselect
}

// Write data to the ILI9341
void ILI9341_WriteData(uint8_t data)
{
    HAL_GPIO_WritePin(dc_port, dc_pin, GPIO_PIN_SET); // DC high for data
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET); // CS low to select
    HAL_SPI_Transmit(hspi, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET); // CS high to deselect
}

// Set the address window for drawing
void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ILI9341_WriteCommand(ILI9341_CASET); // Column address set
    ILI9341_WriteData(x0 >> 8);
    ILI9341_WriteData(x0 & 0xFF);
    ILI9341_WriteData(x1 >> 8);
    ILI9341_WriteData(x1 & 0xFF);

    ILI9341_WriteCommand(ILI9341_PASET); // Page address set
    ILI9341_WriteData(y0 >> 8);
    ILI9341_WriteData(y0 & 0xFF);
    ILI9341_WriteData(y1 >> 8);
    ILI9341_WriteData(y1 & 0xFF);

    ILI9341_WriteCommand(ILI9341_RAMWR); // Memory write
}

// Fill the screen with a color
void ILI9341_FillScreen(uint16_t color)
{
    ILI9341_SetAddressWindow(0, 0, 239, 319);
    for (uint32_t i = 0; i < 240 * 320; i++)
    {
        ILI9341_WriteData(color >> 8);
        ILI9341_WriteData(color & 0xFF);
    }
}


/**
  * @brief  Draw a filled rectangle on the screen.
  * @param  x: X coordinate of the top-left corner.
  * @param  y: Y coordinate of the top-left corner.
  * @param  w: Width of the rectangle.
  * @param  h: Height of the rectangle.
  * @param  color: Color of the rectangle.
  * @retval None
  */
void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    // Check bounds
    if (x >= 240 || y >= 320) return;
    if ((x + w - 1) >= 240) w = 240 - x;
    if ((y + h - 1) >= 320) h = 320 - y;

    // Set the address window to the rectangle area
    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    // Fill the rectangle with the specified color
    for (uint32_t i = 0; i < w * h; i++)
    {
        ILI9341_WriteData(color >> 8);    // Send high byte
        ILI9341_WriteData(color & 0xFF); // Send low byte
    }
}


void ILI9341_DrawChar(uint16_t x, uint16_t y, char c, uint16_t fg_color, uint16_t bg_color, uint8_t size)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t line = font[(c - 32) * 5 + i]; // Get the font data for the character
        for (uint8_t j = 0; j < 8; j++)
        {
            if (line & 0x1) // Check if the pixel is set
            {
                if (size == 1)
                {
                    ILI9341_DrawPixel(x + i, y + j, fg_color);
                }
                else
                {
                    ILI9341_FillRectangle(x + (i * size), y + (j * size), size, size, fg_color);
                }
            }
            else if (bg_color != fg_color) // Draw background if needed
            {
                if (size == 1)
                {
                    ILI9341_DrawPixel(x + i, y + j, bg_color);
                }
                else
                {
                    ILI9341_FillRectangle(x + (i * size), y + (j * size), size, size, bg_color);
                }
            }
            line >>= 1; // Move to the next pixel in the column
        }
    }
}

// Draw a pixel at (x, y) with the specified color
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= 240 || y >= 320) return; // Check bounds
    ILI9341_SetAddressWindow(x, y, x + 1, y + 1);
    ILI9341_WriteData(color >> 8);
    ILI9341_WriteData(color & 0xFF);
}

// Draw a string on the screen
void ILI9341_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t fg_color, uint16_t bg_color, uint8_t size)
{
    while (*str)
    {
        ILI9341_DrawChar(x, y, *str, fg_color, bg_color, size);
        x += 6 * size;
        str++;
    }
}


// Initialize the ILI9341
void ILI9341_Init(SPI_HandleTypeDef *hspi_instance, GPIO_TypeDef* cs_port_instance, uint16_t cs_pin_instance, GPIO_TypeDef* dc_port_instance, uint16_t dc_pin_instance, GPIO_TypeDef* reset_port_instance, uint16_t reset_pin_instance)
{
    hspi = hspi_instance;
    cs_port = cs_port_instance;
    cs_pin = cs_pin_instance;
    dc_port = dc_port_instance;
    dc_pin = dc_pin_instance;
    reset_port = reset_port_instance;
    reset_pin = reset_pin_instance;

    // Reset the display
    HAL_GPIO_WritePin(reset_port, reset_pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(reset_port, reset_pin, GPIO_PIN_SET);
    HAL_Delay(120);

    // Initialization sequence
    ILI9341_WriteCommand(ILI9341_SWRESET); // Software reset
    HAL_Delay(150);

    ILI9341_WriteCommand(ILI9341_PWCTR1); // Power control 1
    ILI9341_WriteData(0x23);

    ILI9341_WriteCommand(ILI9341_PWCTR2); // Power control 2
    ILI9341_WriteData(0x10);

    ILI9341_WriteCommand(ILI9341_VMCTR1); // VCOM control 1
    ILI9341_WriteData(0x3E);
    ILI9341_WriteData(0x28);

    ILI9341_WriteCommand(ILI9341_VMCTR2); // VCOM control 2
    ILI9341_WriteData(0x86);

    ILI9341_WriteCommand(ILI9341_MADCTL); // Memory access control
    ILI9341_WriteData(0x48);

    ILI9341_WriteCommand(ILI9341_PIXFMT); // Pixel format
    ILI9341_WriteData(0x55);

    ILI9341_WriteCommand(ILI9341_SLPOUT); // Sleep out
    HAL_Delay(120);

    ILI9341_WriteCommand(ILI9341_DISPON); // Display on
}