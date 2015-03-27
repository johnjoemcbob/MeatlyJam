//
//  State.cpp
//  MeatlyJam
//  Created by Ian Copland on 15/10/2014.
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

#include <State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/State.h>

#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/Rendering/Material.h>

namespace MeatlyJam
{
	void State::CreateSystems()
	{
		//Add systems here.
	}

	void State::OnInit()
	{
		// Get a reference to the render component factory, to create the components
		CSRendering::RenderComponentFactory* renderComponentFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();

		// Create the camera entity and add the camera component
		Camera = CSCore::Entity::Create();
		{
			// Create the camera component
			CSRendering::CameraComponentSPtr cameraComponent = renderComponentFactory->CreateOrthographicCameraComponent( CSCore::Vector2( 10, 10 ), 1.0f, 100.0f );

			Camera->AddComponent( cameraComponent );
			Camera->GetTransform().SetLookAt( CSCore::Vector3::k_unitNegativeZ * (f32) 10, CSCore::Vector3::k_zero, CSCore::Vector3::k_unitPositiveY );
		}
		GetScene()->Add( Camera );

		//create the sprite entity and add the sprite component
		Sprite_Test = CSCore::Entity::Create();
		{
			//load the texture and texture atlas.
			CSCore::ResourcePool* resourcePool = CSCore::Application::Get()->GetResourcePool();
			CSRendering::TextureCSPtr texture = resourcePool->LoadResource<CSRendering::Texture>( CSCore::StorageLocation::k_package, "Textures/UI.csimage" );
			CSRendering::TextureAtlasCSPtr textureAtlas = resourcePool->LoadResource<CSRendering::TextureAtlas>( CSCore::StorageLocation::k_package, "Textures/UI.csatlas" );

			//create the material
			CSRendering::MaterialFactory* materialFactory = CSCore::Application::Get()->GetSystem<CSRendering::MaterialFactory>();
			CSRendering::MaterialSPtr material = materialFactory->CreateSprite( "SpriteMaterial", texture );

			//Create the sprite component
			CSRendering::SpriteComponentSPtr spriteComponent = renderComponentFactory->CreateSpriteComponent( CSCore::Vector2( 1, 1 ), textureAtlas, "blue_button00", material, CSRendering::SpriteComponent::SizePolicy::k_fitMaintainingAspect );

			Sprite_Test->AddComponent( spriteComponent );
		}
		GetScene()->Add( Sprite_Test );
	}

	void State::OnUpdate( f32 in_deltaTime )
	{
		//Update stuff here.
	}

	void State::OnDestroy()
	{
		//Destruction stuff here.
	}
}
