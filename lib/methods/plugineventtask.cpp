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

#include "methods/plugineventtask.hpp"
#include "icinga/eventcommand.hpp"
#include "icinga/macroprocessor.hpp"
#include "icinga/pluginutility.hpp"
#include "icinga/icingaapplication.hpp"
#include "base/dynamictype.hpp"
#include "base/logger_fwd.hpp"
#include "base/scriptfunction.hpp"
#include "base/utility.hpp"
#include "base/process.hpp"
#include <boost/foreach.hpp>

using namespace icinga;

REGISTER_SCRIPTFUNCTION(PluginEvent, &PluginEventTask::ScriptFunc);

void PluginEventTask::ScriptFunc(const Checkable::Ptr& checkable)
{
	EventCommand::Ptr commandObj = checkable->GetEventCommand();

	Host::Ptr host;
	Service::Ptr service;
	tie(host, service) = GetHostService(checkable);

	MacroProcessor::ResolverList resolvers;
	if (service)
		resolvers.push_back(std::make_pair("service", service));
	resolvers.push_back(std::make_pair("host", host));
	resolvers.push_back(std::make_pair("command", commandObj));
	resolvers.push_back(std::make_pair("icinga", IcingaApplication::GetInstance()));

	PluginUtility::ExecuteCommand(commandObj, checkable, checkable->GetLastCheckResult(), resolvers);
}
