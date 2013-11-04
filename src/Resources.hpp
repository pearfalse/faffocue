/*
 *  Resources.hpp
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _FCLIB_RESOURCES_HPP
#define _FCLIB_RESOURCES_HPP

#if defined(__APPLE__) && !defined(__OBJC__)
struct NSString;
#endif

#include <string>

namespace FaffoCue
{
	namespace _impl
	{
		const std::string& resources_location();
		
#ifdef __APPLE__
		NSString* resources_location_cocoa();
#endif
	};
};

#endif
