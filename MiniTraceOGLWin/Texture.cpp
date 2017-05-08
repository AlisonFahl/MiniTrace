#include "Texture.h"
#include <utility>
#include <windows.h>


Texture::Texture(int width, int height, unsigned char* data)
{
	m_Width = width;
	m_Height = height;

	m_DataSize = width * height * RGB_SIZE;
	m_Data = new float[m_DataSize];
	for (int i = 0; i < m_DataSize; i++)
	{
		m_Data[i] = data[i] / 255.0f;
	}
}


Texture::~Texture()
{
	delete m_Data;
}

Texture* Texture::LoadTexImage(const wchar_t* fileName, int image_width, int image_height)
{
	auto fileHandler = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	auto error = GetLastError();
	auto fileSize = GetFileSize(fileHandler, NULL);
	error = GetLastError();
	unsigned char* data = new unsigned char[fileSize];
	ReadFile(fileHandler, data, fileSize, NULL, NULL);
	int width = (unsigned char)(*data);
	unsigned char rola[] = {255, 0, 0, 255, 0, 0, 0, 255, 0, 0, 255, 0};
	return new Texture(image_width, image_height, data);
}
