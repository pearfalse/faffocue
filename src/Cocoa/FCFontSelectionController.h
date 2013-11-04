//
//  FCFontSelectionController.h
//  FaffoCue
//
//  Created by Simon Harris on 27/01/2011.
//  Copyright 2011 Roadcamp. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface FontSelectionController : NSWindowController {
	
	NSMutableArray *knownFonts;

}

@property (readonly, assign) IBOutlet NSArray *knownFonts;

- (id)initWithFontsFromSTLMap:(void *)map;

@end
