//================================================================================
//
// program name   : tinyI2S.cpp
// date/author    : 2021/02/02 @chrmlinux03
//
// MIT License Copyright(c)2020 M5Stack https://github.com/m5stack/ATOM-ECHO/tree/master/examples/Repeater
// MIT License Copyright(c)2022 @chrmlinux03 https://github.com/chrmlinux/tinyI2S
//
//================================================================================

#include <arduino.h>
#include "tinyI2S.h"

//======================================
// InitI2SSpeakerOrMic
//======================================
uint16_t tinyI2S::InitI2SSpeakerOrMic(uint16_t mode)
{
  esp_err_t err = ESP_OK;

  i2s_driver_uninstall(SPEAKER_I2S_NUMBER);

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER),
    .sample_rate = _recordFreq,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = _dma_buf_cnt,
    .dma_buf_len = _dma_buf_len,
  };

  if (mode == MODE_MIC) {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
  } else {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    i2s_config.use_apll = false;
    i2s_config.tx_desc_auto_clear = true;
  }

  err += i2s_driver_install(SPEAKER_I2S_NUMBER, &i2s_config, 0, NULL);
  i2s_pin_config_t tx_pin_config;

  tx_pin_config.bck_io_num = _i2s_bck_pin;
  tx_pin_config.ws_io_num = _i2s_lrck_pin;
  tx_pin_config.data_out_num = _i2s_data_out_pin;
  tx_pin_config.data_in_num = _i2s_data_in_pin;

  err += i2s_set_pin(SPEAKER_I2S_NUMBER, &tx_pin_config);
  err += i2s_set_clk(SPEAKER_I2S_NUMBER, _recordFreq, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

  return err;
}


//======================================
// tinyI2S
//======================================
tinyI2S::tinyI2S(void)
{
}

tinyI2S::~tinyI2S(void)
{
}

//======================================
// get
//======================================
uint8_t* tinyI2S::get(void)
{
  return _recordBuf;
}

//======================================
// put
//======================================
uint16_t tinyI2S::put(uint8_t *dt, uint16_t cnt)
{
  memcpy(_recordBuf, dt, cnt);
  return 0;
}

//======================================
// setParam
//======================================
uint16_t tinyI2S::setParam(
  uint16_t dma_buf_cnt,
  uint16_t dma_buf_len,
  uint16_t i2s_bck_pin,
  uint16_t i2s_lrck_pin,
  uint16_t i2s_data_out_pin,
  uint16_t i2s_data_in_pin
)
{
  _dma_buf_cnt = dma_buf_cnt;
  _dma_buf_len = dma_buf_len;
  _i2s_bck_pin = i2s_bck_pin;
  _i2s_lrck_pin = i2s_lrck_pin;
  _i2s_data_out_pin = i2s_data_out_pin;
  _i2s_data_in_pin = i2s_data_in_pin;
}

//======================================
// begin
//======================================
uint16_t tinyI2S::begin(uint32_t recordBufSize, uint16_t recordFreq)
{
  _recordBufSize = recordBufSize;
  _recordFreq = recordFreq;
  _recordBuf = new uint8_t[_recordBufSize];

  Serial.printf("i2s_bck_pin=%d i2s_lrck_pin=%d i2s_data_out_pin i2s_data_in_pin=%d dma_buf_cnt=%d dma_buf_len=%d recordBufSize=%d recordFreq=%d\n",
                _i2s_bck_pin, _i2s_lrck_pin, _i2s_data_out_pin, _i2s_data_in_pin, _dma_buf_cnt, _dma_buf_len, _recordBufSize, _recordFreq);


  return 0;
}

//======================================
// isBufFull
//======================================
bool tinyI2S::isBufFull(void)
{
  return _recordBufFull;
}

//======================================
// setBufFullFlag
//======================================
uint16_t tinyI2S::setBufFullFlag(bool flag)
{
  _recordBufFull = flag;
  return 0;
}

//======================================
// end
//======================================
uint16_t tinyI2S::end(void)
{
  free(_recordBuf);
  return 0;
}

//======================================
// setMode
//======================================
uint16_t tinyI2S::setMode(uint16_t mode)
{
  _nowMode = mode;
  InitI2SSpeakerOrMic(_nowMode);
  if (_nowMode == MODE_MIC) _recordBufFull = false;
  return 0;
}

//======================================
//
// record
//
//======================================
uint16_t tinyI2S::record(void)
{
  size_t bytes_read;

  i2s_read(SPEAKER_I2S_NUMBER, (uint8_t *)(_recordBuf + _data_offset), _dma_buf_len, &bytes_read, (100 / portTICK_RATE_MS));
  _data_offset += _dma_buf_len;
  if (_data_offset > (_recordBufSize - 1)) _recordBufFull = true;
  return 0;
}

//======================================
//
// play
//
//======================================
uint16_t tinyI2S::play(void)
{
  size_t bytes_written;

  i2s_write(SPEAKER_I2S_NUMBER, (uint8_t *)_recordBuf, _data_offset, &bytes_written, portMAX_DELAY);

  _data_offset = 0;
  _recordBufFull = false;
  return 0;
}

//======================================
// console
//======================================
uint16_t tinyI2S::console(void)
{
  uint16_t dtMax = 0;

  uint32_t sum = 0;
  uint32_t ave = 0;
  for (int i = 0; i < _recordBufSize; i++) {
    sum += _recordBuf[i];
  }
  ave = sum / _recordBufSize;
  Serial.println(ave);

  _data_offset = 0;
  _recordBufFull = false;
  return dtMax;
}

//======================================
// dump
//======================================
uint16_t tinyI2S::dump(uint32_t len)
{
  static uint16_t cnt = 0;
  Serial.println();
  for (uint32_t i = 0; i < len / 2; i += 2) {
    Serial.printf("%02x%02x ", _recordBuf[i], _recordBuf[i + 1]);
    cnt = (cnt + 1) % 16;
    if (cnt == 0) Serial.println();
  }

  _data_offset = 0;
  _recordBufFull = false;
}

//======================================
// update
//======================================
uint16_t tinyI2S::update(void)
{
  delay(1);
  return 0;
}
