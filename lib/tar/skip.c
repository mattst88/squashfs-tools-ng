/* SPDX-License-Identifier: GPL-3.0-or-later */
/*
 * skip.c
 *
 * Copyright (C) 2019 David Oberhollenzer <goliath@infraroot.at>
 */
#include "config.h"

#include "tar.h"

#include <stdio.h>

static int skip_bytes(FILE *fp, sqfs_u64 size)
{
	unsigned char buffer[1024];
	size_t diff;

	while (size != 0) {
		diff = sizeof(buffer);
		if (diff > size)
			diff = size;

		if (read_retry("reading tar record padding", fp, buffer, diff))
			return -1;

		size -= diff;
	}

	return 0;
}

int skip_padding(FILE *fp, sqfs_u64 size)
{
	size_t tail = size % 512;

	return tail ? skip_bytes(fp, 512 - tail) : 0;
}

int skip_entry(FILE *fp, sqfs_u64 size)
{
	size_t tail = size % 512;

	return skip_bytes(fp, tail ? (size + 512 - tail) : size);
}
