# License of squashfs-tools-ng

The `libsquashfs` library is released under the terms and conditions of the
**GNU Lesser General Public License version 3 or later**. This applies to
all source code in the directories `lib/sqfs`, `lib/util` and `include/sqfs`
with the following exceptions:

 - `lib/util/xxhash.c` contains a modified implementation of the xxhash32
   algorithm. See `licenses/xxhash.txt` for copyright and licensing
   information (2 clause BSD license).
 - `lib/sqfs/comp/lz4` contains files extracted from the LZ4 compression
   library. See `lib/sqfs/comp/lz4/README` for details and `licenses/LZ4.txt`
   for copyright and licensing information (2 clause BSD license).
 - `lib/sqfs/comp/zlib` contains files that have been extracted from the the
   zlib compression library and modified. See `lib/sqfs/comp/zlib/README` for
   details and `licenses/zlib.txt` for details.

The rest of squashfs-tools-ng is released under the terms and conditions of
the **GNU General Public License version 3 or later**.

Copies of the LGPLv3 and GPLv3 are included in `licenses/LGPLv3.txt` and
`licenses/GPLv3.txt` respectively.

The original source code of squashfs-tools-ng has been written by David
Oberhollenzer in 2019 and onward. Additional contributions have been added
since the initial release which makes some parts of the package subject to the
copyright of the respective authors. Appropriate copyright notices and SPDX
identifiers are included in the source code files.

Although the existing squashfs-tools and the Linux kernel implementation have
been used for testing, the source code in this package is neither based on,
nor derived from either of them.

# Binary Packages with 3rd Party Libraries

If this file is included in a binary release package, additional 3rd party
libraries may be included, which are subject to the copyright of their
respective authors and the terms and conditions of their respective liceses.

The following may be included:

 - The LZO compression library. Copyright Markus F.X.J. Oberhumer. This is
   released under the terms and conditions of the GNU General Public License
   version 2. A copy of the license is included in `licenses/GPLv2.txt`.
 - The LZ4 compression library. Copyright Yann Collet. This is released under a
   2 clause BSD style license, included in `licenses/LZ4.txt`. This library may
   be linked directly into `libsquashfs`, built from source code included in
   the source distribution.
 - The XZ utils liblzma library is released into the public domain. An excerpt
   from the `COPYING` file of its source code archive is included
   in `licenses/xz.txt`.
 - The zlib compression library. Copyright Jean-loup Gailly and Mark Adler.
   This is released under the terms and conditions of the zlib license,
   included in `licenses/zlib.txt`. This library may be linked directly
   into `libsquashfs`, built from source code included in the source
   distribution.
 - The zstd compression library. Copyright Facebook, Inc. All rights reserved.
   This is released under a BSD style license, included in `licenses/zstd.txt`.


Independend of build configurations, the `libsquashfs` library contains
the following 3rd party source code, directly linked into the library:

 - A modified version of the xxhash32 hash function (Copyright Yann Collet).
   This is released under a 2-Clause BSD License. See `licenses/xxhash.txt`
   for details.
