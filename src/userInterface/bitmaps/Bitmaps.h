// -----------------------------------------------------------------------------------
// Bitmaps, for UserInterface

// Note: online XBM editors are a great help for creating these...

#include "../../libApp/u8g2ext/u8g2_ext.h"

// 8 x 16
static unsigned char battery_0_percent_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x18,0x7e,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,
 0x7e,0x18,0x00,0x00};

static unsigned char battery_25_percent_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x18,0x7e,0x42,0x42,0x42,0x42,0x42,0x42,0x7e,0x7e,
 0x7e,0x18,0x00,0x00};

static unsigned char battery_50_percent_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x18,0x7e,0x42,0x42,0x42,0x42,0x7e,0x7e,0x7e,0x7e,
 0x7e,0x18,0x00,0x00};

static unsigned char battery_75_percent_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x18,0x7e,0x42,0x42,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,
 0x7e,0x18,0x00,0x00};

static unsigned char battery_100_percent_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x18,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,
 0x7e,0x18,0x00,0x00};

static unsigned char W_bits[] U8X8_PROGMEM = {
 0x00,0x82,0x82,0x92,0x54,0x54,0x28,0x00,0x00,0x00,0x10,0x20,
 0x70,0x38,0x34,0x30};

static unsigned char E_bits[] U8X8_PROGMEM = {
 0x00,0x3c,0x04,0x04,0x1c,0x04,0x3c,0x00,0x00,0x00,0x10,0x08,
 0x1c,0x38,0x58,0x18};

// 16 x 16
static unsigned char align1_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x50, 0x02, 0x18, 0x04, 0x10, 0x08, 0x10, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00};

static unsigned char align2_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x48, 0x02, 0x14, 0x04, 0x10, 0x08, 0x08, 0x10, 0x04, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char align3_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x1c, 0x04, 0x10, 0x08, 0x18, 0x10, 0x10, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char align4_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x14, 0x04, 0x14, 0x08, 0x1c, 0x10, 0x10, 0x20, 0x10, 0x00, 0x00, 0x00 };

static unsigned char align5_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x1c, 0x04, 0x04, 0x08, 0x1c, 0x10, 0x10, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char align6_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x1c, 0x04, 0x04, 0x08, 0x1c, 0x10, 0x14, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char align7_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x1c, 0x04, 0x10, 0x08, 0x08, 0x10, 0x08, 0x20, 0x04, 0x00, 0x00, 0x00 };

static unsigned char align8_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x1c, 0x04, 0x14, 0x08, 0x1c, 0x10, 0x14, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char align9_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x1c, 0x04, 0x14, 0x08, 0x1c, 0x10, 0x10, 0x20, 0x10, 0x00, 0x00, 0x00 };

static unsigned char home_bits[] U8X8_PROGMEM = {
  0x00, 0x02, 0x00, 0x07, 0x80, 0x0f, 0xc0, 0x1f, 0x80, 0x3f, 0x00, 0x7f, 0x00, 0x7e, 0x00, 0x7f, 0x80, 0xfb, 0xc0, 0xc1, 0xe0, 0x01, 0xbc, 0x49, 0x9e, 0x49, 0x9e, 0x79, 0x8c, 0x49, 0x80, 0x49 };

static unsigned char parked_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0xfe, 0x7f, 0x02, 0x40, 0x02, 0x40, 0xe2, 0x43, 0x62, 0x46, 0x62, 0x46, 0x62, 0x46, 0xe2, 0x43, 0x62, 0x40, 0x62, 0x40, 0x62, 0x40, 0x62, 0x40, 0x02, 0x40, 0xfe, 0x7f, 0x00, 0x00 };

static unsigned char parking_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0xf9, 0x80, 0x99, 0x81, 0x99, 0x81, 0x99, 0x81, 0xf9, 0x80, 0x19, 0x80, 0x99, 0x84, 0x99, 0x8d, 0x99, 0x9f, 0x81, 0x8d, 0x81, 0x84, 0x01, 0x80, 0xff, 0xff };

static unsigned char parkingFailed_bits[] U8X8_PROGMEM = {
  0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0xf9, 0x90, 0x99, 0x91, 0x99, 0x91, 0x99, 0x91, 0xf9, 0x90, 0x19, 0x90, 0xd9, 0x93, 0x59, 0x90, 0xd9, 0x91, 0x41, 0x80, 0x41, 0x90, 0x01, 0x80, 0xff, 0xff };

static unsigned char guiding_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0xc0, 0x03, 0x20, 0x04, 0x10, 0x08, 0x08, 0x10, 0x8e, 0x71, 0x8e, 0x71, 0x08, 0x10, 0x10, 0x08, 0x20, 0x04, 0xc0, 0x03, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00 };

static unsigned char no_tracking_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x04,0x70,0x0e,0x70,0x0e,
 0x70,0x0e,0x70,0x0e,0x70,0x0e,0x70,0x0e,0x70,0x0e,0x70,0x0e,
 0x70,0x0e,0x20,0x04,0x00,0x00,0x00,0x00};

static unsigned char tracking_sid_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x0c,0x00,0x1c,0x00,
 0x3c,0x00,0x7c,0x08,0xfc,0x08,0x7c,0x7f,0x3c,0x3e,0x1c,0x1c,
 0x0c,0x3e,0x04,0x22,0x00,0x00,0x00,0x00};

static unsigned char tracking_sid_r_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x80,0x03,0x80,0x04,0x84,0x03,0x8c,0x04,0x9c,0x04,
 0x3c,0x00,0x7c,0x08,0xfc,0x08,0x7c,0x7f,0x3c,0x3e,0x1c,0x1c,
 0x0c,0x3e,0x04,0x22,0x00,0x00,0x00,0x00};

static unsigned char tracking_sid_rd_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x80,0x33,0x80,0x54,0x84,0x53,0x8c,0x54,0x9c,0x34,
 0x3c,0x00,0x7c,0x08,0xfc,0x08,0x7c,0x7f,0x3c,0x3e,0x1c,0x1c,
 0x0c,0x3e,0x04,0x22,0x00,0x00,0x00,0x00};

static unsigned char tracking_sid_f_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x80,0x03,0x80,0x00,0x84,0x03,0x8c,0x00,0x9c,0x00,
 0x3c,0x00,0x7c,0x08,0xfc,0x08,0x7c,0x7f,0x3c,0x3e,0x1c,0x1c,
 0x0c,0x3e,0x04,0x22,0x00,0x00,0x00,0x00};

static unsigned char tracking_sid_fd_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x80,0x33,0x80,0x50,0x84,0x53,0x8c,0x50,0x9c,0x30,
 0x3c,0x00,0x7c,0x08,0xfc,0x08,0x7c,0x7f,0x3c,0x3e,0x1c,0x1c,
 0x0c,0x3e,0x04,0x22,0x00,0x00,0x00,0x00};

static unsigned char tracking_lun_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x0c,0x00,0x1c,0x00,
 0x3c,0x00,0x7c,0x38,0xfc,0x1c,0x7c,0x0e,0x3c,0x0e,0x1c,0x0e,
 0x0c,0x0e,0x04,0x1c,0x00,0x38,0x00,0x00};

static unsigned char tracking_sol_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x0c,0x00,0x1c,0x00,
 0x3c,0x00,0x7c,0x00,0xfc,0x1c,0x7c,0x22,0x3c,0x41,0x1c,0x49,
 0x0c,0x41,0x04,0x22,0x00,0x1c,0x00,0x00};

static unsigned char tracking_kng_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x0c,0x00,0x1c,0x00,
 0x3c,0x00,0x7c,0x00,0xfc,0x24,0x7c,0x24,0x3c,0x14,0x1c,0x0c,
 0x0c,0x14,0x04,0x24,0x00,0x44,0x00,0x00};

static unsigned char slewing_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x02,0x0c,0x06,0x1c,0x0e,
 0x3c,0x1e,0x7c,0x3e,0xfc,0x7e,0x7c,0x3e,0x3c,0x1e,0x1c,0x0e,
 0x0c,0x06,0x04,0x02,0x00,0x00,0x00,0x00};

static unsigned char pec_play_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x00,0x00,0x80,0x03,0x80,0x04,0x80,0x04,0x80,0x04,
 0x80,0x03,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x38,0x40,
 0x44,0x62,0x80,0x5d,0x00,0x00,0x00,0x00};

static unsigned char pec_record_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x00,0x00,0x80,0x03,0x80,0x04,0x80,0x04,0x80,0x04,
 0x80,0x03,0x80,0x00,0xa0,0x02,0xc0,0x01,0x80,0x00,0x38,0x40,
 0x44,0x62,0x80,0x5d,0x00,0x00,0x00,0x00};

static unsigned char pec_wait_bits[] U8X8_PROGMEM = {
 0x00,0x00,0x80,0x03,0x80,0x04,0x90,0x14,0x90,0x14,0x90,0x14,
 0x90,0x13,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x38,0x40,
 0x44,0x62,0x80,0x5d,0x00,0x00,0x00,0x00};

static unsigned char ErrMot_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0xf8,0xc7,0x04,0xc8,0x04,0xc8,0x06,0xd8,0x04,0x08,
 0x04,0xc8,0xf8,0xc7,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x39,
 0xaa,0x12,0xaa,0x12,0x22,0x11,0x00,0x00};

static unsigned char ErrAltLowHigh_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0x00,0xc0,0x00,0xc0,0x00,0xc0,0xf0,0xc7,0x88,0x08,
 0xc4,0xd1,0xa4,0xd2,0x82,0x20,0x02,0x20,0x02,0x20,0x82,0x20,
 0xa2,0x22,0xc2,0x21,0x80,0x00,0x00,0x00};

static unsigned char ErrAltLow_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0x00,0xc0,0x00,0xc0,0x00,0xc0,0xf0,0xc7,0x08,0x08,
 0x04,0xd0,0x04,0xd0,0x02,0x20,0x02,0x20,0x82,0x20,0x82,0x20,
 0xa2,0x22,0xc2,0x21,0x80,0x00,0x00,0x00};

static unsigned char ErrLimitSW_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0x00,0xc0,0x00,0xc0,0x00,0xc0,0x08,0xcc,0x90,0x0c,
 0xbe,0xcc,0x10,0xcd,0x08,0x0d,0x00,0x0e,0x00,0x0e,0x00,0x0c,
 0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char ErrDecGEM_bits[] U8X8_PROGMEM = {
 0x00,0x60,0x00,0x68,0x00,0x65,0x00,0x63,0x20,0x67,0x40,0x00,
 0x80,0x60,0x50,0x61,0x20,0x02,0x70,0x00,0xe8,0x00,0x64,0x00,
 0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00};

static unsigned char ErrDecFork_bits[] = {
 0x00,0x60,0x00,0x60,0x00,0x60,0xe0,0x61,0x20,0x61,0x20,0x01,
 0x20,0x61,0xa4,0x61,0x68,0x01,0x3f,0x02,0x68,0x14,0xa4,0x18,
 0xe0,0x1d,0x00,0x12,0x00,0x1f,0x00,0x00};

static unsigned char ErrAzmDOB_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0x00,0xc0,0x00,0xc0,0x00,0xc0,0x40,0xc0,0xa0,0x00,
 0x10,0xc1,0x20,0xc2,0x40,0x07,0x84,0x0c,0xa8,0x14,0xb0,0x0c,
 0xb8,0x04,0x80,0x04,0xc0,0x0f,0x00,0x00};

static unsigned char ErrRaGEM_bits[] U8X8_PROGMEM = {
 0x00,0x60,0x00,0x60,0x00,0x60,0x01,0x60,0x0a,0x60,0x4c,0x00,
 0x8e,0x60,0x00,0x61,0xa0,0x02,0xc0,0x04,0xe0,0x01,0xd0,0x00,
 0xc8,0x00,0xc0,0x00,0xc0,0x00,0xc0,0x00};

static unsigned char ErrRaFork_bits[] U8X8_PROGMEM = {
 0x00,0x60,0x00,0x60,0x00,0x60,0xe0,0x61,0x20,0x61,0x20,0x01,
 0x20,0x61,0xa0,0x61,0x60,0x01,0x20,0x02,0x60,0x14,0xa4,0x18,
 0xe8,0x1d,0x1f,0x12,0x08,0x1f,0x04,0x00};

static unsigned char ErrMER_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0x00,0xc0,0x00,0xc0,0xf0,0xc1,0x4c,0xc6,0x02,0x08,
 0xe2,0xc9,0x15,0xd2,0x4d,0x14,0x1d,0x14,0x41,0x14,0x01,0x10,
 0x41,0x10,0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char ErrAltHigh_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0x00,0xc0,0x00,0xc0,0x00,0xc0,0xf0,0xc7,0x88,0x08,
 0xc4,0xd1,0xa4,0xd2,0x82,0x20,0x82,0x20,0x02,0x20,0x02,0x20,
 0x02,0x20,0x02,0x20,0x00,0x00,0x00,0x00};

static unsigned char ErrNV_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0x00,0xc0,0x00,0xc0,0x00,0xc0,0x00,0xc0,0xa4,0x08,
 0xac,0xc8,0x2c,0xc5,0x34,0x05,0x34,0x02,0x24,0x02,0x00,0x00,
 0x00,0x00,0x00,0x5a,0x00,0xc9,0x00,0x00};

static unsigned char ErrSite_bits[] U8X8_PROGMEM = {
 0x00,0xc0,0xfe,0xc0,0xc6,0xc0,0xba,0xc0,0xa2,0xc0,0xb2,0x00,
 0xa2,0xc0,0xba,0xc8,0x82,0x1c,0xfe,0x22,0x00,0x49,0x00,0x49,
 0x00,0x51,0x00,0x22,0x00,0x1c,0x00,0x08};

static unsigned char ErrWeather_bits[] U8X8_PROGMEM = {
 0x10,0xc0,0x11,0xc1,0x82,0xc0,0x38,0xc7,0xc4,0xc8,0x65,0x08,
 0x14,0xc8,0x08,0xd0,0x0a,0x20,0x39,0x23,0xc0,0x24,0x00,0x1c,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static unsigned char ErrOther_bits[] U8X8_PROGMEM = {
 0x00,0x60,0x00,0x60,0xe0,0x61,0xf0,0x63,0x30,0x66,0x30,0x06,
 0x00,0x66,0x00,0x63,0x80,0x01,0xc0,0x00,0xc0,0x00,0x00,0x00,
 0xc0,0x00,0xc0,0x00,0x00,0x00,0x00,0x00};

static const unsigned char onstep_logo_bits[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x80, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x7F, 0x00, 
  0x00, 0x00, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xFE, 0xFF, 0x07, 0x00, 0x00, 0x00, 0xFE, 0x3F, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0xFF, 
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x7F, 0xF3, 0x0F, 
  0x00, 0x00, 0x80, 0x1F, 0xFC, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xE0, 0x1F, 0xC3, 0x0F, 0x00, 0x00, 0xC0, 0x07, 0xF0, 0xC1, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xE0, 0x07, 0x83, 0x1F, 0x00, 0x00, 0xC0, 0x03, 
  0xE0, 0xC1, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x07, 0x03, 0x7F, 
  0x00, 0x00, 0xC0, 0x03, 0xE0, 0xC1, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xE0, 0x03, 0x03, 0x7E, 0x1C, 0x3F, 0xC0, 0x03, 0xC0, 0xF1, 0x1F, 0xF0, 
  0x0F, 0xF0, 0xFC, 0x01, 0xF0, 0x03, 0x03, 0x3E, 0xFC, 0xFF, 0xC0, 0x03, 
  0x00, 0xF0, 0x1F, 0xFC, 0x3F, 0xF0, 0xFF, 0x03, 0xF8, 0x01, 0x03, 0x3E, 
  0xFC, 0xFF, 0xC1, 0x0F, 0x00, 0xF0, 0x1F, 0xFE, 0x7F, 0xF0, 0xFF, 0x07, 
  0xF8, 0x01, 0x03, 0x3C, 0xFC, 0xE0, 0x81, 0xFF, 0x00, 0xC0, 0x03, 0x1E, 
  0x78, 0xF0, 0x83, 0x0F, 0xF0, 0x01, 0x03, 0x7C, 0x7C, 0xE0, 0x01, 0xFF, 
  0x07, 0xC0, 0x03, 0x0F, 0xF0, 0xF0, 0x01, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x01, 0x03, 0x7C, 0x3C, 0xC0, 0x03, 0x00, 
  0xFF, 0xC1, 0x83, 0x07, 0xF0, 0xF1, 0x00, 0x1E, 0xF8, 0x01, 0x03, 0x3C, 
  0x3C, 0xC0, 0x01, 0x00, 0xF8, 0xC1, 0x83, 0xFF, 0xFF, 0xF1, 0x00, 0x1E, 
  0xF8, 0x01, 0x03, 0x3E, 0x3C, 0xC0, 0x03, 0x00, 0xE0, 0xC3, 0x83, 0xFF, 
  0xFF, 0xF1, 0x00, 0x1E, 0xF0, 0x03, 0x03, 0x3E, 0x3C, 0xC0, 0xE3, 0x00, 
  0xC0, 0xC3, 0x83, 0xFF, 0xFF, 0xF0, 0x00, 0x1E, 0xE0, 0x03, 0x03, 0x7E, 
  0x3C, 0xC0, 0xE3, 0x01, 0xC0, 0xC3, 0x83, 0x07, 0x00, 0xF0, 0x00, 0x1E, 
  0xC0, 0x07, 0x03, 0x7F, 0x3C, 0xC0, 0xE3, 0x01, 0xC0, 0xC3, 0x83, 0x07, 
  0x00, 0xF0, 0x00, 0x1E, 0xE0, 0x07, 0x83, 0x1F, 0x3C, 0xC0, 0xC3, 0x03, 
  0xC0, 0xC3, 0x83, 0x0F, 0xE0, 0xF0, 0x00, 0x0E, 0xE0, 0x0F, 0xC3, 0x0F, 
  0x3C, 0xC0, 0xC3, 0x07, 0xE0, 0xC1, 0x03, 0x0F, 0xF0, 0xF0, 0x00, 0x0F, 
  0xC0, 0x3F, 0xF3, 0x07, 0x3C, 0xC0, 0x83, 0x1F, 0xF8, 0x81, 0x07, 0x1F, 
  0xF8, 0xF0, 0x81, 0x0F, 0x00, 0xFE, 0xFF, 0x0F, 0x3C, 0xC0, 0x83, 0xFF, 
  0xFF, 0x80, 0x3F, 0xFE, 0x7F, 0xF0, 0xFF, 0x07, 0x00, 0xFE, 0xFF, 0x07, 
  0x3C, 0xC0, 0x03, 0xFE, 0x7F, 0x80, 0x3F, 0xFC, 0x3F, 0xF0, 0xFF, 0x03, 
  0x00, 0xFE, 0x7F, 0x00, 0x3C, 0xC0, 0x01, 0xFC, 0x1F, 0x00, 0x3F, 0xF0, 
  0x1F, 0xF0, 0xFE, 0x01, 0x00, 0xC4, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x80, 0x23, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 
  0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
  0x76, 0x18, 0xF0, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0x12, 0x11, 0x50, 0x01, 0x00, 0x83, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xBB, 0x6B, 0x20, 0xE6, 
  0x8D, 0xDA, 0x97, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 
  0x2B, 0x59, 0x20, 0x55, 0x4B, 0x89, 0x5C, 0x02, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xC0, 0x26, 0x49, 0xA0, 0x3B, 0xC9, 0x91, 0x24, 0x02, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x92, 0x24, 0x90, 0x88, 
  0x24, 0x55, 0x22, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 
  0xB2, 0x65, 0xB8, 0xBB, 0xBC, 0xCF, 0xE2, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
