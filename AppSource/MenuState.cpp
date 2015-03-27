//
//  MenuState.cpp
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

#include <MenuState.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/Rendering/Material.h>

#include <GameState.h>

namespace MeatlyJam
{
	void MenuState::CreateSystems()
	{
		
	}

	void MenuState::OnInit()
	{
		// Initialize state timer
		Time = 0;

		// Initialize background to light blue
		GetScene()->SetClearColour( CSCore::Colour::k_cornflowerBlue );

		// Get a reference to the render component factory, to create the components
		CSRendering::RenderComponentFactory* renderComponentFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();

		// Get a reference to the material creation factory
		CSRendering::MaterialFactory* materialfactory = CSCore::Application::Get()->GetSystem<CSRendering::MaterialFactory>();

		// Get a reference to the resource pool for this application
		auto resourcepool = CSCore::Application::Get()->GetResourcePool();

		// Create the camera entity and add the camera component
		Camera = CSCore::Entity::Create();
		{
			// Create the camera component
			CSRendering::CameraComponentSPtr cameraComponent = renderComponentFactory->CreateOrthographicCameraComponent( CSCore::Vector2( 1280, 720 ), 1.0f, 100.0f );

			Camera->AddComponent( cameraComponent );
			Camera->GetTransform().SetLookAt( CSCore::Vector3::k_unitNegativeZ * (f32) 10, CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY );
		}
		GetScene()->Add( Camera );

		// Create the background city
		City = new CSCore::EntitySPtr[CITY_WIDTH * CITY_HEIGHT];
		{
			// Load the texture and texture atlas
			CSRendering::TextureCSPtr texture_building = resourcepool->LoadResource<CSRendering::Texture>( CSCore::StorageLocation::k_package, "Textures/Building.csimage" );

			CSRendering::TextureAtlasCSPtr textureatlas_building = resourcepool->LoadResource<CSRendering::TextureAtlas>( CSCore::StorageLocation::k_package, "Textures/Building.csatlas" );

			// Create the material
			CSRendering::MaterialSPtr material_building = materialfactory->CreateSprite( "SpriteMaterial_Building", texture_building );
			material_building->SetBlendModes( CSRendering::BlendMode::k_one, CSRendering::BlendMode::k_oneMinusSourceAlpha );
			material_building->SetTransparencyEnabled( true );

			// Load the texture and texture atlas
			CSRendering::TextureCSPtr texture_road = resourcepool->LoadResource<CSRendering::Texture>( CSCore::StorageLocation::k_package, "Textures/Road.csimage" );

			CSRendering::TextureAtlasCSPtr textureatlas_road = resourcepool->LoadResource<CSRendering::TextureAtlas>( CSCore::StorageLocation::k_package, "Textures/Road.csatlas" );

			// Create the material
			CSRendering::MaterialSPtr material_road = materialfactory->CreateSprite( "SpriteMaterial_Road", texture_road );
			material_road->SetBlendModes( CSRendering::BlendMode::k_one, CSRendering::BlendMode::k_oneMinusSourceAlpha );
			material_road->SetTransparencyEnabled( true );

			// Place roads/buildings
			for ( unsigned short column = 0; column < CITY_WIDTH; column++ )
			{
				for ( unsigned short row = 0; row < CITY_HEIGHT; row++ )
				{
					unsigned short index = ( row * CITY_WIDTH ) + column;
					City[index] = CSCore::Entity::Create();
					{
						CSRendering::TextureAtlasCSPtr textureatlas = textureatlas_building;
						CSRendering::MaterialSPtr material = material_building;
						std::string sprite = "buildingTiles_012";
						{
							if ( ( ( column % 3 ) == 0 ) || ( ( row % 3 ) == 0 ) )
							{
								textureatlas = textureatlas_road;
								material = material_road;
								sprite = "cityTiles_000";

								// Pick the correct road tile
								if ( row == 0 ) // Last row of city
								{
									if ( column == 0 ) // First column, corner
									{
										sprite = "cityTiles_126";
									}
									else if ( column == ( CITY_WIDTH - 1 ) ) // Last column, corner
									{
										sprite = "cityTiles_125";
									}
									else if ( ( column % 3 ) == 0 ) // Junction
									{
										sprite = "cityTiles_095";
									}
									else
									{
										sprite = "cityTiles_003";
									}
								}
								else if ( row == ( CITY_HEIGHT - 1 ) ) // First row of city
								{
									if ( column == 0 ) // First column, corner
									{
										sprite = "cityTiles_124";
									}
									else if ( column == ( CITY_WIDTH - 1 ) ) // Last column, corner
									{
										sprite = "cityTiles_122";
									}
									else if ( ( column % 3 ) == 0 ) // Junction
									{
										sprite = "cityTiles_095";
									}
									else
									{
										sprite = "cityTiles_000";
									}
								}
								else if ( ( column % 3 ) == 0 ) // Vertical road
								{
									if ( ( row % 3 ) == 0 ) // Cross roads
									{
										sprite = "cityTiles_089";
									}
									else // Vertical road
									{
										sprite = "cityTiles_057";
									}
								}
								else if ( ( row % 3 ) == 0 ) // Horizontal road
								{
									sprite = "cityTiles_050";
								}
							}
							else // Choose a random building model
							{
								char* spritetemp = new char[18];
								{
									sprintf( spritetemp, "buildingTiles_0%i", (int) ( rand() % 20 ) + 17 );
								}
								sprite = spritetemp;
							}
						}

						CSRendering::SpriteComponentSPtr spritecomponent = renderComponentFactory->CreateSpriteComponent(
							CSCore::Vector2( TILE_GROUND_WIDTH, TILE_GROUND_HEIGHT ),
							textureatlas,
							sprite,
							material,
							CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect
						);
						spritecomponent->SetOriginAlignment( CSRendering::AlignmentAnchor::k_topCentre );
						City[index]->AddComponent( spritecomponent );

						City[index]->GetTransform().SetPosition( CSCore::Vector3( ( column - row ) * TILE_OFFSET_WIDTH, ( ( column + row - 2 ) * 0.5f ) * TILE_OFFSET_HEIGHT, index ) );
					}
					GetScene()->Add( City[index] );
				}
			}
		}

		// Create the test text UI element
		{
			auto widgetFactory = CSCore::Application::Get()->GetWidgetFactory();

			auto templateWidget = resourcepool->LoadResource<CSUI::WidgetTemplate>( CSCore::StorageLocation::k_package, "UI/Event.csui" );

			UI = widgetFactory->Create( templateWidget );
			GetUICanvas()->AddWidget( UI );

			// Sort out name & twitter credits
			//CSUI::TextComponent* text = UI->GetWidget( "Label_Name" )->GetComponent<CSUI::TextComponent>();
			//text->SetTextScale( 0.5f );
			//text = UI->GetWidget( "Label_Handle" )->GetComponent<CSUI::TextComponent>();
			//text->SetTextScale( 0.5f );

			//// Add play button event
			//Connection_Button_Play_ReleaseInside = UI->GetWidget( "Button_Play" )->GetReleasedInsideEvent().OpenConnection(
			//	[]( CSUI::Widget* in_widget, const CSInput::Pointer& in_pointer, CSInput::Pointer::InputType in_inputType )
			//	{
			//		CSCore::Application::Get()->GetStateManager()->Change( (CSCore::StateSPtr) new GameState() );
			//	}
			//);
		}
	}

	void MenuState::OnUpdate( f32 in_deltaTime )
	{
		// Juice: Sine wave the buildings slowly
		Time += in_deltaTime;
		for ( unsigned short column = 0; column < CITY_WIDTH; column++ )
		{
			for ( unsigned short row = 0; row < CITY_HEIGHT; row++ )
			{
				unsigned short index = ( row * CITY_WIDTH ) + column;
				f32 offx = column;
				{
					// Offset the cap to the center
					offx += (f32) CITY_WIDTH / 2;
					// Normalize offset
					offx /= CITY_WIDTH;
					// Normalize to account for x and y
					offx /= 2;
				}
				f32 offy = row;
				{
					// Offset the cap to the center
					offy += (f32) CITY_HEIGHT / 2;
					// Normalize offset
					offy /= CITY_HEIGHT;
					// Normalize to account for x and y
					offy /= 2;
				}
				double costime = cos( Time + offx + offy );
				City[index]->GetTransform().MoveBy( 0, costime / CITY_COS, 0 );
			}
		}
	}

	void MenuState::OnDestroy()
	{
		// Cleanup city background array of tiles
		delete[] City;
		City = 0;
	}
}
