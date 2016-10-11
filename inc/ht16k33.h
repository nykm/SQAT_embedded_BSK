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

#define SEGMENTS_ALL  (char)0xFF
#define SEGMENTS_NONE (char)0x00

#endif /* HT16K33_H_ */
