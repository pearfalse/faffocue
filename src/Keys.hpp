/*
 *  Keys.hpp
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _FCLIB_KEYS_HPP
#define _FCLIB_KEYS_HPP

#include <SFML/Window/Keyboard.hpp>

#include "Support.hpp"

namespace FaffoCue
{
	namespace Keys
	{
		const sf::Keyboard::Key
		FULLSCREEN = sf::Keyboard::F,
		FONTSIZE_INCREASE = sf::Keyboard::RBracket,
		FONTSIZE_DECREASE = sf::Keyboard::LBracket,
		MARGIN_INCREASE = sf::Keyboard::Comma,
		MARGIN_DECREASE = sf::Keyboard::Period,
		SCROLL_TOP = sf::Keyboard::Home,
		SCROLL_BOTTOM = sf::Keyboard::End,
		SCROLL_PREVPAGE = sf::Keyboard::PageUp,
		SCROLL_NEXTPAGE = sf::Keyboard::PageDown,
		LINESPACING_INCREASE = sf::Keyboard::Period,
		LINESPACING_DECREASE = sf::Keyboard::Comma,
#if defined(__APPLE__) or defined(MACOSX)
		_MOD1[2] = { sf::Keyboard::LSystem, sf::Keyboard::RSystem },
#else
		_MOD1[2] = { sf::Keyboard::LControl, sf::Keyboard::RControl },
#endif
		_DUMMY = static_cast<sf::Keyboard::Key> (0);
		
		inline bool Mod1Pressed() {
			return sf::Keyboard::isKeyPressed(_MOD1[0]) or sf::Keyboard::isKeyPressed(_MOD1[1]);
		}
	};
};
		
#endif // _FCLIB_KEYS_HPP