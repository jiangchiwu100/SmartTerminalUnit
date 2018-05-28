/**
  * @file   imageArray.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/20
  * @brief   
  */

#ifndef __IMAGEARRAY_H
#define	__IMAGEARRAY_H

#include "stdint.h"

/* ÕºœÒœ‘ æ */
typedef struct{
	uint16_t wide;
	uint16_t high;
	const uint8_t *image;
	uint8_t imageNumber;
}ImageInfo;

extern ImageInfo imageTab[];
extern const uint8_t imageTabItems;
#endif /* END __IMAGEARRAY_H */
