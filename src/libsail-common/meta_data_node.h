/*  This file is part of SAIL (https://github.com/smoked-herring/sail)

    Copyright (c) 2020 Dmitry Baryshev

    The MIT License

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef SAIL_META_DATA_NODE_H
#define SAIL_META_DATA_NODE_H

#ifdef SAIL_BUILD
    #include "common.h"
    #include "error.h"
    #include "export.h"
#else
    #include <sail-common/common.h>
    #include <sail-common/error.h>
    #include <sail-common/export.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * A tructure representing meta data like a JPEG comment or a binary EXIF profile.
 *
 * For example:
 *
 * {
 *     key               = SAIL_META_DATA_UNKNOWN,
 *     key_unknown       = "My Data",
 *     value_type        = SAIL_META_DATA_TYPE_STRING,
 *     value_string      = "Data",
 *     value_data        = NULL,
 *     value_data_length = 0
 * }
 *
 * {
 *     key               = SAIL_META_DATA_COMMENT,
 *     key_unknown       = NULL,
 *     value_type        = SAIL_META_DATA_TYPE_STRING,
 *     value_string      = "Holidays",
 *     value_data        = NULL,
 *     value_data_length = 0
 * }
 *
 * {
 *     key               = SAIL_META_DATA_EXIF,
 *     key_unknown       = NULL,
 *     value_type        = SAIL_META_DATA_TYPE_DATA,
 *     value_string      = NULL,
 *     value_data        = <binary data>,
 *     value_data_length = 2240
 * }
 *
 * Not every image codec supports key-values. For example:
 *
 *   - JPEG doesn't support keys. When you try to save key-value meta data pairs,
 *     only values are saved.
 *   - TIFF supports only a subset of known meta data keys (Artist, Make, Model etc.).
 *     It doesn't support unknown keys (SAIL_META_DATA_UNKNOWN). This is why TIFF never
 *     output SAIL_META_DATA_UNKNOWN keys.
 *   - PNG supports both keys and values
 *
 * When writing images, SAIL codecs don't necessarily use sail_meta_data_to_string() to convert
 * keys to string representations. PNG, for example, uses hardcoded "Raw profile type exif" key name
 * for raw Hex-encoded EXIF tags.
 */
struct sail_meta_data_node {

    /*
     * If key is SAIL_META_DATA_UNKNOWN, key_unknown contains an actual string key.
     * If key is not SAIL_META_DATA_UNKNOWN, key_unknown is NULL.
     */
    enum SailMetaData key;
    char *key_unknown;

    /* Value type. */
    enum SailMetaDataType value_type;

    /*
     * Actual meta data string value when value_type is SAIL_META_DATA_TYPE_STRING. NULL otherwise.
     */
    char *value_string;

    /*
     * Actual meta data binary value when value_type is SAIL_META_DATA_TYPE_DATA.
     * NULL otherwise. value_data_length holds its length.
     */
    void *value_data;

    /*
     * The length of the binary value or 0.
     */
    unsigned value_data_length;

    struct sail_meta_data_node *next;
};

/*
 * Allocates a new meta entry node. The assigned node MUST be destroyed later with sail_destroy_meta_data_node().
 * Use sail_alloc_meta_data_node_from_string() and sail_alloc_meta_data_node_from_data() to allocate meta data
 * nodes from actual data.
 *
 * Returns SAIL_OK on success.
 */
SAIL_EXPORT sail_status_t sail_alloc_meta_data_node(struct sail_meta_data_node **node);

/*
 * Allocates a new meta entry node from the specified string. The value of 'value_length' is set to strlen(value) + 1.
 * The key must not be SAIL_META_DATA_UNKNOWN. This is the key of this method. The assigned node MUST be destroyed
 * later with sail_destroy_meta_data_node().
 *
 * Returns SAIL_OK on success.
 */
SAIL_EXPORT sail_status_t sail_alloc_meta_data_node_from_known_string(enum SailMetaData key,
                                                                        const char *value,
                                                                        struct sail_meta_data_node **node);

/*
 * Allocates a new meta entry node from the specified string. The value of 'value_length' is set to strlen(value) + 1.
 * Sets the key to SAIL_META_DATA_UNKNOWN. This is the key of this method.
 * The assigned node MUST be destroyed later with sail_destroy_meta_data_node().
 *
 * Returns SAIL_OK on success.
 */
SAIL_EXPORT sail_status_t sail_alloc_meta_data_node_from_unknown_string(const char *key_unknown,
                                                                        const char *value,
                                                                        struct sail_meta_data_node **node);
/*
 * Allocates a new meta entry node from the specified data. The key must not be SAIL_META_DATA_UNKNOWN.
 * This is the key of this method. The assigned node MUST be destroyed later with sail_destroy_meta_data_node().
 *
 * Returns SAIL_OK on success.
 */
SAIL_EXPORT sail_status_t sail_alloc_meta_data_node_from_known_data(enum SailMetaData key,
                                                                    const void *value,
                                                                    unsigned value_length,
                                                                    struct sail_meta_data_node **node);

/*
 * Allocates a new meta entry node from the specified data. Sets the key to SAIL_META_DATA_UNKNOWN.
 * The assigned node MUST be destroyed later with sail_destroy_meta_data_node().
 *
 * Returns SAIL_OK on success.
 */
SAIL_EXPORT sail_status_t sail_alloc_meta_data_node_from_unknown_data(const char *key_unknown,
                                                                        const void *value,
                                                                        unsigned value_length,
                                                                        struct sail_meta_data_node **node);

/*
 * Destroys the specified meta entry node and all its internal allocated memory buffers.
 */
SAIL_EXPORT void sail_destroy_meta_data_node(struct sail_meta_data_node *node);

/*
 * Makes a deep copy of the specified meta entry node. The assigned node MUST be destroyed
 * later with sail_destroy_meta_data_node().
 *
 * Returns SAIL_OK on success.
 */
SAIL_EXPORT sail_status_t sail_copy_meta_data_node(struct sail_meta_data_node *source,
                                                   struct sail_meta_data_node **target);

/*
 * Destroys the specified meta entry node and all its internal allocated memory buffers.
 * Repeats the destruction procedure recursively for the stored next pointer.
 */
SAIL_EXPORT void sail_destroy_meta_data_node_chain(struct sail_meta_data_node *node);

/*
 * Makes a deep copy of the specified meta entry node chain. The assigned chain MUST be destroyed
 * later with sail_destroy_meta_data_node_chain(). If the source chain is NULL, it assigns NULL
 * to the target chain and returns 0.
 *
 * Returns SAIL_OK on success.
 */
SAIL_EXPORT sail_status_t sail_copy_meta_data_node_chain(struct sail_meta_data_node *source,
                                                         struct sail_meta_data_node **target);

/* extern "C" */
#ifdef __cplusplus
}
#endif

#endif
