//================================================================================
//
// program name   : tinyI2S.h
// date/author    : 2021/02/02 @chrmlinux03
//
// License 
// MIT Copyright(c)2020 M5Stack https://github.com/m5stack/ATOM-ECHO/tree/master/examples/Repeater
// MIT Copyright(c)2022 @chrmlinux03 https://github.com/chrmlinux/tinyI2S
//
//================================================================================

#ifndef _TINYI2S_H_
#define _TINYI2S_H_

#include <arduino.h>
#include <driver/i2s.h>

#define __DMA_BUF_CNT__ 		  (16)
#define __DMA_BUF_LEN__ 		  (64)

#define __I2S_BCK_PIN__       (19) // STICKC (-1)
#define __I2S_LRCK_PIN__  	  (33) // STICKC ( 0)
#define __I2S_DATA_OUT_PIN__  (22) // STICKC (-1)
#define __I2S_DATA_IN_PIN__   (23) // STICKC (34)

#define CONFIG_I2S_BCK_PIN 			(__I2S_BCK_PIN__)
#define CONFIG_I2S_LRCK_PIN			(__I2S_LRCK_PIN__)
#define CONFIG_I2S_DATA_OUT_PIN (__I2S_DATA_OUT_PIN__)
#define CONFIG_I2S_DATA_IN_PIN	(__I2S_DATA_IN_PIN__)
#define SPEAKER_I2S_NUMBER I2S_NUM_0

#define MODE_MIC 0
#define MODE_SPK 1


#define SAMPLING_RATE_48K (48000)
#define SAMPLING_RATE_44K (44000)
#define SAMPLING_RATE_32K (32000)
#define SAMPLING_RATE_24K (24000)
#define SAMPLING_RATE_16K (16000)
#define SAMPLING_RATE_8K 	( 8000)

#define RECORD_BUFSIZE_80K (1024 * 80)
#define RECORD_BUFSIZE_48K (1024 * 48)
#define RECORD_BUFSIZE_32K (1024 * 32)
#define RECORD_BUFSIZE_16K (1024 * 16)
#define RECORD_BUFSIZE_8K  (1024 *  8)

class tinyI2S {

  public:

    tinyI2S(void);
    ~tinyI2S(void);
    uint16_t begin(uint32_t, uint16_t);
    uint16_t end(void);
    uint16_t update(void);
    uint16_t record(void);
    uint16_t play(void);
    uint16_t setMode(uint16_t);
    bool isBufFull(void);
    uint16_t setBufFullFlag(bool);
    uint16_t console(void);
    uint16_t dump(uint32_t);
    uint8_t* get(void);
    uint16_t put(uint8_t*, uint16_t);
    uint16_t setParam(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

  private:

    int _data_offset = 0;

    uint16_t  InitI2SSpeakerOrMic(uint16_t);
    uint32_t   _recordBufSize = 0;
    uint16_t  _recordFreq = 0;
    uint8_t   *_recordBuf = NULL;
    uint16_t  _nowMode = 0;
    bool      _recordBufFull = false;

    uint16_t _dma_buf_cnt = __DMA_BUF_CNT__;
    uint16_t _dma_buf_len = __DMA_BUF_LEN__;
    uint16_t _i2s_bck_pin = __I2S_BCK_PIN__;
		uint16_t _i2s_lrck_pin = __I2S_LRCK_PIN__;
    uint16_t _i2s_data_out_pin = __I2S_DATA_OUT_PIN__;
		uint16_t _i2s_data_in_pin = __I2S_DATA_IN_PIN__;

};

#endif
