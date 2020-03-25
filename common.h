#ifndef SAIL_COMMON_H
#define SAIL_COMMON_H

#include <limits.h>
#include <stdbool.h>

/*
 * Common data structures and functions used across SAIL, both in libsail and in image plugins.
 */

/* Pixel format */
enum SailPixelFormat {

    /* Unsupported pixel format that cannot be read or written. */
    SAIL_PIXEL_FORMAT_UNSUPPORTED,

    /* 
     * Don't manipulate the output image data. Copy it as is from the source file.
     * The caller will handle the returned pixel data manually.
     */
    SAIL_PIXEL_FORMAT_SOURCE,

    SAIL_PIXEL_FORMAT_MONO_BE,
    SAIL_PIXEL_FORMAT_MONO_LE,

    SAIL_PIXEL_FORMAT_GRAYSCALE8,
    SAIL_PIXEL_FORMAT_GRAYSCALE16,
    SAIL_PIXEL_FORMAT_INDEXED8,

    SAIL_PIXEL_FORMAT_BGR30,
    SAIL_PIXEL_FORMAT_RGB16,
    SAIL_PIXEL_FORMAT_RGB30,
    SAIL_PIXEL_FORMAT_RGB32,
    SAIL_PIXEL_FORMAT_RGB444,
    SAIL_PIXEL_FORMAT_RGB555,
    SAIL_PIXEL_FORMAT_RGB666,
    SAIL_PIXEL_FORMAT_RGB888,
    SAIL_PIXEL_FORMAT_RGBA8888,
    SAIL_PIXEL_FORMAT_ARGB8888,

    /* Not to be used. Resize the enum for future elements. */
    SAIL_PIXEL_FORMAT_RESIZE_ENUM_TO_INT = MAX_INT
};

/* Image properties. */
enum SailImageProperties {
    SAIL_IMAGE_FLIPPED_VERTICALLY   = 1 << 0,

    /* Not to be used. Resize the enum for future elements. */
    SAIL_IMAGE_PROPERTY_RESIZE_ENUM_TO_INT = MAX_INT
};

/*
 * A structure representing a file object.
 */
struct sail_file
{
    /* File descriptor */
    int fd;

    /* Plugin-specific data */
    void *pimpl;

    /* Plugin-specific data deleter */
    void (*pimpl_destroy)(void *);
};

/*
 * A structure representing an image.
 */
struct sail_image
{
    /* Image width */
    int width;

    /* Image height */
    int height;

    /* Image pixel format. See SailPixelFormat. */
    int pixel_format;

    /* Number of passes in this image */
    int passes;

    /* Is the image an animation */
    bool animated;

    /* Delay in milliseconds if the image is a frame in an animation */
    int delay;

    /* Palette pixel format */
    int palette_pixel_format;

    /* Palette if a "source" pixel format was chosen and the image has a palette */
    void *palette;

    /* Size of the palette data in bytes */
    int palette_size;
};

/*
 * File functions.
 */

/*
 * Opens the specified image file using the specified file flags. The assigned file MUST be closed later
 * with sail_file_close().
 *
 * Returns 0 on success or errno on error.
 */
int sail_file_open(const char *filepath, int flags, sail_file **file);

/*
 * Closes the specified file. Does nothing if the file is already closed.
 * The "file" pointer MUST NOT be used after calling this function.
 */
void sail_file_close(sail_file *file);

/*
 * Image functions.
 */

/*
 * Destroys the specified image and all its internal allocated memory buffers.
 * The "image" pointer MUST NOT be used after calling this function.
 */
void sail_image_destroy(sail_image *image);

#endif
