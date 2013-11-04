/*
 *  Text.hpp
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef	_FCLIB_TEXT_HPP
#define _FCLIB_TEXT_HPP

#include <string>
#include <list>

#include "Support.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
//#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace FaffoCue
{
	class Text;
	
	class _TextLine
	{
		
		public:
		_TextLine(Text&, const std::string&);
		
		_TextLine& operator=(const std::string&);
		
		float wrappedHeight() const;
		const std::string& original;
		
		private:
		
		friend class Text;
		
		void _rewrap();
		Text &m_text;
		std::string m_original;
        sf::Text m_wrapped;
		
	};
	
	class Text
	{
		
		public:
		
		// Inversion begin/end characters. Don't keep the textual ones, so that
		// the user can change this further on in program execution.
		static const char
		BeginInversion = 0x01,
		EndInversion   = 0x0f;
		
		typedef _TextLine Line;
		friend class _TextLine;
		
		
		Text(sf::RenderWindow&, const sf::Font&, float, bool = false);
		
		Text& add_line(const std::string&);
		void draw();
		void update_all();
		void set_size(float);
		
		void rewrap();
		void rerenderWords();
		
		float height() const;
		float lineSpacing() const;
		void setLineSpacing(float);
		
		private:
		void _reflow();
		void _resplit();
		
		sf::RenderWindow& m_window;
		const sf::Font &m_font;
		float m_size;
		bool m_invert;
		
		sf::Vector2f m_spaceSize;
		float m_lineSpacing;
		
		std::list<Line> m_lines;
	};
};

#endif // _FCLIB_TEXT_HPP
