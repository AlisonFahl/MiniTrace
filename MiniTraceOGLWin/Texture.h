#pragma once

#include "Material.h"

class Texture
{
public:
	Texture(int width, int height, unsigned char* data);
	~Texture();

private:
	int m_Width;
	int m_Height;

	float *m_Data;
	int m_DataSize;

public:
	static const int RGB_SIZE = 3;

	static Texture* LoadTexImage(const wchar_t* fileName, int image_width, int image_height);

	inline int GetWidth()
	{
		return m_Width;
	}

	inline int GetHeight()
	{
		return m_Height;
	}

	inline Colour GetPixel(int x, int y)
	{
		Colour pixel;
		int first_pixel = (((m_Height -  y) * m_Width) + x) * Texture::RGB_SIZE;
		pixel.red = m_Data[first_pixel];
		pixel.green = m_Data[first_pixel + 1];
		pixel.blue = m_Data[first_pixel + 2];
		return pixel;
	}
};

