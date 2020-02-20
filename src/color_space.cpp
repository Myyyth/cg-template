#include "color_space.h"


ColorSpace::ColorSpace(unsigned short width, unsigned short height): BlackImage(width, height)
{
	frame_buffer.reserve(width * height);
}

ColorSpace::~ColorSpace()
{
	frame_buffer.clear();
}

void ColorSpace::DrawScene()
{
	frame_buffer.resize(width * height);
	color* clr = new color(255, 0, 0);
	for (unsigned short col = 0; col < width; col++) 
	{
		for (unsigned short row = 0; row < height; row++) 
		{
			SetPixel(col, row, color(
				static_cast<unsigned short>((row * 255) / height),
				static_cast<unsigned short>((col * 255) / width),
				0));
		}
	}
}

void ColorSpace::SetPixel(unsigned short x, unsigned short y, color color)
{
	frame_buffer[width * y + x] = color;
}

