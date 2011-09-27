#include "Shared/JSON.h"
#include "Shared/Helpers.h"
#include "Graphics/Shader.h"

#include <GL/glew.h>

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <gl/GL.h>
#endif

#include <sstream>

namespace Graphics
{
	struct ShaderImpl_t
	{
		// OpenGL
		unsigned int Program;
		unsigned int Vertex;
		unsigned int Fragment;
	};

	Shader::Shader( void )
	{
		Impl = new ShaderImpl_t();
		Impl->Program = glCreateProgram();
		Impl->Vertex = glCreateShader(GL_VERTEX_SHADER);
		Impl->Fragment = glCreateShader(GL_FRAGMENT_SHADER);
	}

	Shader::~Shader()
	{
		glDeleteProgram(Impl->Program);
		glDeleteShader(Impl->Vertex);
		glDeleteShader(Impl->Fragment);

		SAFE_DELETE(Impl);
	}

	bool Shader::CreateFromFX( const std::string& Filename, Shader** Result )
	{
		JSONValue* Value = 0;
		JSONObject Root;
		bool Return = false;

		FILE* File = fopen(Filename.c_str(), "r");

		if( !File )
		{
			delete *Result;
			*Result = 0;
			return false;
		}

		fseek(File, 0, SEEK_END);
		unsigned int Size = ftell(File);
		fseek(File, 0, SEEK_SET);

		if( Size <= 0 )
			goto __Failed;

		char* Buffer = new char[Size + 1];
		Size = fread(Buffer, sizeof(char), Size, File);
		Buffer[Size] = '\0';
		fclose(File);

		Value = JSON::Parse(Buffer);
		delete[] Buffer;

		if( !Value )
			goto __Failed;

		if( !Value->IsObject()  )
		{
			goto __JSONFail;
		}

		Root = Value->AsObject();

		if( Root.find(L"GLSL") != Root.end() && Root[L"GLSL"]->IsObject() )
		{
			JSONObject GLSL = Root[L"GLSL"]->AsObject();

			std::wstring VertexW = GLSL[L"Vertex"]->AsString();
			std::wstring FragmentW = GLSL[L"Fragment"]->AsString();
			std::string Vertex, Fragment;

			Vertex.assign(VertexW.begin(), VertexW.end());
			Fragment.assign(FragmentW.begin(), FragmentW.end());

			std::map<std::string, std::string> Defines;

			if( Root.find(L"Defines") != Root.end() && Root[L"Defines"]->IsObject() )
			{
				JSONObject FXDefines = Root[L"Defines"]->AsObject();

				for( JSONObject::iterator iter = FXDefines.begin(); iter != FXDefines.end(); ++iter )
				{
					std::wstring DefineNameW = iter->first;
					std::string DefineName;
					DefineName.assign(DefineNameW.begin(), DefineNameW.end());

					std::wstring DefineValueW = iter->second->AsString();
					std::string DefineValue;
					DefineValue.assign(DefineValueW.begin(), DefineValueW.end());

					Defines.insert(std::make_pair(DefineName, DefineValue));
				}
			}

			Return = CreateFromSource(Vertex, Fragment, Result, Defines);
		}

		delete Value;
		return Return;

__JSONFail:
		delete Value;
__Failed:
		delete *Result;
		*Result = 0;
		fclose(File);
		return false;
	}

	bool Check( unsigned int Obj )
	{
		int Length = 0;
		char InfoLog[1024];

		if(glIsShader(Obj))
			glGetShaderInfoLog(Obj, 1024, &Length, InfoLog);
		else
			glGetProgramInfoLog(Obj, 1024, &Length, InfoLog);

		if(Length > 0)
			printf("OpenGL/GLSL Error: %s\n", &InfoLog);

		return !(Length > 0);
	}

	bool Shader::CreateFromSource( const std::string& Vertex, const std::string& Fragment, Shader** Result, std::map<std::string, std::string> Defines )
	{
		*Result = new Shader();

		int Status = GL_TRUE;
		char* Buffer = 0;
		unsigned int Size = 0;

		printf("Shader::CreateFromSource(&s, %s)\n", Vertex.c_str(), Fragment.c_str());

		std::stringstream Stream = std::stringstream(std::stringstream::in | std::stringstream::out);
		Stream << "#version 120\n";

		for( std::map<std::string, std::string>::iterator iter = Defines.begin(); iter != Defines.end(); ++iter )
		{
			Stream << "#define " << iter->first << " " << iter->second << "\n";
		}

		Stream << "\n\n";

		printf("Prefix:\n%s", Stream.str().c_str());

		std::string Source = Stream.str();
		const char* CSource = Source.c_str();
		const char* Sources[2] = { CSource, 0 };

		FILE* File = fopen(Vertex.c_str(), "r");

		if( !File )
		{
			delete *Result;
			*Result = 0;
			return false;
		}

		fseek(File, 0, SEEK_END);
		Size = ftell(File);
		fseek(File, 0, SEEK_SET);

		if( Size <= 0 )
			goto __Failed;

		Buffer = new char[Size + 1];
		fread(Buffer, sizeof(char), Size, File);
		Buffer[Size] = '\0';
		fclose(File);

		Sources[1] = Buffer;
		glShaderSource((*Result)->Impl->Vertex, 2, Sources, NULL);
		glCompileShader((*Result)->Impl->Vertex);

		delete[] Buffer;

		Status = GL_TRUE;
		glGetShaderiv((*Result)->Impl->Vertex, GL_COMPILE_STATUS, &Status);

		if(!Status)
		{
			if(!Check((*Result)->Impl->Vertex))
				return false;
		}

		File = fopen(Fragment.c_str(), "r");

		if( !File )
		{
			delete *Result;
			*Result = 0;
			return false;
		}

		fseek(File, 0, SEEK_END);
		Size = ftell(File);
		fseek(File, 0, SEEK_SET);

		if( Size <= 0 )
			goto __Failed;

		Buffer = new char[Size + 1];
		fread(Buffer, sizeof(char), Size, File);
		Buffer[Size] = '\0';
		fclose(File);

		Sources[1] = Buffer;
		glShaderSource((*Result)->Impl->Fragment, 2, Sources, NULL);
		glCompileShader((*Result)->Impl->Fragment);

		delete[] Buffer;

		Status = GL_TRUE;
		glGetShaderiv((*Result)->Impl->Fragment, GL_COMPILE_STATUS, &Status);

		if(!Status)
		{
			if(!Check((*Result)->Impl->Fragment))
				return false;
		}

		glAttachShader((*Result)->Impl->Program, (*Result)->Impl->Vertex);
		glAttachShader((*Result)->Impl->Program, (*Result)->Impl->Fragment);
		glLinkProgram((*Result)->Impl->Program);

		Status = GL_TRUE;
		glGetProgramiv((*Result)->Impl->Program, GL_LINK_STATUS, &Status);
				
		if(!Status)
		{
			if(!Check((*Result)->Impl->Program))
				return false;
		}

		return true;

__Failed:
		delete *Result;
		*Result = 0;
		fclose(File);
		return false;
	}

	void Shader::Bind( void )
	{
		glUseProgram(Impl->Program);
	}

	void Shader::Unbind( void )
	{
		glUseProgram(0);
	}

	void Shader::SetUniformMatrix( const std::string& Variable, const Math::Matrix4& Matrix )
	{
		int Location = glGetUniformLocation(Impl->Program, Variable.c_str());
		glUniformMatrix4fv(Location, 1, GL_TRUE, (const float*)Matrix);
	}

	void Shader::SetUniformVector( const std::string& Variable, const Math::Vector3& Vector )
	{
		int Location = glGetUniformLocation(Impl->Program, Variable.c_str());
		glUniform3f(Location, Vector.X, Vector.Y, Vector.Z);
	}

	void Shader::SetUniformVector( const std::string& Variable, const Math::Vector2& Vector )
	{
		int Location = glGetUniformLocation(Impl->Program, Variable.c_str());
		glUniform2f(Location, Vector.X, Vector.Y);
	}

	void Shader::SetUniformColor( const std::string& Variable, const Graphics::Color& Color )
	{
		int Location = glGetUniformLocation(Impl->Program, Variable.c_str());
		glUniform4f(Location, Color.R, Color.G, Color.B, Color.A);
	}

	void Shader::SetUniformSampler( const std::string& Variable, Texture* Texture )
	{
		SetUniformSampler(Variable, Texture->BoundTo());
	}

	void Shader::SetUniformSampler( const std::string& Variable, TextureTargets::Target Target )
	{
		int Location = glGetUniformLocation(Impl->Program, Variable.c_str());
		int TextureUnit = 0;

		switch( Target )
		{
			default: TextureUnit = 0; break;

			case TextureTargets::Texture0:
			case TextureTargets::Texture1:
			case TextureTargets::Texture2:
			case TextureTargets::Texture3:
			case TextureTargets::Texture4:
			case TextureTargets::Texture5:
			case TextureTargets::Texture6:
			case TextureTargets::Texture7:
				TextureUnit = Target - TextureTargets::Texture0;
			break;
		}

		glUniform1i(Location, TextureUnit);
	}

	void Shader::SetUniformInt( const std::string& Variable, int Value )
	{
		int Location = glGetUniformLocation(Impl->Program, Variable.c_str());
		glUniform1i(Location, Value);
	}

	void Shader::SetUniformFloat( const std::string& Variable, float Value )
	{
		int Location = glGetUniformLocation(Impl->Program, Variable.c_str());
		glUniform1f(Location, Value);
	}
}
