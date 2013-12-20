/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2013 Icinga Development Team (http://www.icinga.org/)   *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "base/allocator.h"
#include "base/debug.h"
#include <boost/thread/mutex.hpp>
#include <sys/mman.h>

using namespace icinga;

struct ncache
{
	unsigned int count;
	unsigned int offset;
	char data[0];
};

#define NCACHESIZE (128 * 1024) /*4096*/
#define NCACHEUSABLE (NCACHESIZE - offsetof(ncache, data))

static ncache *l_Cache;
static int l_CacheCount;

static ncache *new_cache(void)
{
	ncache *cp;

	for (;;) {
		cp = (ncache *)mmap((void *)((1UL << 40) + rand() * NCACHESIZE), NCACHESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

		if (cp == MAP_FAILED || (uintptr_t)cp % NCACHESIZE != 0)
			continue;

		cp->count = 0;
		cp->offset = 0;

		break;
	}

	l_CacheCount++;

	return cp;
}

static void free_cache(ncache *cp)
{
	munmap(cp, NCACHESIZE);

	l_CacheCount--;
}

static boost::mutex& get_mutex(void)
{
	static boost::mutex mtx;
	return mtx;
}

int icinga::nstats(void)
{
	boost::mutex::scoped_lock lock(get_mutex());
	return l_CacheCount;
}

void *icinga::nalloc(size_t size)
{
	VERIFY(size <= NCACHEUSABLE);

	boost::mutex::scoped_lock lock(get_mutex());
	ncache *cp = l_Cache;

	if (!cp || cp->offset + size > NCACHEUSABLE) {
		cp = new_cache();

		if (l_Cache && l_Cache->count == 0)
			free_cache(l_Cache);

		l_Cache = cp;
	}

	void *result = (void *)((uintptr_t)cp->data + cp->offset);

	cp->count++;
	cp->offset += size;

	return result;
}

void icinga::nfree(void *ptr)
{
	boost::mutex::scoped_lock lock(get_mutex());

	ncache *cp = (ncache *)((uintptr_t)ptr - (uintptr_t)ptr % NCACHESIZE);
	cp->count--;

	if (cp->count == 0 && cp != l_Cache)
		free_cache(cp);
}

