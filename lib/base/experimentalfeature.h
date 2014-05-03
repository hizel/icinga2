/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2014 Icinga Development Team (http://www.icinga.org)    *
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

#ifndef EXPERIMENTALFEATURE_H
#define EXPERIMENTALFEATURE_H

#include "base/i2-base.h"
#include "base/registry.h"
#include "base/singleton.h"
#include "base/value.h"
#include "base/dictionary.h"
#include <vector>
#include <boost/function.hpp>

namespace icinga
{

/**
 * A stats function that can be used to execute a stats task.
 *
 * @ingroup base
 */
class I2_BASE_API ExperimentalFeature : public Object
{
public:
	DECLARE_PTR_TYPEDEFS(ExperimentalFeature);

	ExperimentalFeature(const String& description);

private:
	String m_Description;
};

/**
 * A registry for script functions.
 *
 * @ingroup base
 */
class I2_BASE_API ExperimentalFeatureRegistry : public Registry<ExperimentalFeatureRegistry, String>
{
public:
	static ExperimentalFeatureRegistry *GetInstance(void);
};

/**
 * Helper class for registering ExperimentalFeature implementation classes.
 *
 * @ingroup base
 */
class I2_BASE_API RegisterExperimentalFeatureHelper
{
public:
	RegisterExperimentalFeatureHelper(const String& type, const String& description);
};

#define REGISTER_EXPERIMENTALFEATURE(type, description) \
	I2_EXPORT icinga::RegisterExperimentalFeatureHelper g_RegisterExpFeature_ ## type(#type, description)

}

#endif /* EXPERIMENTALFEATURE_H */
