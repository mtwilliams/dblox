#include "Graphics/Context.h"

#include "Utils/Random.h"

#include "dblox/Resources.h"
#include "dblox/Camera.h"
#include "dblox/World.h"

int main( int argc, char** argv) 
{
    Graphics::Context* Context;

    if( !Graphics::Context::Create("dblox", 1, &Context) )
        return -1;

    if( !Resources::Precache() )
        return -1;

    World* TestWorld = new World(Utils::Random::Integer());
    
    if( !TestWorld->Initialize() )
        return -1;

    Camera Cam = Camera(Math::Vector3(0.0f, -15.0f, -1.0f), Math::Vector3(0.0f, -180.0f, 0.0f), 1.0f, 101.0f, 80.0f, 4.0f/3.0f);

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
                            Pos.Y += 250.0f * DeltaTime;
                        } break;

                    case Keys::S: 
                        {
                            Pos.Y -= 250.0f * DeltaTime;
                        } break;

                    case Keys::Up: 
                        {
                            Pos.X -= sin(Rot.Y * 0.0174532925f) * 250.0f * DeltaTime;
                            Pos.Z += cos(Rot.Y * 0.0174532925f) * 250.0f * DeltaTime;
                        } break;

                    case Keys::Down: 
                        {
                            Pos.X += sin(Rot.Y * 0.0174532925f) * 250.0f * DeltaTime;
                            Pos.Z -= cos(Rot.Y * 0.0174532925f) * 250.0f * DeltaTime;
                        } break;

                    case Keys::Left: 
                        {
                            Rot.Y += 5000.0f * DeltaTime;
                        } break;

                    case Keys::Right: 
                        {
                            Rot.Y -= 5000.0f * DeltaTime;
                        } break;
                    }
                } break;
            }

            Cam.SetPosition(Pos);
            Cam.SetRotation(Rot);
            Cam.Update();
        }

        Context->Clear(Graphics::Color(0, 51, 102, 255), 1.0f);

        Cam.Apply();
        TestWorld->Render();

        Context->Display();
    }

Cleanup:
    return 0;
}
