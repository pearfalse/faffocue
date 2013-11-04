/*
 *  Support.hpp
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _FCLIB_COMMON_HPP
#define _FCLIB_COMMON_HPP

#ifdef FCLIB_DEBUG_ON
#define FCLIB_DEBUG( stmt ) stmt
#else
#define FCLIB_DEBUG( stmt )
#endif

#endif // _FCLIB_COMMON_HPP