/*
 * font.h
 *
 *  Created on: Mar 9, 2025
 *      Author: Alva
 */

#ifndef LIBRARY_FONT_H_
#define LIBRARY_FONT_H_
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct {
  uint8_t h;
  uint8_t w;
  uint8_t *chars;
} ASCIIFont;

typedef enum
{
	COLOR_NORMAL = 0,
	COLOR_REVERSE
}OLED_ColorMode;

extern const ASCIIFont afont8x6;
extern const ASCIIFont afont12x6;
extern const ASCIIFont afont16x8;
extern const ASCIIFont afont24x12;
extern const uint8_t OLED_F8x16[][16];

/**
 * @brief 字体结构体
 * @note  字库前4字节存储utf8编码 剩余字节存储字模数据
 * @note 字库数据可以使用波特律动LED取模助手生成(https://led.baud-dance.com)
 */
typedef struct Font {
  uint8_t h;              // 字高度
  uint8_t w;              // 字宽度
  const uint8_t *chars;   // 字库 字库前4字节存储utf8编码 剩余字节存储字模数据
  uint8_t len;            // 字库长度 超过256则请改为uint16_t
  const ASCIIFont *ascii; // 缺省ASCII字体 当字库中没有对应字符且需要显示ASCII字符时使用
} Font;

extern const Font font16x16;

/**
 * @brief 图片结构体
 * @note  图片数据可以使用波特律动LED取模助手生成(https://led.baud-dance.com)
 */
typedef struct Image {
  uint8_t w;           // 图片宽度
  uint8_t h;           // 图片高度
  const uint8_t *data; // 图片数据
} Image;

extern const Image bilibiliImg;

extern const Image bilibiliTVImg;
extern const Image presetsLogoImg ;
#endif /* LIBRARY_FONT_H_ */
