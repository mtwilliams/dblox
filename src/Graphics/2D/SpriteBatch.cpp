#include "Shared/Platform.h"
#include "Shared/Helpers.h"
#include "Math/Matrix4.h"
#include "Graphics/2D/SpriteBatch.h"

#include <GL/glew.h>

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <gl/GL.h>
#endif

namespace Graphics
{
	static const unsigned int VerticesPerBatch = 2048;

	typedef struct
	{
		float X, Y, Z;
		unsigned char R, G, B, A;
		float U, V;
	} SpriteBatchVertex_t;

	struct SpriteBatchImpl_t
	{
		// Agnostic
		Texture* Bound;
		unsigned int Vertex;
		SpriteBatchVertex_t Vertices[VerticesPerBatch];
	};

	SpriteBatch::SpriteBatch( void )
	{
		Impl = new SpriteBatchImpl_t();
		Impl->Vertex = 0;
		Impl->Bound = 0;

		for( unsigned int i = 0; i < VerticesPerBatch; ++i )
		{
			Impl->Vertices[i].X = 0.0f;
			Impl->Vertices[i].Y = 0.0f;
			Impl->Vertices[i].Z = 0.0f;
			Impl->Vertices[i].U = 0.0f;
			Impl->Vertices[i].V = 0.0f;
			Impl->Vertices[i].R = 255;
			Impl->Vertices[i].G = 255;
			Impl->Vertices[i].B = 255;
			Impl->Vertices[i].A = 255;
		}
	}

	SpriteBatch::~SpriteBatch()
	{
		SAFE_DELETE(Impl);
	}

	void SpriteBatch::Begin( unsigned int Width, unsigned int Height )
	{
		// Setup orthographic projection
		Math::Matrix4 ProjMatrix;
		ProjMatrix.Ortho(0.0f, (float)Width, (float)Height, 0.0f, -1.0f, 1.0f);
		ProjMatrix.Load(Math::MatrixType::Projection);
		Math::Matrix4::Identity.Load(Math::MatrixType::ModelView);

		// Disable depth writing
		glDisable(GL_DEPTH_TEST);

		// Normal blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void SpriteBatch::Flush( void )
	{
		if( Impl->Vertices <= 0 )
			return;

		glEnableClientState(GL_VERTEX_ARRAY);

		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glTexCoordPointer(2, GL_FLOAT, sizeof(SpriteBatchVertex_t), &Impl->Vertices[0].U);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SpriteBatchVertex_t), &Impl->Vertices[0].R);
		glVertexPointer(3, GL_FLOAT, sizeof(SpriteBatchVertex_t), &Impl->Vertices[0].X);

		glDrawArrays(GL_QUADS, 0, Impl->Vertex);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glFlush();

		Impl->Vertex = 0;
	}

	void SpriteBatch::End( void )
	{
		Flush();

		// Enable depth writing
		glEnable(GL_DEPTH_TEST);
	}

	void SpriteBatch::AddQuad( Texture* Texture, const Vector2& Min, const Vector2& Max, float Depth, const Vector2& U, const Vector2& V, const Color& Color )
	{
		if( Texture != Impl->Bound )
		{
			Flush();

			if( Texture )
			{
				Texture->Bind(TextureTargets::Texture0);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		AddVertex(Min, Depth, U.X, 1.0f - V.X, Color);
		AddVertex(Vector2(Min.X, Max.Y), Depth, U.X, 1.0f - V.Y, Color);
		AddVertex(Vector2(Max.X, Max.Y), Depth, U.Y, 1.0f - V.Y, Color);
		AddVertex(Vector2(Max.X, Min.Y), Depth, U.Y, 1.0f - V.X, Color);
	}

	void SpriteBatch::AddVertex( const Vector2& Position, float Depth, float U, float V, const Color& Color )
	{
		if( Impl->Vertex + 1 >= VerticesPerBatch )
			Flush();

		Impl->Vertices[Impl->Vertex].X = Position.X;
		Impl->Vertices[Impl->Vertex].Y = Position.Y;
		Impl->Vertices[Impl->Vertex].Z = Depth;
		Impl->Vertices[Impl->Vertex].U = U;
		Impl->Vertices[Impl->Vertex].V = V;
		*((unsigned int*)&Impl->Vertices[Impl->Vertex].R) = *((unsigned int*)&Color.R);

		++Impl->Vertex;
	}
}
