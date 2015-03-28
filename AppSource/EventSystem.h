//
//  EventSystem.h
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

#ifndef MEATLYJAM_EVENTSYSTEM_H
#define MEATLYJAM_EVENTSYSTEM_H

// Required engine headers
#include <ChilliSource/ChilliSource.h>

// Required application headers
#include <App.h>
#include <Event.h>

// Defines
#define EVENTS 4

namespace MeatlyJam
{
	class EventSystemClass
	{
		public:
			// Constructor
			EventSystemClass();
			// Destructor
			~EventSystemClass();

			// Initialize the array of possible events
			// IN: N/A
			// OUT: N/A
			void Initialize();

			// Add a new event to the end of the array of possible events
			// IN: (description) The description of the event
			// OUT: N/A
			void AddEvent( char* description );

			// Add an option to the last event added to the array of possible events
			// IN: (option) The option text, (callback) The callback to perform when it is chosen
			// OUT: N/A
			void AddOption( char* option, void (*callback)( EventSystemClass* eventsystem ) );

			// Activate an event & display its information on screen
			// IN: (eventid) The id of the new event
			// OUT: N/A
			void SetEvent( unsigned short eventid );
			// Getter for the id of the current event
			inline unsigned short GetEvent() { return CurrentEvent; };

			// Getter for the array of possible events
			inline EventStruct* GetEvents() { return Event; };

		private:
			// Reference to the application handle
			App* Application;

			// The event UI widgets
			CSUI::WidgetSPtr UI;

			// Array of possible events
			EventStruct* Event;

			// The current event option being initialized
			unsigned short Option;

			// The current event to display
			unsigned short CurrentEvent;

			// The connection to the released inside event option button events
			CSCore::EventConnectionUPtr* Connection_Button_EventOption_ReleaseInside;
	};
}

#endif // MEATLYJAM_EVENTSYSTEM_H