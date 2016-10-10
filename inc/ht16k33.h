/*
 * ht16k33.h
 *
 *  Created on: Oct 6, 2016
 *      Author: timppa
 */

#ifndef HT16K33_H_
#define HT16K33_H_

#define SEGMENT_IDX_NUMBER1 0
#define SEGMENT_IDX_NUMBER2 1
#define SEGMENT_IDX_COLON   2
#define SEGMENT_IDX_NUMBER3 3
#define SEGMENT_IDX_NUMBER4 4

#define HT16K33_BLINK_CMD       0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF       0x00
#define HT16K33_BLINK_2HZ       0x01
#define HT16K33_BLINK_1HZ       0x02
#define HT16K33_BLINK_HALFHZ    0x03

#define HT16K33_CMD_BRIGHTNESS  0xE0

#define HT16K33_CMD_OSCILLATOR_OFF 0x20
#define HT16K33_CMD_OSCILLATOR_ON  0x21

#define SEVENSEG_DIGITS 5

//
// number segments are displayed with combination
// of following values:
//
//   1 1 1
// 32     2
// 32     2
// 32     2
//   64 64
// 16     4
// 16     4
// 16     4
//   8 8 8
//

//
// 0 (zero) is 1+2+4+8+16+32 = 63
#define SEGMENTS_0 63
// 1 (one)  is 2+4 = 6
#define SEGMENTS_1 6

//
// define these (correctly), now the all display as "-"
//
//#define SEGMENTS_2 64
//#define SEGMENTS_3 64
//#define SEGMENTS_4 64
//#define SEGMENTS_5 64
//#define SEGMENTS_6 64
//#define SEGMENTS_7 64
//#define SEGMENTS_8 64
//#define SEGMENTS_9 64
//#define SEGMENTS_A 64
//#define SEGMENTS_B 64
//#define SEGMENTS_C 64
//#define SEGMENTS_D 64
//#define SEGMENTS_E 64

#define SEGMENTS_2 0x5B
#define SEGMENTS_3 0x4F
#define SEGMENTS_4 0x66
#define SEGMENTS_5 0x6D
#define SEGMENTS_6 0x7D
#define SEGMENTS_7 0x07
#define SEGMENTS_8 0x7F
#define SEGMENTS_9 0x6F

#define SEGMENTS_MINUS 0x40

#define SEGMENTS_A 0x77
#define SEGMENTS_B 0x7C
#define SEGMENTS_C 0x39
#define SEGMENTS_D 0x5E
#define SEGMENTS_E 0x79
//
// Letters may be needed later: some of them can be displayed correctly
//
// F is 1+32+64+16 = 113
#define SEGMENTS_F 0x71

#define SEGMENTS_d    (char)0x5E
#define SEGMENTS_m    (char)0x37
#define SEGMENTS_n    (char)0x54
#define SEGMENTS_o    (char)0x5C
#define SEGMENTS_P    (char)0x73
#define SEGMENTS_S    (char)0x6C
#define SEGMENTS_u    (char)0x1C
#define SEGMENTS_w    (char)0x7E

#define SEGMENTS_ALL  (char)0xFF
#define SEGMENTS_NONE (char)0x00

#endif /* HT16K33_H_ */
