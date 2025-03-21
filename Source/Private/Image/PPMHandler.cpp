#include "Image/PPMHandler.hpp"

#include <stdio.h>
#include <stdlib.h>

static errno_t _read_file_line(FILE* fp, uint8_t* buffer, size_t size)
{
	char c;
	uint8_t* buffer_ptr = buffer;

	while (size && (c = fgetc(fp)) != EOF)
	{
		*buffer_ptr = c;

		if (*buffer_ptr == '\n')
			break;

		size--;
		buffer_ptr++;
	}

	return buffer == buffer_ptr ? -1 : 0;
}

static errno_t _read_file(Image* image)
{
	FILE* fp = fopen(image->filename, "rb");
	if (!fp)
		return -1;

	uint8_t header_buffer[256];
	uint32_t width, height, max_pixel_value, current_hdr_line = 1;

	if (!_read_file_line(fp, header_buffer, sizeof(header_buffer)))
		return -1;

	if (!(header_buffer[0] == 'P' && (header_buffer[1] == '3' || header_buffer[1] == '6')))
		return -1;

	while (current_hdr_line < 3 && _read_file_line(fp, header_buffer, sizeof(header_buffer)))
	{
		if (header_buffer[0] == '#')
			continue;

		switch (current_hdr_line)
		{
		case 1:
			if (!sscanf((char*)header_buffer, "%d %d\n", &width, &height))
				return -1;
			break;
		case 2:
			if (!sscanf((char*)header_buffer, "%d\n", &max_pixel_value))
				return -1;
			break;
		default:
			break;
		}

		current_hdr_line++;
	}

	if (width < 1 || height < 1 || max_pixel_value > 65535)
		return -1;

	size_t bytes;
	if (max_pixel_value == 255) bytes = 3;
	else if (max_pixel_value == 65535) bytes = 6;
	else return -1;

	if (bytes == 3) image->pixel_fmt = Image::FMT_RGB24;
	else if (bytes == 6) image->pixel_fmt = Image::FMT_RGBF48;
	else return -1;

	size_t framebuffer_size = width * height * bytes;

	fclose(fp);
	return 0;
}
static errno_t _write_file(Image* image)
{
	FILE* fp = fopen(image->filename, "wb");
	if (!fp)
		return -1;


	fclose(fp);
	return 0;
}

static errno_t _read_memory_stream(Image* image, MemoryStream* stream)
{
	return 0;
}
static errno_t _write_memory_stream(Image* image, MemoryStream* stream)
{
	return 0;
}

void ppm_handler_new(ImageHandler* handler)
{
	handler = (ImageHandler*)malloc(sizeof(ImageHandler));

	handler->ops.read_file = &_read_file;
	handler->ops.write_file = &_write_file;

	handler->ops.read_memory_stream = &_read_memory_stream;
	handler->ops.write_memory_stream = &_write_memory_stream;

	handler->exts = "ppm:pgm:pnm";
}