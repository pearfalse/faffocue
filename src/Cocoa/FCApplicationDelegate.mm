//
//  FCApplicationDelegate.mm
//  FaffoCue
//
//  Created by Simon Harris on 29/01/2011.
//  Copyright 2011 Roadcamp. All rights reserved.
//

#import "FCApplicationDelegate.h"

#include "Resources.hpp"


@implementation FCApplicationDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)aNotification {
	
	// Gather list of fonts
	NSFileManager *fileManager = [[NSFileManager alloc] init];
	NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:(FaffoCue::_impl::resources_location_cocoa())];
	
	
	NSAlert *a = [[NSAlert alloc] init];
	[a setAlertStyle:NSInformationalAlertStyle];
	[a setMessageText:@"This version of FaffoCue doesn't work."];
	[a setInformativeText:@"The UI is being prototyped, and SFML hasn't been brought back in the picture. Try hg revision proof-of-concept to play with pretty text."];
	[a addButtonWithTitle:@"OK"];
	[a runModal];
	
	[fileManager release];
	
}

@end
