#include "common.h"
#include "tiles.h"
const uint8_t tiles[54][8] = 
{
	{0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,},
	{0x11,0x55,0x44,0x55,0x11,0x55,0x44,0x55,},
	{0xFE,0xFE,0xF8,0xF0,0xF0,0xE0,0x80,0x80,},
	{0x80,0xC0,0xE0,0xE0,0xF8,0xF0,0xFE,0xFF,},
	{0x01,0x03,0x07,0x07,0x1F,0x3F,0x3F,0xFF,},
	{0xFF,0x7F,0x1F,0x0F,0x0F,0x07,0x01,0x01,},
	{0xFE,0xCB,0x9F,0xCF,0x9B,0xCF,0x9F,0xFE,},
	{0xFE,0xCF,0x9F,0xCF,0x9F,0xCF,0x9F,0xFE,},
	{0x3F,0x37,0xFF,0x2F,0xFF,0x37,0xFF,0x3F,},
	{0x3F,0x3F,0xFF,0x3F,0xFF,0x3F,0xFF,0x3F,},
	{0xFF,0xFD,0xC7,0xCD,0xC7,0xE6,0xC6,0xCE,},
	{0xFF,0xFD,0xC7,0xCD,0xC7,0xE6,0xC6,0xCE,},
	{0x7E,0xFB,0x99,0x9F,0xFB,0xF9,0xFF,0x7E,},
	{0xFB,0x99,0x9F,0xFB,0xF9,0x7F,0x3E,0x3C,},
	{0x3C,0x7E,0xBB,0x81,0x81,0xBF,0x7E,0x3C,},
	{0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C,},
	{0x00,0x7C,0xA2,0x92,0x92,0x8A,0x7C,0x00,},
	{0x00,0x00,0x00,0x04,0xFE,0x00,0x00,0x00,},
	{0x00,0xC4,0xA2,0xA2,0x92,0x92,0x8C,0x00,},
	{0x00,0x44,0x82,0x92,0x92,0x92,0x6C,0x00,},
	{0x00,0x30,0x28,0x24,0x22,0xFE,0x20,0x00,},
	{0x00,0x4E,0x8A,0x8A,0x8A,0x8A,0x72,0x00,},
	{0x00,0x7C,0x92,0x92,0x92,0x92,0x64,0x00,},
	{0x00,0x0E,0x02,0xC2,0x22,0x12,0x0E,0x00,},
	{0x00,0x6C,0x92,0x92,0x92,0x92,0x6C,0x00,},
	{0x00,0x4C,0x92,0x92,0x92,0x92,0x7C,0x00,},
	{0x00,0xC0,0x30,0x2C,0x22,0x22,0xFE,0x00,},
	{0x00,0x82,0xFE,0x92,0x92,0x92,0x6C,0x00,},
	{0x00,0x38,0x44,0x82,0x82,0x82,0x44,0x00,},
	{0x00,0x82,0xFE,0x82,0x82,0x42,0x3C,0x00,},
	{0x00,0x82,0xFE,0x92,0x92,0x92,0x82,0x00,},
	{0x00,0x82,0xFE,0x12,0x12,0x12,0x02,0x00,},
	{0x00,0x38,0x44,0x82,0x92,0x92,0x74,0x00,},
	{0x00,0x82,0x7E,0x10,0x10,0x10,0xFE,0x00,},
	{0x00,0x00,0x82,0xFE,0x82,0x00,0x00,0x00,},
	{0x00,0x42,0x82,0x82,0x7E,0x02,0x02,0x00,},
	{0x00,0x82,0x7E,0x10,0x28,0x44,0x82,0x00,},
	{0x00,0x82,0xFE,0x80,0x80,0x80,0xC0,0x00,},
	{0x80,0xFE,0x04,0x08,0x08,0x04,0xFE,0x00,},
	{0x80,0xFE,0x04,0x08,0x10,0x20,0xFE,0x00,},
	{0x00,0x7C,0x82,0x82,0x82,0x82,0x7C,0x00,},
	{0x00,0x82,0xFE,0x22,0x22,0x22,0x1C,0x00,},
	{0x00,0x7C,0x82,0xA2,0xA2,0x42,0xBC,0x00,},
	{0x00,0x82,0xFE,0x12,0x32,0x52,0x8C,0x00,},
	{0x00,0x4C,0x92,0x92,0x92,0x92,0x64,0x00,},
	{0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0x00,},
	{0x00,0x7E,0x80,0x80,0x80,0x80,0x7E,0x00,},
	{0x02,0x04,0xF8,0x40,0x20,0x10,0x0E,0x00,},
	{0x02,0xFE,0x40,0x30,0x20,0x40,0xFE,0x00,},
	{0x80,0x42,0x22,0x7C,0x88,0x84,0x82,0x00,},
	{0x06,0x08,0x10,0xE0,0x10,0x08,0x06,0x00,},
	{0x00,0x86,0xC2,0xA2,0x92,0x8A,0xC6,0x00,},
	{0x38,0x44,0x92,0xAA,0xBA,0xA2,0x1C,0x00,},
	{0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,},
};
