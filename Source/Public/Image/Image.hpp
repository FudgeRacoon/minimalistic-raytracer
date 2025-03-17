#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdio.h>
#include <stdint.h>

struct Image;
struct MemoryStream;

using ReadFileCallback = errno_t(*)(Image*, FILE*);
using WriteFileCallback = errno_t(*)(Image*, FILE*);
using ReadMemoryStreamCallback = errno_t(*)(Image*, MemoryStream*);
using WriteMemoryStreamFileCallback = errno_t(*)(Image*, MemoryStream*);

struct ImageHandler
{
	struct {
		ReadFileCallback read_file;
		WriteFileCallback write_file;
		ReadMemoryStreamCallback read_memory_stream;
		WriteMemoryStreamFileCallback write_memory_stream;
	} image_ops;

	char* extensions;
};

struct ImageHandlerList
{
	ImageHandler* value;
	ImageHandlerList* next;
};

struct Image
{
	enum Format {
		FMT_GREY8,
		FMT_RGB24,
		FMT_RGBA32,

		FMT_NULL
	} pixel_fmt;

	void* pixels;

	char* filename;

	uint8_t pixel_size;
	uint32_t width, height;

	static ImageHandlerList s_handlers;
};

#ifdef __cplusplus
extern "C" {
#endif

errno_t image_register_handler(ImageHandler* handler);

errno_t image_get_handler_count(size_t* count);

errno_t image_get_handler_by_index(ImageHandler* handler, size_t index);
errno_t image_get_handler_by_format(ImageHandler* handler, const char* ext);

errno_t image_find_handler_by_filename(ImageHandler* handler, const char* filename);

errno_t image_new(Image* image);
errno_t image_free(Image* image);

errno_t image_load_file(Image* image);
errno_t image_save_file(Image* image);

errno_t image_load_stream(Image* image, MemoryStream* stream);
errno_t image_save_stream(Image* image, MemoryStream* stream);

errno_t image_get_pixel_size(Image* image, size_t* size);

errno_t image_set_pixel(Image* image, int x, int y, errno_t* pixel);
errno_t image_get_pixel(Image* image, int x, int y, errno_t* pixel);

errno_t image_set_pixel1i(Image* image, int x, int y, int pixel);
errno_t image_set_pixel1f(Image* image, int x, int y, float pixel);
errno_t image_set_pixel3i(Image* image, int x, int y, int r, int g, int b);
errno_t image_set_pixel3f(Image* image, int x, int y, float r, float g, float b);
errno_t image_set_pixel4i(Image* image, int x, int y, int r, int g, int b, int a);
errno_t image_set_pixel4f(Image* image, int x, int y, float r, float g, float b, float a);

errno_t image_get_pixel1i(Image* image, int x, int y, int *pix);
errno_t image_get_pixel1f(Image* image, int x, int y, float *pix);
errno_t image_get_pixel3i(Image* image, int x, int y, int *r, int *g, int *b);
errno_t image_get_pixel3f(Image* image, int x, int y, float *r, float *g, float *b);
errno_t image_get_pixel4i(Image* image, int x, int y, int *r, int *g, int *b, int *a);
errno_t image_get_pixel4f(Image* image, int x, int y, float *r, float *g, float *b, float *a);

#ifdef __cplusplus
}
#endif

#endif