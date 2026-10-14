#include "main.h"
#include "definitions.h"
#include <stdbool.h>

void oled_init(I2C_HandleTypeDef *hi2c){

     uint8_t cmds[] = {
        SSD1306_SET_DISP,               // set display off
        /* memory mapping */
        SSD1306_SET_MEM_MODE,           // set memory address mode 0 = horizontal, 1 = vertical, 2 = page
        0x00,                           // horizontal addressing mode
        /* resolution and layout */
        SSD1306_SET_DISP_START_LINE,    // set display start line to 0
        SSD1306_SET_SEG_REMAP | 0x01,   // set segment re-map, column address 127 is mapped to SEG0
        SSD1306_SET_MUX_RATIO,          // set multiplex ratio
        SSD1306_HEIGHT - 1,             // Display height - 1
        SSD1306_SET_COM_OUT_DIR | 0x08, // set COM (common) output scan direction. Scan from bottom up, COM[N-1] to COM0
        SSD1306_SET_DISP_OFFSET,        // set display offset
        0x00,                           // no offset
        SSD1306_SET_COM_PIN_CFG,        // set COM (common) pins hardware configuration. Board specific magic number.
                                        // 0x02 Works for 128x32, 0x12 Possibly works for 128x64. Other options 0x22, 0x32
#if ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 32))
        0x02,
#elif ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 64))
        0x12,
#else
        0x02,
#endif
        /* timing and driving scheme */
        SSD1306_SET_DISP_CLK_DIV,       // set display clock divide ratio
        0x80,                           // div ratio of 1, standard freq
        SSD1306_SET_PRECHARGE,          // set pre-charge period
        0xF1,                           // Vcc internally generated on our board
        SSD1306_SET_VCOM_DESEL,         // set VCOMH deselect level
        0x30,                           // 0.83xVcc
        /* display */
        SSD1306_SET_CONTRAST,           // set contrast control
        0xFF,
        SSD1306_SET_ENTIRE_ON,          // set entire display on to follow RAM content
        SSD1306_SET_NORM_DISP,           // set normal (not inverted) display
        SSD1306_SET_CHARGE_PUMP,        // set charge pump
        0x14,                           // Vcc internally generated on our board
        SSD1306_SET_SCROLL | 0x00,      // deactivate horizontal scrolling if set. This is necessary as memory writes will corrupt if scrolling was enabled
        SSD1306_SET_DISP | 0x01, // turn display on
    };
 
    int cmd_num = sizeof(cmds)/ sizeof(cmds[0]);
    for (int i = 0; i < cmd_num; i++){

        uint8_t buf[2] = {0x00, cmds[i]};
        HAL_I2C_Master_Transmit(hi2c,SSD1306_I2C_ADDR, buf, 2, HAL_MAX_DELAY);

    }
        //uint8_t test[2] = {0x00, 0xA5}; 
        //HAL_I2C_Master_Transmit(&hi2c1,SSD1306_I2C_ADDR, test, 2, HAL_MAX_DELAY); } /* USER CODE END 0 */
}
void sendCommand(uint8_t comm, I2C_HandleTypeDef *hi2c){


        uint8_t buf[2] = {0x00, comm}; 
        HAL_I2C_Master_Transmit(hi2c,SSD1306_I2C_ADDR, buf, 2, HAL_MAX_DELAY);
}
void setPageAndColumnAddress(uint8_t pStart,uint8_t pEnd,uint8_t cStart,int8_t cEnd, I2C_HandleTypeDef *hi2c){
    sendCommand(SSD1306_SET_COL_ADDR, hi2c);
    sendCommand(cStart, hi2c);
    sendCommand(cEnd, hi2c);
    sendCommand(SSD1306_SET_PAGE_ADDR, hi2c);
    sendCommand(pStart & 0x07, hi2c);
    sendCommand(pEnd & 0x07, hi2c);
}
void setHorizontalMode(I2C_HandleTypeDef *hi2c){
    sendCommand(0x20, hi2c);
    sendCommand(0x00, hi2c);
}

void clearScreen(I2C_HandleTypeDef *hi2c){
        uint8_t rset[2] = {0x40, 0x00};
        for (int i = 0u; i < SSD1306_BUF_LEN; i++){
            HAL_I2C_Master_Transmit(hi2c,SSD1306_I2C_ADDR, rset, 2, HAL_MAX_DELAY);
        }
}


void fillScreen(I2C_HandleTypeDef *hi2c){
        uint8_t rset[2] = {0x40, 0xFF};
        for (int i = 0u; i < SSD1306_BUF_LEN; i++){
            HAL_I2C_Master_Transmit(hi2c,SSD1306_I2C_ADDR, rset, 2, HAL_MAX_DELAY);
        }
}


void printLines(uint8_t flipByte,I2C_HandleTypeDef *hi2c){
        uint8_t rset[2] = {0x40, flipByte};
        for (int i = 0u; i < SSD1306_BUF_LEN; i++){
            HAL_I2C_Master_Transmit(hi2c,SSD1306_I2C_ADDR, rset, 2, HAL_MAX_DELAY);
        }
}

void set_pixel(int x, int y, bool state, uint8_t* buf){
       int pos = ((y >> 3) * SSD1306_WIDTH) + x; 
    if (state){
        buf[pos] |= (0x01 << (y & 0x7));
    } else {
        buf[pos] &= ~(0x01 << (y & 0x7));
    }
}

void update(uint8_t* buf, int len, I2C_HandleTypeDef *hi2c){
        uint8_t rset[2] = {0x40, 0x00};
        for (int i = 0u; i < len; i++){
            rset[1] = buf[i];
            HAL_I2C_Master_Transmit(hi2c,SSD1306_I2C_ADDR, rset, 2, HAL_MAX_DELAY);
        }

}
void printBitmap(int x, int y, int width, int height, int size, const uint8_t *bitmap, uint8_t* buf){
    uint8_t chkMsk = 0;

    int originalX = x;
    int originalY = y;

    for (int i = 0; i < size; i++){
        for (int j = 0; j < 8; j++){
        chkMsk = 0x80 >>j;

        set_pixel(x, y, bitmap[i] & chkMsk, buf);
        x++;

        if ((x - originalX) == width){
         y++;
         x = originalX;
        }

        if (( y - originalY) == height){
            return;
         }
         }
    }
}

void printText(int x, int y, int size, char* text,const uint8_t* font, uint8_t* buf){
    const uint8_t *drawChar;

    int runningX = x;
    int runningY = y;

    int target;
   
    int len = 0;
    while (text[len] != '\0')
        len++;
    for (int i = 0; i < len; i++){
        runningY += size*(runningX >> 7);
        runningX &= SSD1306_WIDTH-1;
        target = (size*size >> 3) * (text[i] - ' ');
        drawChar = font + target;

        printBitmap(runningX, runningY, size, size, size, drawChar, buf);
        runningX += size;

        if (runningY == SSD1306_HEIGHT)
            return;
    }
}
