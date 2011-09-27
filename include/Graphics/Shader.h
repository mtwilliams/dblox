#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include "Graphics/Exports.h"

#include "Shared/Platform.h"
#include "Math/Matrix4.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Graphics/Color.h"
#include "Graphics/Format.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderTarget.h"

#include <map>

namespace Graphics
{
	typedef struct
	{
		const char* Name;
		const char* Value;
	} ShaderDefine_t;

	struct ShaderImpl_t;
	class DBLOX_GRAPHICS_EXPORT Shader
	{
		public:
			Shader( void );
			~Shader();

			static bool CreateFromFX( const std::string& Filename, Shader** Result );
			static bool CreateFromSource( const std::string& Vertex, const std::string& Fragment, Shader** Result, std::map<std::string, std::string> Defines );

			void Bind( void );
			static void Unbind( void );

			// Shaders can't be API agnostic without preprocessing and/or a hackish soluton
			// so you'll need to bind differently for each API.
			#ifdef API_OPENGL
			
			void SetUniformMatrix( const std::string& Variable, const Math::Matrix4& Matrix );
			void SetUniformVector( const std::string& Variable, const Math::Vector3& Vector );
			void SetUniformVector( const std::string& Variable, const Math::Vector2& Vector );
			void SetUniformColor( const std::string& Variable, const Graphics::Color& Color );
			void SetUniformSampler( const std::string& Variable, Texture* Texture );
			void SetUniformSampler( const std::string& Variable, TextureTargets::Target Target );
			void SetUniformInt( const std::string& Variable, int Value );
			void SetUniformFloat( const std::string& Variable, float Value );

			#endif

		protected:
			ShaderImpl_t* Impl;
	};
}

#endif