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

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "base/i2-base.h"
#include <stddef.h>

namespace icinga
{

I2_BASE_API void *nalloc(size_t size);
I2_BASE_API void nfree(void *ptr);
I2_BASE_API int nstats(void);

template<class T>
class naive_allocator
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T *pointer;
	typedef const T *const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	template<class U>
	struct rebind {
		typedef naive_allocator<U> other;
	};

	naive_allocator(void) throw()
	{ }

	naive_allocator(const naive_allocator& other) throw()
	{ }

	template<class U>
	naive_allocator(const naive_allocator<U>&)
	{ }

	pointer allocate(size_type n, const void * = 0)
	{
		return (pointer)nalloc(n * sizeof(T));
	}

	void deallocate(void *p, size_type)
	{
		nfree(p);
	}

	pointer address(reference x) const
	{
		return &x;
	}

	const_pointer address(const_reference x) const
	{
		return &x;
	}

	naive_allocator<T>& operator=(const naive_allocator&)
	{
		return *this;
	}

	template<class U>
	naive_allocator& operator=(const naive_allocator<U>&)
	{
		return *this;
	}

	void construct(pointer p, const T& val)
	{
		new ((T *) p) T(val);
	}

	void destroy(pointer p)
	{
		p->~T();
	}

	size_type max_size(void) const
	{
		return size_t(-1);
	}

	template<class U>
	bool operator==(const naive_allocator<U>&) const
	{
		return true;
	}
};

}

#endif /* ALLOCATOR_H */
