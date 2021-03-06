/*
 *   Copyright (C) 2016 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "OLED.h"

static unsigned char logo_glcd_bmp[] =
  { 0b00101011, 0b11010100,
    0b01010111, 0b11101010,
    0b01010111, 0b11101010,
    0b00101011, 0b11010100,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000001, 0b10000000,
    0b00000000, 0b00000000 };

//DMR 48x16 px
static unsigned char logo_dmr_bmp[] =
  { 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
    0b10111111, 0b11111000, 0b01111000, 0b00011110, 0b01111111, 0b11100001,
    0b10111111, 0b11111110, 0b01111100, 0b00111110, 0b01100000, 0b00011001,
    0b10110000, 0b00001110, 0b01100110, 0b01100110, 0b01100000, 0b00011001,
    0b10110000, 0b00000110, 0b01100011, 0b11000110, 0b01100000, 0b00011001,
    0b10110000, 0b00000110, 0b01100001, 0b10000110, 0b01100000, 0b00011001,
    0b10110000, 0b00000110, 0b01100000, 0b00000110, 0b01111111, 0b11111001,
    0b10110000, 0b00000110, 0b01100000, 0b00000110, 0b01111000, 0b00000001,
    0b10110000, 0b00000110, 0b01100000, 0b00000110, 0b01101100, 0b00000001,
    0b10110000, 0b00000110, 0b01100000, 0b00000110, 0b01100110, 0b00000001,
    0b10110000, 0b00001110, 0b01100000, 0b00000110, 0b01100011, 0b00000001,
    0b10111111, 0b11111110, 0b01100000, 0b00000110, 0b01100001, 0b10000001,
    0b10011111, 0b11111000, 0b01100000, 0b00000110, 0b01100000, 0b11000001,
    0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111
  };

//D-Star 64x16 px
static unsigned char logo_dstar_bmp[] =
  { 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
    0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
    0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
    0b10000001, 0b11111100, 0b00000000, 0b00111100, 0b00000000, 0b00000000, 0b00000000, 0b01000001,
    0b10000001, 0b00000010, 0b00000000, 0b01000010, 0b01000000, 0b00000000, 0b00000000, 0b01000001,
    0b10000001, 0b00000010, 0b00000000, 0b01000000, 0b01000000, 0b00000000, 0b00000000, 0b01000001,
    0b10000001, 0b00000010, 0b00000000, 0b01000000, 0b01000000, 0b00000000, 0b00000000, 0b01000001,
    0b10000001, 0b00000010, 0b01111111, 0b00111100, 0b01111000, 0b00111100, 0b00111100, 0b01000001,
    0b10000001, 0b00000010, 0b00000000, 0b00000010, 0b01000000, 0b00000010, 0b01000010, 0b01000001,
    0b10000001, 0b00000010, 0b00000000, 0b00000010, 0b01000000, 0b00111110, 0b01000000, 0b01000001,
    0b10000001, 0b00000010, 0b00000000, 0b00000010, 0b01000000, 0b01000010, 0b01000000, 0b00000001,
    0b10000001, 0b00000010, 0b00000000, 0b01000010, 0b01000010, 0b01000010, 0b01000000, 0b01000001,
    0b10000001, 0b11111100, 0b00000000, 0b00111100, 0b00111100, 0b00111100, 0b01000000, 0b01000001,
    0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
    0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
    0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111
  };

COLED::COLED(unsigned char displayType, unsigned char displayBrightness, bool displayInvert) :
m_displayType(displayType),
m_displayBrightness(displayBrightness),
m_displayInvert(displayInvert)
{
}

COLED::~COLED()
{
}

bool COLED::open()
{
    // SPI
    if (display.oled_is_spi_proto(m_displayType))
    {
        // SPI change parameters to fit to your LCD
        if ( !display.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, m_displayType) )
            return false;
    }
    else
    {
        // I2C change parameters to fit to your LCD
        if ( !display.init(OLED_I2C_RESET, m_displayType) )
            return false; 
    }

    display.begin();

    display.invertDisplay(m_displayInvert ? 1 : 0);
    if (m_displayBrightness > 0U)
        display.setBrightness(m_displayBrightness);

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();        // display it (clear display)

    OLED_statusbar();
    display.setCursor(0,OLED_LINE1);
    display.print("Startup");
    display.display();

	return true;
}

void COLED::setIdleInt()
{
    m_mode = MODE_IDLE; 

    display.clearDisplay();
    OLED_statusbar();
    display.setCursor(0,display.height()/2);
    display.setTextSize(2);
    display.print("Idle");
    display.setTextSize(1);
    display.display();
    display.startscrollright(0x02,0x0f);
}

void COLED::setErrorInt(const char* text)
{
    m_mode = MODE_ERROR;

    display.clearDisplay();
    OLED_statusbar();
    display.setCursor(0,OLED_LINE1);
    display.printf("%s\n", text);
    display.display();
}

void COLED::setLockoutInt()
{
    m_mode = MODE_LOCKOUT;

    display.clearDisplay();
    OLED_statusbar();
    display.setCursor(0,OLED_LINE1);
    display.print("Lockout");
    display.display();
}

void COLED::writeDStarInt(const char* my1, const char* my2, const char* your, const char* type, const char* reflector)
{
    m_mode = MODE_DSTAR;
    display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE1);
    display.printf("%s %.8s/%4.4s", type, my1, my2);
    display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE2);
    display.printf("via %.8s", reflector);
    display.fillRect(0, OLED_LINE3, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE3);
    display.printf("%.8s <- %-8s", your, reflector);
    OLED_statusbar();
    display.display();
}

void COLED::clearDStarInt()
{
    display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE1);
    display.print("Listening");
    display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
    display.fillRect(0, OLED_LINE3, display.width(), 10, BLACK);
    OLED_statusbar();
    display.display();
}

void COLED::writeDMRInt(unsigned int slotNo, const std::string& src, bool group, const std::string& dst, const char* type)
{

    if (m_mode != MODE_DMR) {
        display.clearDisplay(); 
        m_mode = MODE_DMR;
        if (slotNo == 1U)
        {
          display.fillRect(0, OLED_LINE3, display.width(), 10, BLACK);
          display.setCursor(0,OLED_LINE3);
          display.print("2 Listening");
          display.fillRect(0, OLED_LINE4, display.width(), 10, BLACK);
        }
        else
        {
          display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
          display.setCursor(0,OLED_LINE1);
          display.print("1 Listening");
          display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
        }
    }

    if (slotNo == 1U) {
        display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
        display.setCursor(0,OLED_LINE1);
        display.printf("%i %s %s", slotNo, type, src.c_str());
        display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
        display.setCursor(0,OLED_LINE2);
        display.printf("%s%s", group ? "TG" : "", dst.c_str());
    } else {
        display.fillRect(0, OLED_LINE3, display.width(), 10, BLACK);
        display.setCursor(0,OLED_LINE3);
        display.printf("%i %s %s", slotNo, type, src.c_str());
        display.fillRect(0, OLED_LINE4, display.width(), 10, BLACK);
        display.setCursor(0,OLED_LINE4);
        display.printf("%s%s", group ? "TG" : "", dst.c_str());
    }
    OLED_statusbar();
    display.display();
}

void COLED::clearDMRInt(unsigned int slotNo)
{
    OLED_statusbar();
    if (slotNo == 1U)
    {
      display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
      display.setCursor(0,OLED_LINE1);
      display.print("1 Listening");
      display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
    }
    else
    {
      display.fillRect(0, OLED_LINE3, display.width(), 10, BLACK);
      display.setCursor(0,OLED_LINE3);
      display.print("2 Listening");
      display.fillRect(0, OLED_LINE4, display.width(), 10, BLACK);
    }
    display.display();
}

void COLED::writeFusionInt(const char* source, const char* dest, const char* type, const char* origin)
{
    m_mode = MODE_YSF;
    display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE1);
    display.printf("%s %.10s", type, source);
    display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE2);
    display.printf("  %.10s", dest);
    OLED_statusbar();
    display.display();
}

void COLED::clearFusionInt()
{
    display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE1);
    display.print("Listening");
    display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
    OLED_statusbar();
    display.display();
}

void COLED::writeP25Int(const char* source, bool group, unsigned int dest, const char* type)
{
    m_mode = MODE_P25;
    display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE1);
    display.printf("%s %.10s", type, source);
    display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE2);
    display.printf("  %s%u", group ? "TG" : "", dest);
    OLED_statusbar();
    display.display();
}

void COLED::clearP25Int()
{
    display.fillRect(0, OLED_LINE1, display.width(), 10, BLACK);
    display.setCursor(0,OLED_LINE1);
    display.print("Listening");
    display.fillRect(0, OLED_LINE2, display.width(), 10, BLACK);
    OLED_statusbar();
    display.display();
}

void COLED::writeCWInt()
{
    display.clearDisplay();
    display.setCursor(0,display.height()/2);
    display.setTextSize(3);
    display.print("CW TX");
    display.setTextSize(1);
    display.display();
    display.startscrollright(0x02,0x0f);
}

void COLED::clearCWInt()
{
    display.clearDisplay();
    display.setCursor(0,display.height()/2);
    display.setTextSize(2);
    display.print("Idle");
    display.setTextSize(1);
    display.display();
    display.startscrollright(0x02,0x0f);
}

void COLED::close()
{
    display.close();
}

void COLED::OLED_statusbar()
{
    display.stopscroll();
    display.fillRect(0, 0, display.width(), 16, BLACK);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    if (m_mode == MODE_DMR)
      display.drawBitmap(0, 0, logo_dmr_bmp, 48, 16, WHITE);
    else if (m_mode == MODE_DSTAR)
      display.drawBitmap(0, 0, logo_dstar_bmp, 64, 16, WHITE);
    else if (m_mode == MODE_YSF)
      display.print("Fusion");
    else if (m_mode == MODE_P25)
      display.print("P25");
    else
      display.drawBitmap(0, 0, logo_glcd_bmp, 16, 15, WHITE);
}
