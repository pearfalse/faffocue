//
//  FCFontSelectionController.mm
//  FaffoCue
//
//  Created by Simon Harris on 27/01/2011.
//  Copyright 2011 Roadcamp. All rights reserved.
//

#import "FontSelectionController.h"
#include "FontList.hpp"


@implementation FontSelectionController

@synthesize knownFonts;

- (id)initWithFontsFromSTLMap:(void *)map {
	if ([super initWithWindowNibName:@"FontSelection"] == nil) {
		return nil;
	}
	
	FontList *stlmap = reinterpret_cast<FontList*> (map);
	
	knownFonts = [NSMutableArray arrayWithCapacity:(stlmap->size()) ];
	for (FontList::const_iterator iter = stlmap->begin(); iter != stlmap->end(); ++iter) {
		NSString *s = [NSString stringWithUTF8String:iter->first.c_str()];
		[knownFonts addObject:s];
		NSLog(@"String refcount: %d", [s retainCount]);
	}
	
	return self;
}

@end
