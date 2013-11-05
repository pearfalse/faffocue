/*
 *  App.cpp
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Support.hpp"
#include "App.hpp"
#include "Keys.hpp"

#include <SFML/Window/ContextSettings.hpp>

#include <stdio.h>
#include <algorithm>

namespace FaffoCue {
    
    const float App::MaxTextSize = 112.f, App::MinTextSize = 24.f, App::LineSpacingGradation = 0.05f;
	
	App::App(sf::VideoMode vm, bool fs, bool inv, sf::Font& font, float textSize)
	: m_cx(vm, App::Title,
		   fs ? sf::Style::Fullscreen : sf::Style::Resize|sf::Style::Close,
		   sf::ContextSettings(24, 8, 2)
		   )
	, m_font(font)
	, m_text(m_cx, font, std::max(textSize, App::MinTextSize), inv)
	
	, m_fullscreen(fs)
	, m_invert(inv)
	
	, fullscreen(m_fullscreen)
	, inverted(m_invert)
	
	, m_fontsize(std::max(textSize, App::MinTextSize)), m_fontspeed(0)
	, m_scroll(*this, App_Impl::ScrollProxy::default_speed, 0, 0) 
	
	, m_width(vm.width), m_height(vm.height)
	{
		m_cx.setFramerateLimit(60);
		m_cx.setVerticalSyncEnabled(true);
	}
	
	void App::main()
	{
//		const sf::Input &cxInput = cx().GetInput();
		double x = 0;
		
		float tempFontSize = m_fontsize;
		
		m_scroll.recalc_max();
		
		// Start loop
		while (m_cx.isOpen())
		{
			bool force_view_redraw = false;
			
			
			// Process events
			sf::Event Event;
			while (m_cx.pollEvent(Event))
			{
				// Close window: exit
				if (Event.type == sf::Event::Closed)
					m_cx.close();
				else if (Event.type == sf::Event::Resized) {
					m_cx.setView(sf::View(sf::FloatRect(
						0.f, 0.f, float(Event.size.width), float(Event.size.height)
					)));
					
					m_width = Event.size.width;
					m_height = Event.size.height;
					
					m_text.update_all();
					m_scroll.recalc_max();
				}
				else if (Event.type == sf::Event::KeyPressed) switch (Event.key.code) {
						
					case FaffoCue::Keys::FULLSCREEN:
						if (!FaffoCue::Keys::Mod1Pressed()) break;
						
						{
							sf::VideoMode dsk = sf::VideoMode::getDesktopMode();
							
							// fullscreening time
							if (!fullscreen) {
								m_cx.create(dsk, "FaffoCue", sf::Style::Fullscreen);
							}
							else {
								m_cx.create(sf::VideoMode(dsk.width/2, dsk.height/2), "FaffoCue");
							}
							m_width = dsk.width; m_height = dsk.height;
						}
						m_fullscreen = !m_fullscreen;
						m_text.update_all();
						m_scroll.recalc_max();
						force_view_redraw = true;
						break;
						
					case FaffoCue::Keys::FONTSIZE_INCREASE:
						m_fontspeed = s_fontspeed;
						break;
						
					case FaffoCue::Keys::FONTSIZE_DECREASE:
						m_fontspeed = -s_fontspeed;
						break;
						
					case FaffoCue::Keys::SCROLL_TOP:
						m_scroll.current = m_scroll.min;
						force_view_redraw = true;
						break;
						
					case FaffoCue::Keys::SCROLL_BOTTOM:
						m_scroll.current = m_scroll.max;
						force_view_redraw = true;
						break;
						
					case FaffoCue::Keys::SCROLL_PREVPAGE:
						m_scroll.current = std::max(m_scroll.current - m_height, m_scroll.min);
						force_view_redraw = true;
						break;
						
					case FaffoCue::Keys::SCROLL_NEXTPAGE:
						m_scroll.current = std::min(m_scroll.current + m_height, m_scroll.max);
						force_view_redraw = true;
						break;
						
					case FaffoCue::Keys::LINESPACING_DECREASE:
						m_text.setLineSpacing(m_text.lineSpacing() - App::LineSpacingGradation);
						break;
						
					case FaffoCue::Keys::LINESPACING_INCREASE:
						m_text.setLineSpacing(m_text.lineSpacing() + App::LineSpacingGradation);
						break;
						
						
						
					case sf::Keyboard::Up:
						m_scroll.move = -m_scroll.speed;
						break;
						
					case sf::Keyboard::Down:
						m_scroll.move = m_scroll.speed;
                        
                    default: break;
						
				}
				else if (Event.type == sf::Event::KeyReleased) switch (Event.key.code) {
						
					case FaffoCue::Keys::FONTSIZE_INCREASE:
					case FaffoCue::Keys::FONTSIZE_DECREASE:
						m_fontspeed = 0;
						m_fontsize = tempFontSize;
						m_text.set_size(m_fontsize);
						m_text.update_all();
						m_scroll.recalc_max();
						force_view_redraw = true;
						break;
						
					case sf::Keyboard::Up:
					case sf::Keyboard::Down:
						m_scroll.move = 0;
						break;
                        
                    default: break;
						
				}
			} // event loop
			
			//
			// TICKS
			//
			
			// Text resizing
			if (m_fontspeed) {
				tempFontSize += m_fontspeed; // accelerate font size while key is held
				if (tempFontSize < App::MinTextSize) tempFontSize = App::MinTextSize;
				if (tempFontSize > App::MaxTextSize) tempFontSize = App::MaxTextSize;
//				fprintf(stderr, "tempFontSize = %.2f\n", tempFontSize);
				force_view_redraw = true;
			}
			
			// Scrolling
			if (m_scroll.move != 0 || force_view_redraw) {
				m_scroll.current += m_scroll.move;
				// These two lines have to be this way round to stop m_scroll.current dropping < 0
				if (m_scroll.current > m_scroll.max) m_scroll.current = m_scroll.max;
				if (m_scroll.current < m_scroll.min) m_scroll.current = m_scroll.min;
				force_view_redraw = true;
			}
			
			if (force_view_redraw) {
				
				float ratio = m_fontsize / tempFontSize;
				
//				fprintf(stderr, "ratio: %.2f\n", ratio);
				
				m_cx.setView(sf::View(
					sf::FloatRect(0, m_scroll.current, m_width * ratio, m_height * ratio)
				));
				
				force_view_redraw = false;
			}
				
				
			
			
			// Clear screen
			m_cx.clear();
			
			// Draw apredefined shape
			//App.Draw(sf::Shape::Circle(sin(x/10)*200 + 300, cos(x/10)*200 + 300, 100, sf::Color::Yellow, 10, sf::Color::Blue));
			m_text.draw();
			
			// Finally, display the rendered frame on screen
			m_cx.display();
			
			
			++x;
		} // loop
		
	} // main
		
	const std::string App::Title = "FaffoCue";
	const float App::s_fontspeed = 1.0f;
	const float App::Scroll::default_speed = 4.f;
	
	namespace App_Impl
	{
		void ScrollProxy::recalc_max() {
			this->max = app.m_text.height() - (app.m_cx.getSize().y / 2);
			validate_current();
		}
		
	}
	
	
}