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
	_TextLine::_TextLine(Text& t, const std::string& s)
	: m_text(t)
	, m_original(s)
	, m_wrapped("Dummy", t.m_font, t.m_size)
	
	, original(m_original)
	{
		this->_rewrap();
	}

	
	_TextLine& _TextLine::operator= (const std::string& s)
	{
		m_original = s;
		this->_rewrap();
	}
	
	float _TextLine::wrappedHeight() const
	{
		return m_wrapped.getLocalBounds().height;
	}
	
	void _TextLine::_rewrap()
	{
		// this is where the magic happens.
		// SFML can't do word-wrap, so we have to do it ourselves
		
		
		// we need to:
		// 1) split the line into words
		// 2) re-assemble the words into a line, watching out for the need to force wrap
		
		// Prereq: get the size of a space.
		sf::Vector2f spaceSize = sf::Text("\x20", m_text.m_font, m_text.m_size).findCharacterPos(1);
		
		
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
		std::string dst; dst.reserve(m_original.size() * 2);
		
        for (std::list<std::string>::const_iterator iter = splitWords.begin(), iend = splitWords.end(); iter != iend; ++iter) {
        
			float wThis = sf::Text(*iter, m_text.m_font, m_text.m_size).findCharacterPos((size_t) -1).x + spaceSize.x;
			if ((wCurrent + wThis > wWindow) && /* some words will always be too long! */wCurrent > 0) {
				// no room! create a new line first
				dst += '\n';
				wCurrent = 0;
			}
			// add the word, and reintroduce the space
			(dst      += *iter)  += ' ';
			(wCurrent += wThis)  += spaceSize.y;
		}		
			
		m_wrapped.setString(dst);
		
		//FCLIB_DEBUG( std::cerr << "DEBUG: Wrapped string:" << "EOS" << std::endl; )
		
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
		
		m_lines.push_back(Line(*this, s_copy));
		this->_reflow();
		
		return *this;
		
	}
	
	void Text::_reflow()
	{
		// update the Y coord of each text object.
		float hCurrent = 0;
        for (std::list<Line>::iterator iter = m_lines.begin(), iend = m_lines.end(); iter != iend; ++iter)
        {
			iter->m_wrapped.setPosition(0, hCurrent);
			hCurrent += iter->m_wrapped.getLocalBounds().height + (m_font.getLineSpacing(m_size) * m_lineSpacing);
		}
	}
	
	void Text::draw()
	{
		//FCLIB_DEBUG( m_window.Draw(sf::String("Gumption")); )
        for (std::list<Line>::iterator iter = m_lines.begin(), iend = m_lines.end(); iter != iend; ++iter) {
			m_window.draw(iter->m_wrapped);
		}
	}
	
	void Text::set_size(float sz)
	{
		m_size = sz;
		for (std::list<Line>::iterator iter = m_lines.begin(), iend = m_lines.end(); iter != iend; ++iter) {
			iter->m_wrapped.setCharacterSize(sz);
		}
	}
	
	void Text::update_all()
	{
		for (std::list<Line>::iterator iter = m_lines.begin(), iend = m_lines.end(); iter != iend; ++iter) {
			iter->_rewrap();
		}
		_reflow();
	}
	
	float Text::height() const
	{
		float r = 0.f;
		for (std::list<Line>::const_iterator iter = m_lines.begin(), iend = m_lines.end(); iter != iend; ++iter) {
			r += iter->m_wrapped.getLocalBounds().height;
		}
		return r;
	}
	
	float Text::lineSpacing() const
	{
		return m_lineSpacing;
	}
	
	void Text::setLineSpacing(float spacing)
	{
		if (spacing <= 0) return;
		m_lineSpacing = spacing;
		this->_reflow();
	}
			
	
};
		