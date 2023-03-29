/*
 * This file is part of vitaGL
 * Copyright 2017, 2018, 2019, 2020 Rinnegatamante
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * gpu_utils.h:
 * Header file for the GPU utilities exposed by gpu_utils.c
 */

#ifndef _GPU_UTILS_H_
#define _GPU_UTILS_H_

#include "mem_utils.h"

// Align a value to the requested alignment
#define ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))
#define ALIGNBLOCK(x, a) (((x) + ((a)-1)) / a)

// Texture object status enum
enum {
	TEX_UNUSED,
	TEX_UNINITIALIZED,
	TEX_VALID
};

// Texture object struct
typedef struct {
#ifndef TEXTURES_SPEEDHACK
	GLboolean used;
#endif
	uint8_t status;
	uint8_t mip_count;
	uint8_t ref_counter;
	uint8_t faces_counter;
	GLboolean use_mips;
	GLboolean dirty;
	GLboolean overridden;
	SceGxmTexture gxm_tex;
	void *data;
	void *palette_data;
	uint32_t type;
	void (*write_cb)(void *, uint32_t);
	SceGxmTextureFilter min_filter;
	SceGxmTextureFilter mag_filter;
	SceGxmTextureAddrMode u_mode;
	SceGxmTextureAddrMode v_mode;
	SceGxmTextureMipFilter mip_filter;
	uint32_t lod_bias;
#ifdef HAVE_UNPURE_TEXTURES
	int8_t mip_start;
#endif
} texture;

static inline __attribute__((always_inline)) void gpu_store_texture_data(uint32_t orig_w, uint32_t w, uint32_t h, uint32_t src_stride, const void *src_data, void *dst_data, uint8_t src_bpp, uint8_t bpp, uint32_t (*read_cb)(void *), void (*write_cb)(void *, uint32_t), GLboolean fast_store, GLint xoffset) {
	uint32_t dst_stride = ALIGN(orig_w, 8) * bpp;
	uint8_t *src;
	uint8_t *dst;
	int i, j;
	if (fast_store) { // Internal Format and Data Format are the same, we can just use vgl_fast_memcpy for better performance
		if (xoffset == 0 && src_stride == dst_stride && src_stride == orig_w * bpp) // Texture size is already aligned, we can use a single vgl_fast_memcpy for better performance
			sceClibMemcpy(dst_data, src_data, w * h * bpp);
		else {
			for (i = 0; i < h; i++) {
				src = (uint8_t *)src_data + src_stride * i;
				dst = (uint8_t *)dst_data + dst_stride * i;
				sceClibMemcpy(dst, src, w * bpp);
			}
		}
	} else { // Different internal and data formats, we need to go with slower callbacks system
		for (i = 0; i < h; i++) {
			src = (uint8_t *)src_data + src_stride * i;
			dst = (uint8_t *)dst_data + dst_stride * i;
			for (j = 0; j < w; j++) {
				uint32_t clr = read_cb(src);
				write_cb(dst, clr);
				src += src_bpp;
				dst += bpp;
			}
		}
	}
}

// Alloc a generic memblock into sceGxm mapped memory
void *gpu_alloc_mapped(size_t size, vglMemType type);

// Alloc a generic memblock into sceGxm mapped memory and marks it for garbage collection
void *gpu_alloc_mapped_temp(size_t size);

// Alloc a generic memblock into sceGxm mapped memory with a given alignment
void *gpu_alloc_mapped_aligned(size_t alignment, size_t size, vglMemType type);

// Alloc into sceGxm mapped memory a vertex USSE memblock
void *gpu_vertex_usse_alloc_mapped(size_t size, unsigned int *usse_offset);

// Dealloc from sceGxm mapped memory a vertex USSE memblock
void gpu_vertex_usse_free_mapped(void *addr);

// Alloc into sceGxm mapped memory a fragment USSE memblock
void *gpu_fragment_usse_alloc_mapped(size_t size, unsigned int *usse_offset);

// Dealloc from sceGxm mapped memory a fragment USSE memblock
void gpu_fragment_usse_free_mapped(void *addr);

// Calculate bpp for a requested texture format
int tex_format_to_bytespp(SceGxmTextureFormat format);

// Alloc a texture
void gpu_alloc_texture(uint32_t w, uint32_t h, SceGxmTextureFormat format, const void *data, texture *tex, uint8_t src_bpp, uint32_t (*read_cb)(void *), void (*write_cb)(void *, uint32_t), GLboolean fast_store);

// Alloc a cube texture
void gpu_alloc_cube_texture(uint32_t w, uint32_t h, SceGxmTextureFormat format, SceGxmTransferFormat src_format, const void *data, texture *tex, uint8_t src_bpp, int index);

// Alloc a compresseed texture
void gpu_alloc_compressed_texture(int32_t level, uint32_t w, uint32_t h, SceGxmTextureFormat format, uint32_t image_size, const void *data, texture *tex, uint8_t src_bpp, uint32_t (*read_cb)(void *));

// Alloc a compressed cube texture
void gpu_alloc_compressed_cube_texture(uint32_t w, uint32_t h, SceGxmTextureFormat format, uint32_t image_size, const void *data, texture *tex, uint8_t src_bpp, uint32_t (*read_cb)(void *), int index);

// Alloc a paletted texture
void gpu_alloc_paletted_texture(int32_t level, uint32_t w, uint32_t h, SceGxmTextureFormat format, const void *data, texture *tex, uint8_t src_bpp, uint32_t (*read_cb)(void *));

// Dealloc a texture data
void gpu_free_texture_data(texture *tex);

// Dealloc a texture
void gpu_free_texture(texture *tex);

// Alloc a palette
void *gpu_alloc_palette(const void *data, uint32_t w, uint32_t bpe);

// Dealloc a palette
void gpu_free_palette(void *pal);

// Generate mipmaps for a given texture
void gpu_alloc_mipmaps(int level, texture *tex);

#endif
