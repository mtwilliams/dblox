#include "Graphics/Context.h"

#include "dblox/Textures.h"
#include "dblox/Blocks.h"

#include "dblox/Chunk.h"
#include "dblox/Camera.h"

int main( int argc, char** argv) 
{
	Graphics::Context* Context;

	if( !Graphics::Context::Create("dblox", 1, &Context) )
		return -1;

	if( !Textures::Initialize() )
		return -1;

	if( !Blocks::Initialize() )
		return -1;

	Chunk TestChunk = Chunk(0, 0, 0);

	// Horizontal FoV
	Camera Cam = Camera(Math::Vector3(0.0f, -10.0f, -5.0f), Math::Vector3::Zero, 1.0f, 101.0f, 80.0f, 4.0f/3.0f);

	float LastTime = Context->GetTickCount();
	float CurrentTime = Context->GetTickCount();
	float DeltaTime = 0.0f;

	while( Context->IsOpen() )
	{
		DeltaTime = CurrentTime - LastTime;
		LastTime = CurrentTime;
		CurrentTime = Context->GetTickCount();

		Event_t Event;
		while( Context->GetEvent(Event) )
		{
			if( Event.Type == Events::Closed )
				goto Cleanup;

			Math::Vector3 Pos = Cam.GetPosition();
			Math::Vector3 Rot = Cam.GetRotation();

			switch( Event.Type )
			{
				case Events::KeyPressed:
				{
					switch( Event.Event.KeyPressed.Key )
					{
						case Keys::W: 
						{
							Pos.Y += 500.0f * DeltaTime;
						} break;

						case Keys::S: 
						{
							Pos.Y -= 500.0f * DeltaTime;
						} break;

						case Keys::Up: 
						{
							Pos.X -= sin(Rot.Y * 0.0174532925f) * 500.0f * DeltaTime;
							Pos.Z += cos(Rot.Y * 0.0174532925f) * 500.0f * DeltaTime;
						} break;

						case Keys::Down: 
						{
							Pos.X += sin(Rot.Y * 0.0174532925f) * 500.0f * DeltaTime;
							Pos.Z -= cos(Rot.Y * 0.0174532925f) * 500.0f * DeltaTime;
						} break;

						case Keys::Left: 
						{
							Rot.Y += 10000.0f * DeltaTime;
						} break;

						case Keys::Right: 
						{
							Rot.Y -= 10000.0f * DeltaTime;
						} break;
					}
				} break;
			}

			Cam.SetPosition(Pos);
			Cam.SetRotation(Rot);
			Cam.Update();
		}

		Context->Clear(Graphics::Color::Green, 1.0f);

		Cam.Apply();
		TestChunk.Draw();

		Context->Display();
	}

Cleanup:
	return 0;
}