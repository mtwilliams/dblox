#include "Shared/Platform.h"
#include "Shared/Helpers.h"
#include "Graphics/RenderTarget.h"

#include <GL/glew.h>

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <gl/GL.h>
#endif

namespace Graphics
{
	struct RenderTargetImpl_t
	{
		// General
		unsigned int Width;
		unsigned int Height;

		// Agnostic
		Texture* Depth;

		// OpenGL
		unsigned int ID;
	};
	
	// Purpose: Constructor
	// Format: Determines the format of the color buffer.
	// Depth: If true, creates and binds a depth buffer.
	RenderTarget::RenderTarget( unsigned int Width, unsigned int Height, bool Depth )
	{
		Impl = new RenderTargetImpl_t();
		Impl->Width = Width;
		Impl->Height = Height;
		Impl->Depth = Depth ? new Texture(Width, Height, Formats::D24, false) : 0;
		
		glGenFramebuffers(1, &Impl->ID);
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, Impl->ID);
		
		if( Depth )
			Impl->Depth->Bind(TextureTargets::DepthBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	}

	RenderTarget::~RenderTarget()
	{
		SAFE_DELETE(Impl->Depth);
		
		glDeleteFramebuffers(1, &Impl->ID);

		SAFE_DELETE(Impl);
	}

	void RenderTarget::Bind( TextureTargets::Target Target )
	{
		static const unsigned int Buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT, GL_COLOR_ATTACHMENT6_EXT, GL_COLOR_ATTACHMENT7_EXT };

		if( Target >= TextureTargets::ColorBuffer0 && Target <= TextureTargets::ColorBuffer7 )
		{
			glPushAttrib(GL_VIEWPORT_BIT);
			glViewport(0, 0, Impl->Width, Impl->Height);

			if( Impl->Depth )
				glEnable(GL_DEPTH);
			else
				glDisable(GL_DEPTH);

			glBindFramebuffer(GL_FRAMEBUFFER_EXT, Impl->ID);
			glDrawBuffers(Target - TextureTargets::ColorBuffer0 + 1, Buffers);
		}
	}

	void RenderTarget::Unbind( void )
	{
		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		
		glPopAttrib();
	}
			
	unsigned int RenderTarget::GetWidth( void )
	{
		return Impl->Width;
	}

	unsigned int RenderTarget::GetHeight( void )
	{
		return Impl->Height;
	}

	Texture* RenderTarget::GetDepthBuffer( void )
	{
		return Impl->Depth;
	}
}
