//
//  EventSystem.cpp
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

// Associated header
#include <EventSystem.h>

// Required application headers
#include <EventCallbacks.h>
#include <GameState.h>

// Required engine headers
#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Event.h>

#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace MeatlyJam
{
	// Constructor
	EventSystemClass::EventSystemClass()
	{
		UI = 0;
		Event = 0;
		Option = 0;
		CurrentEvent = -1; // Used during initialize to hold the number of events
	}
	// Destructor
	EventSystemClass::~EventSystemClass()
	{
		if ( Event != 0 )
		{
			delete[] Event;
			Event = 0;
		}

		delete[] Connection_Button_EventOption_ReleaseInside;
	}

	// Initialize the array of possible events
	// IN: N/A
	// OUT: N/A
	void EventSystemClass::Initialize()
	{
		// Get a reference to the resource pool for this application
		auto resourcepool = CSCore::Application::Get()->GetResourcePool();

		// Create the event UI element
		auto widgetfactory = CSCore::Application::Get()->GetWidgetFactory();
		auto templatewidget = resourcepool->LoadResource<CSUI::WidgetTemplate>( CSCore::StorageLocation::k_package, "UI/Event.csui" );

		UI = widgetfactory->Create( templatewidget );
		CSCore::Application::Get()->GetStateManager()->GetActiveState()->GetUICanvas()->AddWidget( UI );

		// Add option callbacks
		Connection_Button_EventOption_ReleaseInside = new CSCore::EventConnectionUPtr[OPTIONS];
		for ( unsigned short option = 0; option < OPTIONS; option++ )
		{
			char* bordername = new char[13];
			{
				sprintf( bordername, "Option%iBorder", option + 1 );
			}
			CSUI::WidgetSPtr button = UI->GetWidget( "Panel" )->GetWidget( "OptionsBorder" )->GetWidget( bordername );
			Connection_Button_EventOption_ReleaseInside[option] = button->GetReleasedInsideEvent().OpenConnection(
				[]( CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType )
				{
					GameStateClass* game = (GameStateClass*) CSCore::Application::Get()->GetStateManager()->GetActiveState().get();
					unsigned short* optionid = new unsigned short;
					{
						sscanf( in_widget->GetName().c_str(), "Option%i", optionid ); // Find the number of the option from its widget name
						(*optionid) -= 1; // - 1 for code array starting at 0
					}
					OptionStruct option = game->GetEventSystem()->GetEvents()[game->GetEventSystem()->GetEvent()].Option[(*optionid)];
					if ( option.Description != "" ) // Option exists
					{
						option.Callback( game->GetEventSystem() );
					}
					//delete optionid;
				}
			);
		}

		// Initialize array of possible events
		Event = new EventStruct[EVENTS];
		{
			AddEvent( "Life happens, what do you do?" );
			{
				AddOption( "Embrace it, it is beautiful", EventCallback_Life_Embrace );
				AddOption( "Game a game about it, I am a game dev", EventCallback_Life_Game );
				AddOption( "Ignore it, maybe it will go away", EventCallback_Life_Ignore );
			}
			AddEvent( "You embrace life." );
			{
				AddOption( "Cool.", EventCallback_Life );
			}
			AddEvent( "You make a game!" );
			{
				AddOption( "I like games", EventCallback_Life );
				AddOption( "Yatta!", EventCallback_Life_Embrace );
			}
			AddEvent( "You ignore it for a while, but it does not leave" );
			{
				AddOption( "Embrace it", EventCallback_Life_Embrace );
				AddOption( "Game a game about it, I am a game dev", EventCallback_Life_Game );
				AddOption( "Ignore it, maybe it will go away this time", EventCallback_Life_Ignore );
			}
		}
		SetEvent( 0 ); // Initialize to first event
	}

	// Add a new event to the end of the array of possible events
	// IN: (description) The description of the event
	// OUT: N/A
	void EventSystemClass::AddEvent( char* description )
	{
		CurrentEvent++;
		Option = 0;
		Event[CurrentEvent].Description = description;

		// Initialize options to not exist
		for ( unsigned short option = 0; option < OPTIONS; option++ )
		{
			Event[CurrentEvent].Option[option].Description = "";
		}
	}

	// Add an option to the last event added to the array of possible events
	// IN: (option) The option text, (callback) The callback to perform when it is chosen
	// OUT: N/A
	void EventSystemClass::AddOption( char* option, void (*callback)( EventSystemClass* eventsystem ) )
	{
		Event[CurrentEvent].Option[Option].Description = option;
		Event[CurrentEvent].Option[Option].Callback = callback;
		Option++;
	}

	// Activate an event & display its information on screen
	// IN: (eventid) The id of the new event
	// OUT: N/A
	void EventSystemClass::SetEvent( unsigned short eventid )
	{
		CurrentEvent = eventid;
		CSUI::WidgetSPtr border = UI->GetWidget( "Panel" )->GetWidget( "DescriptionBorder" );
		if ( Event[CurrentEvent].Description != "" ) // Event exists
		{
			// Add description
			border->SetVisible( true );
			border->GetWidget( "Description" )->GetComponent<CSUI::TextComponent>()->SetText( Event[CurrentEvent].Description );
			// Add options
			for ( unsigned short option = 0; option < OPTIONS; option++ )
			{
				char* bordername = new char[13];
				char* buttonname = new char[7];
				{
					sprintf( bordername, "Option%iBorder", option + 1 );
					sprintf( buttonname, "Option%i", option + 1 );
				}
				CSUI::WidgetSPtr button = UI->GetWidget( "Panel" )->GetWidget( "OptionsBorder" )->GetWidget( bordername );
				CSUI::WidgetSPtr buttonlabel = button->GetWidget( buttonname );
				button->SetVisible( Event[CurrentEvent].Option[option].Description != "" ); // Hide non existant options
				buttonlabel->GetComponent<CSUI::TextComponent>()->SetText( Event[CurrentEvent].Option[option].Description );
			}
		}
		else // Otherwise hide UI
		{
			border->SetVisible( false );
		}
	}
}