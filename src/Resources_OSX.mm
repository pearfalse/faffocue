/*
 *  Resources_OSX.mm
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Support.hpp"

#include <string>

#import <Foundation/NSBundle.h>
#import <Foundation/NSString.h>

namespace FaffoCue
{
	namespace _impl
	{
		const std::string& resources_location()
		{
			static std::string result;
			
			if (result.empty()) {
				result = [[[NSBundle mainBundle] resourcePath] UTF8String];
			}
			return result;
		}
		
		NSString* resources_location_cocoa()
		{
			return [[NSBundle mainBundle] resourcePath];
		}
	}; // _impl
} // FaffoCue