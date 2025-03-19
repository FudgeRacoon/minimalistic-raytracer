#include "Image.hpp"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

errno_t image_register_handler(ImageHandler* handler)
{
	assert(handler);

	if (!Image::s_handler_list)
	{
		Image::s_handler_list = (ImageHandlerNode*)malloc(sizeof(ImageHandlerNode));
		if (!Image::s_handler_list)
			return -1;

		Image::s_handler_list->data = handler;
		Image::s_handler_list->next = nullptr;
	}

	ImageHandlerNode* node = (ImageHandlerNode*)malloc(sizeof(ImageHandlerNode));
	if (node)
		return -1;

	node->data = handler;
	node->next = Image::s_handler_list;

	Image::s_handler_list = node;

	return 0;
}

errno_t image_get_handler_count(size_t* count)
{
	size_t counter = 0;

	auto node = Image::s_handler_list;
	for (; node; node = node->next)
		++counter;

	*count = counter;

	return 0;
}

errno_t image_get_handler_by_index(ImageHandler* handler, size_t index)
{
	assert(handler);

	size_t counter = 0;

	auto node = Image::s_handler_list;
	for (; node || index == counter; node = node->next)
		++counter;

	handler = node->data;

	return 0;
}
errno_t image_get_handler_by_format(ImageHandler* handler, const char* ext)
{
	assert(handler);

	auto node = Image::s_handler_list;
	for (; node || strcmp(node->data->ext, ext) == 0; node = node->next);

	handler = node->data;

	return 0;
}

errno_t image_find_handler_by_filename(ImageHandler* handler, const char* filename)
{
	assert(handler);
}


errno_t image_new(Image* image)
{
	image = (Image*)malloc(sizeof(Image));
	if (!image)
		return -1;

	image->pixel_fmt = Image::FMT_RGBA32;

	image->pixels = nullptr;
	image->filename = nullptr;

	size_t size;
	if (!image_get_pixel_size(image, &size))
		return -1;

	image->pixel_size = size;
	image->width = image->height = 0;

	return 0;
}
errno_t image_free(Image* image)
{
	assert(image);

	free(image->pixels);
	free(image->filename);

	free(image);

	return 0;
}

errno_t image_load_file(Image* image)
{
	assert(image);

	size_t handler_count;
	if(!image_get_handler_count(&handler_count) || !handler_count)
		return -1;

	ImageHandler handler;
	if (!image_find_handler_by_filename(&handler, image->filename))
		return -1;

	FILE* fp = fopen(image->filename, "rb");
	if (!fp)
		return -1;

	if (!handler.ops.read_file(image, fp))
		return -1;

	fclose(fp);
	return 0;
}
errno_t image_save_file(Image* image)
{
	assert(image);

	size_t handler_count;
	if(!image_get_handler_count(&handler_count) || !handler_count)
		return -1;

	ImageHandler handler;
	if (!image_find_handler_by_filename(&handler, image->filename))
		return -1;

	FILE* fp = fopen(image->filename, "wb");
	if (!fp)
		return -1;

	if (!handler.ops.write_file(image, fp))
		return -1;

	fclose(fp);
	return 0;
}

errno_t image_load_memory(Image* image, MemoryStream* stream)
{
	assert(image && stream);

	size_t handler_count;
	if(!image_get_handler_count(&handler_count) || !handler_count)
		return -1;

	ImageHandler handler;
	if (!image_find_handler_by_filename(&handler, image->filename))
		return -1;

	if (!handler.ops.read_memory_stream(image, stream))
		return -1;

	return 0;
}
errno_t image_save_memory(Image* image, MemoryStream* stream)
{
	assert(image && stream);

	size_t handler_count;
	if(!image_get_handler_count(&handler_count) || !handler_count)
		return -1;

	ImageHandler handler;
	if (!image_find_handler_by_filename(&handler, image->filename))
		return -1;

	if (!handler.ops.write_memory_stream(image, stream))
		return -1;

	return 0;
}

errno_t image_get_pixel_size(Image* image, size_t* size)
{
	assert(image);

	switch(image->pixel_fmt)
	{
		case Image::FMT_GREY8:  *size = 1;
		case Image::FMT_RGB24:  *size = 3;
		case Image::FMT_RGBA32: *size = 4;

		default: break;
	}

	return 0;
}

errno_t image_set_pixel(Image* image, uint32_t x, uint32_t y, void* pixel)
{
	assert(image);

	if (x >= image->width || y >= image->height)
		return -1;

	uint8_t* dst = (uint8_t*)image->pixels + ((x + y * image->width) * image->pixel_size);
	memcpy(dst, pixel, image->pixel_size);

	return 0;
}
errno_t image_get_pixel(Image* image, uint32_t x, uint32_t y, void* pixel)
{
	assert(image);

	if (x >= image->width || y >= image->height)
		return -1;

	uint8_t* src = (uint8_t*)image->pixels + ((x + y * image->width) * image->pixel_size);
	memcpy(pixel, src, image->pixel_size);

	return 0;
}

errno_t image_set_pixel1i(Image* image, uint32_t x, uint32_t y, int pixel)
{
	assert(image);

	return image_set_pixel(image, x, y, &pixel);
}
errno_t image_get_pixel1i(Image* image, uint32_t x, uint32_t y, int* pixel)
{
	assert(image);

	return image_get_pixel(image, x, y, pixel);
}

errno_t image_set_pixel3i(Image* image, uint32_t x, uint32_t y, int r, int g, int b)
{
	assert(image);

	uint8_t pixels[] = { r, g, b };
	return image_set_pixel(image, x, y, pixels);
}
errno_t image_get_pixel3i(Image* image, uint32_t x, uint32_t y, int* r, int* g, int* b)
{
	assert(image);

	uint8_t pixels[3];
	if (!image_get_pixel(image, x, y, pixels))
		return -1;

	*r = pixels[0];
	*g = pixels[1];
	*b = pixels[2];

	return 0;
}

errno_t image_set_pixel4i(Image* image, uint32_t x, uint32_t y, int r, int g, int b, int a)
{
	assert(image);

	uint8_t pixels[] = { r, g, b, a };
	return image_set_pixel(image, x, y, pixels);
}
errno_t image_get_pixel4i(Image* image, uint32_t x, uint32_t y, int *r, int *g, int *b, int *a)
{
	assert(image);

	uint8_t pixels[4];
	if (!image_get_pixel(image, x, y, pixels))
		return -1;

	*r = pixels[0];
	*g = pixels[1];
	*b = pixels[2];
	*a = pixels[4];

	return 0;
}