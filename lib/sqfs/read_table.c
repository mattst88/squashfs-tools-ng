/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * read_table.c
 *
 * Copyright (C) 2019 David Oberhollenzer <goliath@infraroot.at>
 */
#define SQFS_BUILDING_DLL
#include "config.h"

#include "sqfs/meta_reader.h"
#include "sqfs/error.h"
#include "sqfs/table.h"
#include "sqfs/data.h"
#include "sqfs/io.h"
#include "util.h"

#include <endian.h>
#include <stdlib.h>

int sqfs_read_table(sqfs_file_t *file, sqfs_compressor_t *cmp,
		    size_t table_size, uint64_t location, uint64_t lower_limit,
		    uint64_t upper_limit, void **out)
{
	size_t diff, block_count, blk_idx = 0;
	uint64_t start, *locations;
	sqfs_meta_reader_t *m;
	void *data, *ptr;
	int err;

	data = malloc(table_size);
	if (data == NULL)
		return SQFS_ERROR_ALLOC;

	/* restore list from image */
	block_count = table_size / SQFS_META_BLOCK_SIZE;

	if ((table_size % SQFS_META_BLOCK_SIZE) != 0)
		++block_count;

	locations = alloc_array(sizeof(uint64_t), block_count);

	if (locations == NULL) {
		err = SQFS_ERROR_ALLOC;
		goto fail_data;
	}

	err = file->read_at(file, location, locations,
			    sizeof(uint64_t) * block_count);
	if (err)
		goto fail_idx;

	/* Read the actual data */
	m = sqfs_meta_reader_create(file, cmp, lower_limit, upper_limit);
	if (m == NULL) {
		err = SQFS_ERROR_ALLOC;
		goto fail_idx;
	}

	ptr = data;

	while (table_size > 0) {
		start = le64toh(locations[blk_idx++]);

		err = sqfs_meta_reader_seek(m, start, 0);
		if (err)
			goto fail;

		diff = SQFS_META_BLOCK_SIZE;
		if (diff > table_size)
			diff = table_size;

		err = sqfs_meta_reader_read(m, ptr, diff);
		if (err)
			goto fail;

		ptr = (char *)ptr + diff;
		table_size -= diff;
	}

	sqfs_meta_reader_destroy(m);
	free(locations);
	*out = data;
	return 0;
fail:
	sqfs_meta_reader_destroy(m);
fail_idx:
	free(locations);
fail_data:
	free(data);
	*out = NULL;
	return err;
}
