/*
 *  App.hpp
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _FCLIB_APP_HPP
#define _FCLIB_APP_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include "Support.hpp"
#include "Text.hpp"
#include "FontList.hpp"

namespace FaffoCue {
	
	class App;
	
	namespace App_Impl {
		
		
		// humour me for a second
		/* class AppProxy {
			AppProxy() = 0;
			void tickPreEvents() = 0;
			void tickPostEvents() = 0;
			void onContextChange() = 0;
		}; */
		
		struct ScrollProxy {
			// Handle maths in scroll position calculation
		private:
			friend class FaffoCue::App;
			
			
			float speed, current, min, max, page_height;
			char move;
			
			ScrollProxy(App& a, float _s, float _c, float _min)
			: app(a), speed(_s), current(_c), min(_min), max(0), move(0) {}
			
			void recalc_max();
			
			void validate_current() {
				this->current = std::min(this->current, this->max);
				this->current = std::max(this->current, this->min);
			}
			
			//void recalc_page_height
			
			static const float default_speed;
			
		private:
			App& app;
		};
		
		struct ViewProxy {
			// Handle two-layer views.
		};
		
		struct TextResizeProxy {
			// Handles the text resizing magic
		};
		
		struct OverscanProxy {
			//
		};
			
	};
	
	class App {
	public:
		App(sf::VideoMode, bool, bool, float);
		
		typedef App_Impl::ScrollProxy Scroll;
		
		friend class App_Impl::ScrollProxy;
		
		// getters and setters...
		
		Text& text();
		const Text& text() const;
		
		sf::Font& font();
		const sf::Font& font() const;
		
		sf::RenderWindow& cx();
		const sf::RenderWindow& cx() const;
		
		void load_font(const std::string&);
		void load_font(const char*, std::size_t);
		
		// and the rest
		
		void main();
		
		static const std::string Title;
		static const float MaxTextSize, MinTextSize, LineSpacingGradation;
		
		// FaffoCue script states
		//bool set_inverted(bool = true);
		
		FontList fontList;
		
		
	private:
		sf::RenderWindow m_cx;
		sf::Font m_font;
		Text m_text;
		
		bool m_fullscreen, m_invert;
		
		float m_fontsize, m_fontspeed;
		static const float s_fontspeed;
		
		Scroll m_scroll;
		
		int m_width, m_height;
		
	public:
		bool &fullscreen, &inverted;
		
	};
	
	// GETTERS AND SETTERS GALORE
	
	inline       Text& App::text()       { return m_text; }
	inline const Text& App::text() const { return m_text; }
	
	inline       sf::Font& App::font()       { return m_font; }
	inline const sf::Font& App::font() const { return m_font; }
	
	inline       sf::RenderWindow& App::cx()       { return m_cx; }
	inline const sf::RenderWindow& App::cx() const { return m_cx; }
	
	// and other inlines
	inline void App::load_font(const std::string& path)
	{
		m_font.loadFromFile(path);
	}
	
	inline void App::load_font(const char* data, std::size_t sz)
	{
		m_font.loadFromMemory(data, sz);
	}
	
};

#endif // _FCLIB_APP_HPP