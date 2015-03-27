//
//  MenuState.h
//  MeatlyJam
//  Created by Matthew Cormack 27/03/15
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

#ifndef MEATLYJAM_MENUSTATE_H
#define MEATLYJAM_MENUSTATE_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Event.h>

#define TILE_GROUND_WIDTH 133
#define TILE_GROUND_HEIGHT 127
#define TILE_OFFSET_WIDTH 64
#define TILE_OFFSET_HEIGHT 64

#define CITY_WIDTH 5
#define CITY_HEIGHT 5
#define CITY_COS 8

namespace MeatlyJam
{
	class MenuState : public CSCore::State
	{
		public:
			void CreateSystems() override;
			void OnInit() override;
			void OnUpdate( f32 in_deltaTime ) override;
			void OnDestroy() override;

		private:
			// The main menu orthographic camera
			CSCore::EntitySPtr Camera;

			// The background city sprites
			CSCore::EntitySPtr* City;

			// The main menu UI widget
			CSUI::WidgetSPtr UI;

			// Store the time since the state began
			f32 Time;

			// The connection to the released inside play button event
			CSCore::EventConnectionUPtr Connection_Button_Play_ReleaseInside;
	};
}

#endif // MEATLYJAM_MENUSTATE_H