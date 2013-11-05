/*
 *  Text.cpp
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Text.hpp"

#include <string>

#ifdef FCLIB_DEBUG_ON
#include <iostream>
#include <SFML/Graphics/Shape.hpp>
#endif

namespace FaffoCue
{
	_TextLine::_TextLine(Text& t, const std::string& s, float initialY)
	: m_text(t)
	, m_original(s)
	
	, original(m_original)
	, m_bounds(0.f, 0.f, 0.f, 0.f)
	, m_globalY(initialY)
	{
		this->_rewrap(initialY);
	}

	
	_TextLine& _TextLine::operator= (const std::string& s)
	{
		m_original = s;
		this->_rewrap(m_globalY);
	}
	
	float _TextLine::wrappedHeight() const
	{
		return _bottom();
	}
	
	void _TextLine::_rewrap(float initialY)
	{
		// this is where the magic happens.
		// SFML can't do word-wrap, so we have to do it ourselves
		
		
		// we need to:
		// 1) split the line into words
		// 2) re-assemble the words into a line, watching out for the need to force wrap
		
		// Prereq: get the size of a space.
		sf::Vector2f spaceSize = sf::Text("\x20\n", m_text.m_font, m_text.m_size).findCharacterPos(2);
		this->m_text.m_spaceSize = spaceSize;
		//FCLIB_DEBUG( fprintf(stderr, "spaceSize: %.2f,%.2f", spaceSize.x, spaceSize.y); )
		this->m_globalY = initialY;
		
		// 1) split the line into words
		std::string thisWord;
		std::list<std::string> splitWords;
        
		for (std::string::const_iterator iter = m_original.begin(), iend = m_original.end(); iter != iend; ++iter) {
			// iterate through letters. when we reach a space, break off & start a new word
			if (*iter == ' ') {
				if (!thisWord.empty()) splitWords.push_back(thisWord);
				thisWord.clear();
			}
			// not a space, add this letter
			else {
				thisWord.append(1, *iter);
			}
		}
		splitWords.push_back(thisWord);
		// end step 1
		
		// step 2: pixel width of each word, bake in newlines
		float wWindow = this->m_text.m_window.getSize().x, wCurrent = 0;
		m_wrappedLines.clear();
		std::string dst; dst.reserve(m_original.size() * 2);
		sf::Vector2f runningPosition = sf::Vector2f(0.f, initialY);
		
        for (std::list<std::string>::const_iterator iter = splitWords.begin(), iend = splitWords.end(); iter != iend; ++iter) {
			sf::FloatRect fr = sf::Text(dst + ' ' + *iter, m_text.m_font, m_text.m_size).getLocalBounds();
			float wThis = fr.width + spaceSize.x;
			if (wCurrent > 0 && wThis > wWindow /* some words will always be too long! */) {
				// no room! create a new line first
				m_wrappedLines.push_back(sf::Text(dst, this->m_text.m_font, this->m_text.m_size));
				m_wrappedLines.back().setPosition(runningPosition);
				runningPosition.y += (spaceSize.y * m_text.m_lineSpacing);
				//FCLIB_DEBUG( fprintf(stderr, "Saving line \"%s\" -- runningPosition: %.2f,%.2f (line width=%.2f)\n", dst.c_str(), runningPosition.x, runningPosition.y, m_wrappedLines.back().getLocalBounds().width); )
				dst.clear();
				wCurrent = 0;
			}

			// add the word, and reintroduce the space
			dst      += *iter;
			dst      += ' ';
			wCurrent += wThis;
		}
		
		m_wrappedLines.push_back(sf::Text(dst, this->m_text.m_font, this->m_text.m_size));
		m_wrappedLines.back().setPosition(runningPosition);
		runningPosition.y += (spaceSize.y * m_text.m_lineSpacing);
		
		this->m_bounds = sf::FloatRect(0.f, initialY, m_text.m_window.getSize().x, runningPosition.y - initialY);
		//FCLIB_DEBUG( fprintf(stderr, "this->m_bounds: %.2f,%.2f,+%.2f,+%.2f\n", m_bounds.left, m_bounds.top, m_bounds.width, m_bounds.height); )
			
		//m_wrapped.setString(dst);
		
		//FCLIB_DEBUG( std::cerr << "DEBUG: Wrapped string:" << "EOS" << std::endl; )
		
	}
	
	void _TextLine::_drawTo(sf::RenderWindow& rw)
	{
		for (std::list<sf::Text>::iterator jter = m_wrappedLines.begin(), jend = m_wrappedLines.end(); jter != jend; ++jter) {
			rw.draw(*jter);
		}
	}
	
	float _TextLine::_bottom() const
	{
		return m_bounds.top + m_bounds.height + (m_text.m_spaceSize.y * m_text.m_lineSpacing);
	}
	
	///////////////////////////////////////////////////////////////////
	// FaffoCue::Text
	///////////////////////////////////////////////////////////////////
	
	Text::Text(sf::RenderWindow& w, const sf::Font& f, float size, bool invert)
	: m_window(w)
	, m_font(f)
	, m_size(size)
	, m_invert(invert)
	, m_spaceSize(m_size, m_size)
	, m_lineSpacing(1.0f)
	{ }
	
	Text& Text::add_line(const std::string& s)
	{
		std::string s_copy = s;
		// TODO: handle situation where text contains a \n or \r
		// For now, we replace it with a space, as with \t.
        for (std::string::iterator iter = s_copy.begin(), iend = s_copy.end(); iter != iend; ++iter) {
			if (*iter < 0x20) *iter = ' ';
		}
		
		float start = 0.f;
		if (!m_lines.empty()) {
			//sf::FloatRect &fr = m_lines.back().m_bounds;
			start = m_lines.back()._bottom();
		}
		m_lines.push_back(Line(*this, s_copy, start));
		
		return *this;
		
	}
	
	void Text::draw()
	{
		
		//FCLIB_DEBUG( m_window.Draw(sf::String("Gumption")); )
        for (std::list<Line>::iterator iter = m_lines.begin(), iend = m_lines.end(); iter != iend; ++iter) {
			//m_window.draw(iter->m_wrapped);
			iter->_drawTo(m_window);
		}
	}
	
	void Text::set_size(float sz)
	{
		m_size = sz;
		update_all();
	}
	
	void Text::update_all()
	{
		float initialY = 0.f;
		for (std::list<Line>::iterator iter = m_lines.begin(), iend = m_lines.end(); iter != iend; ++iter) {
			iter->_rewrap(initialY);
			initialY = iter->_bottom();
		}
		//_reflow();
	}
	
	float Text::height() const
	{
		return m_lines.back()._bottom();
	}
	
	float Text::lineSpacing() const
	{
		return m_lineSpacing;
	}
	
	void Text::setLineSpacing(float spacing)
	{
		if (spacing <= 0) return;
		m_lineSpacing = spacing;
		this->update_all();
	}
			
	
};
		