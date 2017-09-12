#ifndef IS31FL3728_H
#define IS31FL3728_H

#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDR_BASE B01100000
#define I2C_ADDR_ALT0 B01100000
#define I2C_ADDR_ALT1 B01100001
#define I2C_ADDR_ALT2 B01100010
#define I2C_ADDR_ALT3 B01100011
#define I2C_ADDR_MASK B00000011

#define MATRIX_SIZE_8X8  0
#define MATRIX_SIZE_7X9  1
#define MATRIX_SIZE_6X10 2
#define MATRIX_SIZE_5X11 3
#define MATRIX_SIZE_MASK 3

#define MATRIX_MIN_ROWS 8
#define MATRIX_MAX_ROWS 11
#define MATRIX_MIN_COLS 5
#define MATRIX_MAX_COLS 8

#define AUDIO_GAIN_0DB  0
#define AUDIO_GAIN_3DB  1
#define AUDIO_GAIN_6DB  2
#define AUDIO_GAIN_9DB  3
#define AUDIO_GAIN_12DB 4
#define AUDIO_GAIN_15DB 5
#define AUDIO_GAIN_18DB 6
#define AUDIO_GAIN_M6DB 7
#define AUDIO_GAIN_MASK 7

#define ROW_CURRENT_40MA B0000
#define ROW_CURRENT_45MA B0001
#define ROW_CURRENT_50MA B0010
#define ROW_CURRENT_55MA B0011
#define ROW_CURRENT_60MA B0100
#define ROW_CURRENT_65MA B0101
#define ROW_CURRENT_70MA B0110
#define ROW_CURRENT_75MA B0111
#define ROW_CURRENT_05MA B1000
#define ROW_CURRENT_10MA B1001
#define ROW_CURRENT_15MA B1010
#define ROW_CURRENT_20MA B1011
#define ROW_CURRENT_25MA B1100
#define ROW_CURRENT_30MA B1101
#define ROW_CURRENT_35MA B1110
#define ROW_CURRENT_XXMA B1111
#define ROW_CURRENT_MASK B1111

#define REG_ADDR_CONFIGURATION   0x00
#define REG_ADDR_COLUMN_DATA     0x01
#define REG_ADDR_UPDATE_COLUMN   0x0C
#define REG_ADDR_LIGHTING_EFFECT 0x0D
#define REG_ADDR_AUDIO_EQUALIZER 0x0F

#define BIT_CONFIG_SSD      7
#define BIT_CONFIG_AUDIO_EN 2
#define BIT_CONFIG_ADM      0

#define BIT_EFFECT_AUDIO_GAIN  4
#define BIT_EFFECT_ROW_CURRENT 0

#define BIT_AUDIO_EQ_EN     6

class IS32FL3728_FB
{
 public:
  IS32FL3728_FB();
 public:
  void clear();
  void clearPixel(const uint8_t x, const uint8_t y);
  void setPixel(const uint8_t x, const uint8_t y);
  void setFont(const uint8_t *font, const uint8_t n_chars = 1, const uint8_t n_rows = 8);
  void setCharacter(const uint8_t c);
  void setBitmap(const uint8_t *data, const uint8_t n_rows = 8);
  uint8_t getRow(const uint8_t y);
 private:
  uint8_t *m_Font;
  uint8_t m_FontSize;
  uint8_t m_FontHeight;
  uint8_t m_Data[MATRIX_MAX_ROWS];
};

class IS32FL3728_TWI
{
 public:
  IS32FL3728_TWI();
 public:
  void begin(const uint8_t addr);
  void enableDisplay();
  void disableDisplay();
  void enableAudioInput();
  void disableAudioInput();
  void getMetrics(uint8_t& width, uint8_t& height);
  void setMatrixSize(const uint8_t value);
  void updateDisplay();
  void setAudioGain(const uint8_t value);
  void setCurrentLimit(const uint8_t value);
  void enableEqualizer();
  void disableEqualizer();
  //
  IS32FL3728_FB& frameBuffer() { return m_FrameBuffer; }
  IS32FL3728_FB& fb() { return m_FrameBuffer; }
 private:
  uint8_t readReg(const uint8_t addr);
  void writeReg(const uint8_t addr, const uint8_t data);
  uint8_t makeConfigReg();
  uint8_t makeEffectReg();
 private:
  uint8_t m_Addr;
  bool m_Shutdown;
  bool m_AudioInput;
  uint8_t m_MatrixSize;
  uint8_t m_AudioInputGain;
  uint8_t m_CurrentLimit;
  //bool m_AudioEqualizer;
  IS32FL3728_FB m_FrameBuffer;
};

#endif //IS31FL3728_H
