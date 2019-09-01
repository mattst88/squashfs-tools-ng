/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * compressor.c
 *
 * Copyright (C) 2019 David Oberhollenzer <goliath@infraroot.at>
 */
#include "config.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "internal.h"
#include "util.h"

typedef compressor_t *(*compressor_fun_t)(const compressor_config_t *cfg);

static compressor_fun_t compressors[SQFS_COMP_MAX + 1] = {
#ifdef WITH_GZIP
	[SQFS_COMP_GZIP] = create_gzip_compressor,
#endif
#ifdef WITH_XZ
	[SQFS_COMP_XZ] = create_xz_compressor,
#endif
#ifdef WITH_LZO
	[SQFS_COMP_LZO] = create_lzo_compressor,
#endif
#ifdef WITH_LZ4
	[SQFS_COMP_LZ4] = create_lz4_compressor,
#endif
#ifdef WITH_ZSTD
	[SQFS_COMP_ZSTD] = create_zstd_compressor,
#endif
};

static const char *names[] = {
	[SQFS_COMP_GZIP] = "gzip",
	[SQFS_COMP_LZMA] = "lzma",
	[SQFS_COMP_LZO] = "lzo",
	[SQFS_COMP_XZ] = "xz",
	[SQFS_COMP_LZ4] = "lz4",
	[SQFS_COMP_ZSTD] = "zstd",
};

int generic_write_options(int fd, const void *data, size_t size)
{
	uint8_t buffer[size + 2];

	*((uint16_t *)buffer) = htole16(0x8000 | size);
	memcpy(buffer + 2, data, size);

	if (write_data("writing compressor options",
		       fd, buffer, sizeof(buffer))) {
		return -1;
	}

	return sizeof(buffer);
}

int generic_read_options(int fd, void *data, size_t size)
{
	uint8_t buffer[size + 2];

	if (read_data_at("reading compressor options", sizeof(sqfs_super_t),
			 fd, buffer, sizeof(buffer))) {
		return -1;
	}

	if (le16toh(*((uint16_t *)buffer)) != (0x8000 | size)) {
		fputs("reading compressor options: invalid meta data header\n",
		      stderr);
		return -1;
	}

	memcpy(data, buffer + 2, size);
	return 0;
}

bool compressor_exists(E_SQFS_COMPRESSOR id)
{
	if (id < SQFS_COMP_MIN || id > SQFS_COMP_MAX)
		return false;

	return (compressors[id] != NULL);
}

compressor_t *compressor_create(const compressor_config_t *cfg)
{
	if (cfg == NULL || cfg->id < SQFS_COMP_MIN || cfg->id > SQFS_COMP_MAX)
		return NULL;

	if (compressors[cfg->id] == NULL)
		return NULL;

	return compressors[cfg->id](cfg);
}

const char *compressor_name_from_id(E_SQFS_COMPRESSOR id)
{
	if (id < 0 || (size_t)id >= sizeof(names) / sizeof(names[0]))
		return NULL;

	return names[id];
}

int compressor_id_from_name(const char *name, E_SQFS_COMPRESSOR *out)
{
	size_t i;

	for (i = 0; i < sizeof(names) / sizeof(names[0]); ++i) {
		if (names[i] != NULL && strcmp(names[i], name) == 0) {
			*out = i;
			return 0;
		}
	}

	return -1;
}

int compressor_config_init(compressor_config_t *cfg, E_SQFS_COMPRESSOR id,
			   size_t block_size, uint16_t flags)
{
	memset(cfg, 0, sizeof(*cfg));

	cfg->id = id;
	cfg->flags = flags;
	cfg->block_size = block_size;

	switch (id) {
	case SQFS_COMP_GZIP:
		cfg->opt.gzip.level = SQFS_GZIP_DEFAULT_LEVEL;
		cfg->opt.gzip.window_size = SQFS_GZIP_DEFAULT_WINDOW;
		break;
	case SQFS_COMP_LZO:
		cfg->opt.lzo.algorithm = SQFS_LZO_DEFAULT_ALG;
		cfg->opt.lzo.level = SQFS_LZO_DEFAULT_LEVEL;
		break;
	case SQFS_COMP_ZSTD:
		cfg->opt.zstd.level = SQFS_ZSTD_DEFAULT_LEVEL;
		break;
	case SQFS_COMP_XZ:
		cfg->opt.xz.dict_size = block_size;
		break;
	default:
		break;
	}

	return 0;
}