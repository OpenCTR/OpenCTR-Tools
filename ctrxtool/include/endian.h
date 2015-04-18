/*
 * endian.h - Macros to define endian-independent operations.
 * 
 * Copyright 2015, Alex Shaw <alex.shaw.as@gmail.com>
 * 
 * This file is part of OpenCTR.
 * 
 * OpenCTR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OpenCTR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OpenCTR.  If not, see <http://www.gnu.org/licenses/>.
 */

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define htobe32(a)  __builtin_bswap32(a)
#define htobe16(a) __builtin_bswap16(a)
#define htole32(a)  (a)
#define htole16(a) (a)
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define htobe16(a)  (a)
#define htobe32(a) (a)
#define htole32(a)  __builtin_bswap32(a)
#define htole16(a) __builtin_bswap16(a)
#else
#error "Error detecting endian order"
#endif

