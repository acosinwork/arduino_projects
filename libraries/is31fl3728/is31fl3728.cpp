#include "is31fl3728.h"

IS32FL3728_FB::IS32FL3728_FB()
{
 m_Font = 0;
 m_FontSize = 1;
 m_FontHeight = 8;
 clear();
}

void IS32FL3728_FB::clear()
{
 for (uint8_t i=0; i<MATRIX_MAX_ROWS; i++) m_Data[i] = 0;
}

void IS32FL3728_FB::clearPixel(const uint8_t x, const uint8_t y)
{
 uint8_t i=x%8, j=y%MATRIX_MAX_ROWS;
 m_Data[j] = m_Data[j] & ~_BV(i);
}

void IS32FL3728_FB::setPixel(const uint8_t x, const uint8_t y)
{
 uint8_t i=x%8, j=y%MATRIX_MAX_ROWS;
 m_Data[j] = m_Data[j] | _BV(i);
}

void IS32FL3728_FB::setFont(const uint8_t *font, const uint8_t n_chars, const uint8_t n_rows)
{
 m_Font = (uint8_t *)font;
 m_FontSize = n_chars;
 m_FontHeight = n_rows;
}

void IS32FL3728_FB::setCharacter(const uint8_t c)
{
 if (m_Font)
 {
  if (c<m_FontSize)
  {
   setBitmap(&m_Font[c*m_FontHeight],m_FontHeight);
  }
 }
}

void IS32FL3728_FB::setBitmap(const uint8_t *data, const uint8_t n_rows)
{
 uint8_t n=min(n_rows, MATRIX_MAX_ROWS);
 for (uint8_t i=0; i<n; i++) m_Data[i] = data[i];
}

uint8_t IS32FL3728_FB::getRow(const uint8_t y)
{
 return m_Data[y % MATRIX_MAX_ROWS];
}

//

IS32FL3728_TWI::IS32FL3728_TWI()
{
 m_Addr = I2C_ADDR_ALT0;
}

void IS32FL3728_TWI::begin(const uint8_t addr)
{
 m_Addr = I2C_ADDR_BASE | (addr & I2C_ADDR_MASK);
 m_Shutdown = false;
 m_AudioInput = false;
 m_MatrixSize = MATRIX_SIZE_8X8;
 //m_AudioEqualizer = false;
 m_AudioInputGain = AUDIO_GAIN_0DB;
 m_CurrentLimit = ROW_CURRENT_40MA;
 //
 Wire.begin();
 writeReg(REG_ADDR_CONFIGURATION, makeConfigReg());
 writeReg(REG_ADDR_LIGHTING_EFFECT, makeEffectReg());
 disableEqualizer();
}

uint8_t IS32FL3728_TWI::readReg(const uint8_t addr)
{
 Wire.beginTransmission(m_Addr);
 Wire.write(addr);
 Wire.endTransmission();
 Wire.requestFrom(m_Addr, 1);
 while (!Wire.available());
 uint8_t data = Wire.read();
 return data; 
}

void IS32FL3728_TWI::writeReg(const uint8_t addr, const uint8_t data)
{
 Wire.beginTransmission(m_Addr);
 Wire.write(addr);
 Wire.write(data);
 Wire.endTransmission();
}

uint8_t IS32FL3728_TWI::makeConfigReg()
{
 uint8_t data = 0;
 if (m_Shutdown) data |= _BV(BIT_CONFIG_SSD);
 if (m_AudioInput) data |= _BV(BIT_CONFIG_AUDIO_EN);
 data |= m_MatrixSize << BIT_CONFIG_ADM;
 return data;
}

void IS32FL3728_TWI::enableDisplay()
{
 m_Shutdown = false;
 uint8_t data = makeConfigReg();
 writeReg(REG_ADDR_CONFIGURATION, data);
}

void IS32FL3728_TWI::disableDisplay()
{
 m_Shutdown = true;
 uint8_t data = makeConfigReg();
 writeReg(REG_ADDR_CONFIGURATION, data);
}

void IS32FL3728_TWI::enableAudioInput()
{
 m_AudioInput = true;
 uint8_t data = makeConfigReg();
 writeReg(REG_ADDR_CONFIGURATION, data);
}

void IS32FL3728_TWI::disableAudioInput()
{
 m_AudioInput = false;
 uint8_t data = makeConfigReg();
 writeReg(REG_ADDR_CONFIGURATION, data);
}

void IS32FL3728_TWI::getMetrics(uint8_t& width, uint8_t& height)
{
 switch (m_MatrixSize)
 {
  case MATRIX_SIZE_8X8: { width=8; height=8; break; }
  case MATRIX_SIZE_7X9: { width=7; height=9; break; }
  case MATRIX_SIZE_6X10: { width=6; height=10; break; }
  case MATRIX_SIZE_5X11: { width=5; height=11; break; }
 }
}

void IS32FL3728_TWI::setMatrixSize(const uint8_t value)
{
 m_MatrixSize = (value & MATRIX_SIZE_MASK);
 uint8_t data = makeConfigReg();
 writeReg(REG_ADDR_CONFIGURATION, data);
}

void IS32FL3728_TWI::updateDisplay()
{
 uint8_t w,h;
 getMetrics(w,h);
 for (uint8_t i=0; i<h; i++)
 {
  uint8_t data = m_FrameBuffer.getRow(i);
  writeReg(REG_ADDR_COLUMN_DATA+i, data);
 }
 writeReg(REG_ADDR_UPDATE_COLUMN, 0xff);
}

uint8_t IS32FL3728_TWI::makeEffectReg()
{
 uint8_t data = (m_AudioInputGain << BIT_EFFECT_AUDIO_GAIN) |
                (m_CurrentLimit << BIT_EFFECT_ROW_CURRENT);
 return data;
}

void IS32FL3728_TWI::setAudioGain(const uint8_t value)
{
 m_AudioInputGain = value & AUDIO_GAIN_MASK;
 uint8_t data = makeEffectReg();
 writeReg(REG_ADDR_LIGHTING_EFFECT, data);
}

void IS32FL3728_TWI::setCurrentLimit(const uint8_t value)
{
 m_CurrentLimit = value & ROW_CURRENT_MASK;
 uint8_t data = makeEffectReg();
 writeReg(REG_ADDR_LIGHTING_EFFECT, data);
}

void IS32FL3728_TWI::enableEqualizer()
{
 //m_AudioEqualizer = true;
 writeReg(REG_ADDR_AUDIO_EQUALIZER, _BV(BIT_AUDIO_EQ_EN));
}

void IS32FL3728_TWI::disableEqualizer()
{
 //m_AudioEqualizer = false;
 writeReg(REG_ADDR_AUDIO_EQUALIZER, 0);
}
