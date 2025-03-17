#include "Image.hpp"

#include <stdlib.h>

errno_t image_register_handler(ImageHandler* handler)
{

}

errno_t image_get_handler_count(ImageHandler* handler, size_t* count)
{

}

errno_t image_get_handler_by_index(ImageHandler* handler, size_t index)
{

}
errno_t image_get_handler_by_format(ImageHandler* handler, const char* ext)
{

}

errno_t image_find_handler_by_filename(ImageHandler* handler, const char* filename)
{

}


errno_t image_new(Image* image)
{
	image = (Image*)malloc(sizeof *image);
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
	free(image->pixels);
	free(image->filename);

	free(image);

	return 0;
}

errno_t image_load_file(Image* image)
{
	if (!image)
		return -1;

	size_t count;
	if(!image_get_handler_count(&count) || !count)
		return -1;

	FILE* fp = fopen(image->filename, "rb");
	if (!fp)
		return -1;

	ImageHandler handler;
	if (!image_find_handler_by_filename(&handler, image->filename))
		return -1;

	if (!handler.image_ops.read_file(image, fp))
		return -1;

	fclose(fp);
	return 0;
}
errno_t image_save_file(Image* image)
{
	if (!image)
		return -1;

	size_t count;
	if(!image_get_handler_count(&count) || !count)
		return -1;

	FILE* fp = fopen(image->filename, "wb");
	if (!fp)
		return -1;

	ImageHandler handler;
	if (!image_find_handler_by_filename(&handler, image->filename))
		return -1;

	if (!handler.image_ops.write_file(image, fp))
		return -1;

	fclose(fp);
	return 0;
}

errno_t image_load_memory(Image* image, MemoryStream* stream)
{
	if (!image || !stream)
		return -1;

	size_t count;
	if(!image_get_handler_count(&count) || !count)
		return -1;

	ImageHandler handler;
	if (!image_find_handler_by_filename(&handler, image->filename))
		return -1;

	if (!handler.image_ops.read_memory_stream(image, stream))
		return -1;

	return 0;
}
errno_t image_save_memory(Image* image, MemoryStream* stream)
{
	if (!image || !stream)
		return -1;

	size_t count;
	if(!image_get_handler_count(&count) || !count)
		return -1;

	ImageHandler handler;
	if (!image_find_handler_by_filename(&handler, image->filename))
		return -1;

	if (!handler.image_ops.write_memory_stream(image, stream))
		return -1;

	return 0;
}

errno_t image_get_pixel_size(Image* image, size_t* size)
{
	if (!image)
		return -1;

	switch(image->pixel_fmt)
	{
		case Image::FMT_GREY8:
			return 1;
		case Image::FMT_RGB24:
			return 3;
		case Image::FMT_RGBA32:
			return 4;
		default:
			break;
	}

	return 0;
}

errno_t image_set_pixel(Image* image, int x, int y, void* pixel)
{

}
errno_t image_get_pixel(Image* image, int x, int y, void* pixel)
{

}

errno_t image_set_pixel1i(Image* image, int x, int y, int pixel)
{

}
errno_t image_set_pixel1f(Image* image, int x, int y, float pixel)
{

}
errno_t image_set_pixel3i(Image* image, int x, int y, int r, int g, int b)
{

}
errno_t image_set_pixel3f(Image* image, int x, int y, float r, float g, float b)
{

}
errno_t image_set_pixel4i(Image* image, int x, int y, int r, int g, int b, int a)
{

}
errno_t image_set_pixel4f(Image* image, int x, int y, float r, float g, float b, float a)
{

}

errno_t image_get_pixel1i(Image* image, int x, int y, int *pix)
{

}
errno_t image_get_pixel1f(Image* image, int x, int y, float *pix)
{

}
errno_t image_get_pixel3i(Image* image, int x, int y, int *r, int *g, int *b)
{

}
errno_t image_get_pixel3f(Image* image, int x, int y, float *r, float *g, float *b)
{

}
errno_t image_get_pixel4i(Image* image, int x, int y, int *r, int *g, int *b, int *a)
{

}
errno_t image_get_pixel4f(Image* image, int x, int y, float *r, float *g, float *b, float *a)
{

}