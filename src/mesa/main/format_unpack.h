/*
 * Mesa 3-D graphics library
 *
 * Copyright (c) 2011 VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef FORMAT_UNPACK_H
#define FORMAT_UNPACK_H

#include "formats.h"

extern void
_mesa_unpack_rgba_row(mesa_format format, uint32_t n,
                      const void *src, float dst[][4]);

extern void
_mesa_unpack_ubyte_rgba_row(mesa_format format, uint32_t n,
                            const void *src, uint8_t dst[][4]);

void
_mesa_unpack_uint_rgba_row(mesa_format format, uint32_t n,
                           const void *src, uint32_t dst[][4]);

extern void
_mesa_unpack_rgba_block(mesa_format format,
                        const void *src, int32_t srcRowStride,
                        float dst[][4], int32_t dstRowStride,
                        uint32_t x, uint32_t y, uint32_t width, uint32_t height);

extern void
_mesa_unpack_float_z_row(mesa_format format, uint32_t n,
                         const void *src, float *dst);


void
_mesa_unpack_uint_z_row(mesa_format format, uint32_t n,
                        const void *src, uint32_t *dst);

void
_mesa_unpack_ubyte_stencil_row(mesa_format format, uint32_t n,
                               const void *src, uint8_t *dst);

void
_mesa_unpack_uint_24_8_depth_stencil_row(mesa_format format, uint32_t n,
                                         const void *src, uint32_t *dst);

void
_mesa_unpack_float_32_uint_24_8_depth_stencil_row(mesa_format format,
                                                  uint32_t n,
                                                  const void *src,
                                                  uint32_t *dst);

#endif /* FORMAT_UNPACK_H */
