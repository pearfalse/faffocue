/*
 *  main.cpp
 *  FaffoCue
 *
 *  Copyright (C) Simon Harris 2010-2013.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

// This is very rough-and-ready main.cpp, until we can get the mainloop
// integrated with Cocoa and Winforms/WPF.

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <TCLAP/CmdLine.h>
#include <sstream>

////////////////////////////////////////////////////////////
// FaffoCue headers
////////////////////////////////////////////////////////////
#include "Text.hpp"
#include "Keys.hpp"
#include "Resources.hpp"

#include "App.hpp"

namespace fcmain
{
	unsigned int resWidth, resHeight;
	bool fullscreen;
	std::string script_file, font_file;
	double font_size;
	////////////////////////////////////////////////////////////
	// Command-line argument parsing
	//
	// -t --font: font name
	// -z --font-size
	// -r --resolution: screen resolution
	// -f --fullscreen: overrides resolution
	// -w --windowed: in case fullscreen is somehow the default
	// unlabeled: autocue script (extension .fcx for xml, .fct for text)
	////////////////////////////////////////////////////////////
	void parse_cmdline(int argc, char *argv[])
	{
		try {
			TCLAP::CmdLine cmd("These command-line arguments are not recommended for production setups.", ' ', "3.0");
			
			TCLAP::ValueArg<std::string>
			resolution("r", "resolution", "Screen resolution", true, "!", "<number>x<number>", cmd),
			font_file("t", "font", "Font names", true, "!", "string", cmd);
			
			TCLAP::ValueArg<float>
			font_size("z", "font-size", "Font size", false, 48.f, "positive number", cmd);
			
			TCLAP::SwitchArg
			fullscreen("f", "fullscreen", "Fullscreen", cmd),
			windowed("w", "windowed", "Windowed", cmd, true);
			
			TCLAP::UnlabeledValueArg<std::string>
			script_file("script-file", "Autocue script to load", false, "", "file path");
			
			cmd.parse(argc, argv);
			
			// fc-specific parsing time
			
			fcmain::fullscreen = fullscreen.getValue() && !windowed.getValue();
			fcmain::font_file = font_file.getValue();
			fcmain::font_size = font_size.getValue();
			fcmain::script_file = script_file.getValue();
			
			{
				std::stringstream ss(resolution.getValue());
				char dummy;
				/* try: */ ss >> fcmain::resWidth >> dummy >> fcmain::resHeight;
				if (!ss) throw TCLAP::ArgException("Bad resolution format (should be <number>x<number>)", "resolution");
			}
		}
		catch (TCLAP::ArgException& e) {
			std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
			exit(1);
		}
		
	}
	
}; // ns


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	//fcmain::parse_cmdline(argc, argv);
	// @HACK@
	fcmain::resWidth = 800; fcmain::resHeight = 600;
	fcmain::font_size = 24.f;
	fcmain::fullscreen = false;
	
	
    //sf::RenderWindow App(sf::VideoMode(640, 480), "FaffoCue");
	
	FaffoCue::App app(sf::VideoMode(fcmain::resWidth, fcmain::resHeight), false, false);
	
	//App.SetFramerateLimit(60);
	//App.UseVerticalSync(true);
	
//	sf::Font fnt;
	fcmain::font_file = FaffoCue::_impl::resources_location() + "/AurulentSans-Bold.otf"; // @HACK@
	app.font().loadFromFile(fcmain::font_file);
	app.text().add_line("The quick brown fox jumps over the lazy dog. And here's another sentence because the last one wasn't really long enough to test the word wrap thoroughly. But this should be okay.");
	app.text().add_line("And now, for a limited time only, a second line of text!");
	
//	sf::View  &cxView  = app.cx().GetDefaultView();
//	const sf::Input &cxInput = app.cx().GetInput();
	
	double x = 0;
	bool fullscreen = false;

    
	app.main();

    return EXIT_SUCCESS;
}
