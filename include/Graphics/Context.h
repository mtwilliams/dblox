#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "Graphics/Exports.h"

#include "Shared/Event.h"
#include "Math/Vector2.h"
#include "Graphics/Color.h"

#include <string>

namespace Graphics
{
	namespace BlendingModes
	{
		enum Mode
		{
			None = 0,
			Additive, 
			Normal,
		};
	};

	struct DBLOX_GRAPHICS_EXPORT VideoMode_t
	{
		unsigned int Width;
		unsigned int Height;
		unsigned int MSAA;
		bool Fullscreen;
	};

	static const VideoMode_t VideoModes[] = {
		{ 640, 480, 4, false },
		{ 800, 600, 4, false },
		{ 1280, 720, 4, false },
		{ 1280, 800, 4, false },
		{ 1440, 900, 4, false },
		{ 1680, 1050, 4, false },
		{ 1920, 1080, 4, false },
		{ 1920, 1200, 4, false },

		{ 640, 480, 4, true },
		{ 800, 600, 4, true },
		{ 1280, 720, 4, true },
		{ 1280, 800, 4, true },
		{ 1440, 900, 4, true },
		{ 1680, 1050, 4, true },
		{ 1920, 1080, 4, true },
		{ 1920, 1200, 4, true }
	};

	struct ContextImpl_t;
	class DBLOX_GRAPHICS_EXPORT Context
	{
		public:
			Context( void );
			~Context();

			static bool Create( const std::string& Title, unsigned int VideoMode, Context** Result );

			void Bind( void );
			void Unbind( void );

			bool IsOpen( void );
			bool GetEvent( Event_t& Event );

			void Clear( float Depth = 1.0f );
			void Clear( const Color& Color, float Depth = 1.0f );
			void Display( void );

			void Clip( const Math::Vector2& Min, const Math::Vector2& Max );
			void Clip( void );

			void SetBlendingMode( BlendingModes::Mode Mode );
			void SetDepthWrite( bool Toogle );

			unsigned int GetWidth( void );
			unsigned int GetHeight( void );
			const VideoMode_t GetVideoMode( void );

			float GetTickCount( void );

		protected:
			ContextImpl_t* Impl;
	};
}

#endif