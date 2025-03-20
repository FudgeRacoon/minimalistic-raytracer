#include "Image/PPMHandler.hpp"

#include <stdio.h>
#include <stdlib.h>

static errno_t read_file(Image* image)
{
	FILE* fp = fopen(image->filename, "rb");
	if (!fp)
		return -1;


	fclose(fp);
}
static errno_t write_file(Image* image)
{
	FILE* fp = fopen(image->filename, "wb");
	if (!fp)
		return -1;


	fclose(fp);
}
static errno_t read_memory_stream(Image* image, MemoryStream* stream)
{

}
static errno_t write_memory_stream(Image* image, MemoryStream* stream)
{

}

void ppm_handler_new(ImageHandler* handler)
{
	ImageHandler* handler = (ImageHandler*)malloc(sizeof(ImageHandler));

	handler->ops.read_file = &read_file;
	handler->ops.write_file = &write_file;
	handler->ops.read_memory_stream = &read_memory_stream;
	handler->ops.write_memory_stream = &write_memory_stream;

	handler->exts = "ppm:pgm:pnm";
}