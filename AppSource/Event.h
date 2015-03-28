//
//  Event.h
//  MeatlyJam
//  Created by Matthew Cormack 28/03/15
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef MEATLYJAM_EVENT_H
#define MEATLYJAM_EVENT_H

// Required application headers
#include <EventSystem.h>

// Defines
#define OPTIONS 3

namespace MeatlyJam
{
	// Forward Declarations
	class EventSystemClass;

	typedef struct
	{
		// The text describing the option to display on the UI
		char* Description;

		// The callback function for this button press
		void (*Callback)( EventSystemClass* eventsystem );
	} OptionStruct;

	typedef struct
	{
		// Description of the event to display on the UI
		char* Description;

		// The options available for this event
		OptionStruct Option[OPTIONS];
	} EventStruct;
}

#endif // MEATLYJAM_EVENT_H