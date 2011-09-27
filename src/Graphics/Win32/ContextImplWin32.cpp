#include "Shared/Platform.h"
#include "Shared/Helpers.h"
#include "Graphics/Context.h"

#include <GL/glew.h>

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <GL/wglew.h>
	#include <gl/GL.h>
#endif

namespace Graphics
{
	struct ContextImpl_t
	{
	#ifdef PLATFORM_WINDOWS
		// OpenGL
		HWND hWnd;
		HDC hDC;
		HGLRC hRC;

		// Timing
		LARGE_INTEGER HighPreformanceFreq;
		LARGE_INTEGER HighPreformanceStart;
		DWORD LowPreformanceStart;
		BOOL UseHighPreformance;
	#endif

		// Agnostic
		unsigned int VideoMode;
		bool IsOpen;
	};

	Context::Context( void )
	{
		Impl = new ContextImpl_t();
	}

	Context::~Context()
	{
		#ifdef PLATFORM_WINDOWS
		if(Impl->hRC)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(Impl->hRC);
			ReleaseDC(Impl->hWnd, Impl->hDC);
			DestroyWindow(Impl->hWnd);
			UnregisterClass("dblox", GetModuleHandle(NULL));
		}
		#endif

		SAFE_DELETE(Impl);
	}

#ifdef PLATFORM_WINDOWS
	LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	LRESULT CALLBACK DummyWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch( uMsg )
		{
			//case WM_CREATE: return -1; break;
			case WM_DESTROY: PostQuitMessage(0); return 0; break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	typedef struct
	{
		HWND hWnd;
		HDC hDC;
		HGLRC hRC;

		WNDCLASSEX WndClassEx;
	} DummyContextInfo_t;

	bool CreateDummyContext( DummyContextInfo_t& DummyContextInfo )
	{
		memset((void*)(&DummyContextInfo.WndClassEx), 0, sizeof(WNDCLASSEX));
		DummyContextInfo.WndClassEx.cbSize = sizeof(WNDCLASSEX);
		DummyContextInfo.WndClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		DummyContextInfo.WndClassEx.lpfnWndProc = DummyWndProc;
		DummyContextInfo.WndClassEx.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		DummyContextInfo.WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
		DummyContextInfo.WndClassEx.hInstance = (HINSTANCE)GetModuleHandle(0);
		DummyContextInfo.WndClassEx.lpszClassName = "DBLOX_DUMMY";

		if( !RegisterClassEx(&DummyContextInfo.WndClassEx) )
            return false;

        DummyContextInfo.hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, "DBLOX_DUMMY", "",
											   WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
											   0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);

        if( !DummyContextInfo.hWnd )
			return false;

		PIXELFORMATDESCRIPTOR PFD = { 0, };

        PFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        PFD.nVersion = 1;
        PFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
        PFD.iPixelType = PFD_TYPE_RGBA;
        PFD.cColorBits = 24;
        PFD.cDepthBits = 24;
        PFD.iLayerType = PFD_MAIN_PLANE;

		if( !(DummyContextInfo.hDC = GetDC(DummyContextInfo.hWnd)) )
			return false;
		
        int PixelFormat = ChoosePixelFormat(DummyContextInfo.hDC, &PFD);
        if( !SetPixelFormat(DummyContextInfo.hDC, PixelFormat, &PFD) )
            return false;

        if( !(DummyContextInfo.hRC = wglCreateContext(DummyContextInfo.hDC)) )
            return false;

        if( !wglMakeCurrent(DummyContextInfo.hDC, DummyContextInfo.hRC) )
            return false;

        return true;
	}

	void DestroyDummyContext( DummyContextInfo_t& DummyContextInfo )
	{
        if( DummyContextInfo.hRC )
        {
            wglMakeCurrent(NULL, NULL);
			wglDeleteContext(DummyContextInfo.hRC);
			ReleaseDC(DummyContextInfo.hWnd, DummyContextInfo.hDC);
			DestroyWindow(DummyContextInfo.hWnd);
			UnregisterClass("DBLOX_DUMMY", GetModuleHandle(NULL));

			MSG Msg;
			while( PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) )
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}

			DummyContextInfo.hRC = 0;
			DummyContextInfo.hDC = 0;
			DummyContextInfo.hWnd = 0;
        }
	}

	int ChoosePixelFormatForVideoMode( DummyContextInfo_t& DummyContextInfo, unsigned int VideoMode )
    {
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		if( !wglChoosePixelFormatARB )
			return 0;

		/*PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

		if( !wglCreateContextAttribsARB )
			return 0;

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		if( !wglSwapIntervalEXT )
			return 0;*/

        int Attribs [] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,     24,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     24,
            WGL_STENCIL_BITS_ARB,   8,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB,        Graphics::VideoModes[VideoMode].MSAA,
            0, 0
        };

        int PixelFormat = 0;
        UINT NumFormats = 0;
        BOOL Status = wglChoosePixelFormatARB(DummyContextInfo.hDC, Attribs, 0, 1, &PixelFormat, &NumFormats);

        if( Status == TRUE && NumFormats )
			return PixelFormat;

		return 0;
    }

	bool Context::Create( const std::string& Title, unsigned int VideoMode, Context** Result )
	{
		*Result = new Context();
		(*Result)->Impl->VideoMode = VideoMode;

		WNDCLASS WC;
		WC.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		WC.lpfnWndProc		= WndProc;
		WC.cbClsExtra		= 0;
		WC.cbWndExtra		= 0;
		WC.hInstance		= GetModuleHandle(NULL);
		WC.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
		WC.hCursor			= LoadCursor(NULL, IDC_ARROW);
		WC.hbrBackground	= NULL;
		WC.lpszMenuName		= NULL;
		WC.lpszClassName	= "dblox";

		RECT ClientArea = { 0, 0, Graphics::VideoModes[VideoMode].Width - 1, Graphics::VideoModes[VideoMode].Height - 1 };
		AdjustWindowRectEx(&ClientArea, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		ClientArea.right = ClientArea.right - ClientArea.left + 1;
		ClientArea.bottom = ClientArea.bottom - ClientArea.top + 1;

		// Use a dummy window/context to get a MSAA'd format.
		DummyContextInfo_t DummyContextInfo;
		if( !CreateDummyContext(DummyContextInfo) )
		{
			DestroyDummyContext(DummyContextInfo);
			goto __WinFail;
		}

		GLenum Error = glewInit();
		if( Error != GLEW_OK )
			goto __WinFail;

		int PixelFormat = ChoosePixelFormatForVideoMode(DummyContextInfo, VideoMode);
		DestroyDummyContext(DummyContextInfo);

		static PIXELFORMATDESCRIPTOR PFD =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,
			8,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		if( !RegisterClass(&WC) )
			goto __WinFail;

		// TODO: Fullscreen support
		if( Graphics::VideoModes[VideoMode].Fullscreen )
			goto __WinFail;

		if ( ((*Result)->Impl->hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
										WC.lpszClassName, Title.c_str(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
										0, 0, ClientArea.right, ClientArea.bottom, NULL, NULL, GetModuleHandle(NULL), NULL)) == 0)
		{
			goto __WinFail;
		}

		if( !((*Result)->Impl->hDC = GetDC((*Result)->Impl->hWnd)) )
			goto __WinFail;

		// Fallback to non-MSAA'd.
		if( !PixelFormat )
		{
			if( !(PixelFormat = ChoosePixelFormat((*Result)->Impl->hDC, &PFD)) )
				goto __WinFail;
		}

		if( !SetPixelFormat((*Result)->Impl->hDC, PixelFormat, &PFD) )
			goto __WinFail;

		if( !((*Result)->Impl->hRC = wglCreateContext((*Result)->Impl->hDC)) )
			goto __WinFail;

		wglMakeCurrent((*Result)->Impl->hDC, (*Result)->Impl->hRC);

		Error = glewInit();
		if( Error != GLEW_OK )
			goto __WinFail;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		//glDisable(GL_CULL_FACE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);

		RECT Rect;
		GetWindowRect((*Result)->Impl->hWnd, &Rect);
		SetWindowPos((*Result)->Impl->hWnd, 0, (GetSystemMetrics(SM_CXSCREEN) - Rect.right) / 2, (GetSystemMetrics(SM_CYSCREEN) - Rect.bottom) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		ShowWindow((*Result)->Impl->hWnd, SW_SHOW);
		SetForegroundWindow((*Result)->Impl->hWnd);
		SetFocus((*Result)->Impl->hWnd);

		(*Result)->Impl->IsOpen = true;

		(*Result)->Impl->LowPreformanceStart = ::GetTickCount();
		(*Result)->Impl->UseHighPreformance = QueryPerformanceFrequency(&(*Result)->Impl->HighPreformanceFreq);
		QueryPerformanceCounter(&(*Result)->Impl->HighPreformanceStart);

		freopen("stdout.txt", "w", stdout);

		return true;
__WinFail:
		delete (*Result);
		return false;
	}

#endif

	void Context::Bind( void )
	{
		#ifdef PLATFORM_WINDOWS
		wglMakeCurrent(Impl->hDC, Impl->hRC);
		#endif
	}

	void Context::Unbind( void )
	{
		#ifdef PLATFORM_WINDOWS
		wglMakeCurrent(NULL, NULL);
		#endif
	}

	bool Context::IsOpen( void )
	{
		return Impl->IsOpen;
	}

#ifdef PLATFORM_WINDOWS
	Keys::Key TranslateKeycode( WPARAM wParam, LPARAM lParam )
	{
		switch(wParam)
		{
			case VK_SHIFT:
			{
				static UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
				UINT scancode = (UINT)((lParam & (0xFF << 16)) >> 16);
				return scancode == lShift ? Keys::LShift : Keys::RShift;
			} break;

			case VK_MENU: return (HIWORD(lParam) & KF_EXTENDED) ? Keys::RAlt : Keys::LAlt; break;
			case VK_CONTROL: return (HIWORD(lParam) & KF_EXTENDED) ? Keys::RControl : Keys::LControl; break;
			case VK_LWIN:       return Keys::LSystem; break;
			case VK_RWIN:       return Keys::RSystem; break;
			case VK_APPS:       return Keys::Menu; break;
			case VK_OEM_1:      return Keys::SemiColon; break;
			case VK_OEM_2:      return Keys::Slash; break;
			case VK_OEM_PLUS:   return Keys::Equal; break;
			case VK_OEM_MINUS:  return Keys::Dash; break;
			case VK_OEM_4:      return Keys::LBracket; break;
			case VK_OEM_6:      return Keys::RBracket; break;
			case VK_OEM_COMMA:  return Keys::Comma; break;
			case VK_OEM_PERIOD: return Keys::Period; break;
			case VK_OEM_7:      return Keys::Quote; break;
			case VK_OEM_5:      return Keys::BackSlash; break;
			case VK_OEM_3:      return Keys::Tilde; break;
			case VK_ESCAPE:     return Keys::Escape; break;
			case VK_SPACE:      return Keys::Space; break;
			case VK_RETURN:     return Keys::Return; break;
			case VK_BACK:       return Keys::Back; break;
			case VK_TAB:        return Keys::Tab; break;
			case VK_PRIOR:      return Keys::PageUp; break;
			case VK_NEXT:       return Keys::PageDown; break;
			case VK_END:        return Keys::End; break;
			case VK_HOME:       return Keys::Home; break;
			case VK_INSERT:     return Keys::Insert; break;
			case VK_DELETE:     return Keys::Delete; break;
			case VK_ADD:        return Keys::Add; break;
			case VK_SUBTRACT:   return Keys::Subtract; break;
			case VK_MULTIPLY:   return Keys::Multiply; break;
			case VK_DIVIDE:     return Keys::Divide; break;
			case VK_PAUSE:      return Keys::Pause; break;
			case VK_F1:         return Keys::F1; break;
			case VK_F2:         return Keys::F2; break;
			case VK_F3:         return Keys::F3; break;
			case VK_F4:         return Keys::F4; break;
			case VK_F5:         return Keys::F5; break;
			case VK_F6:         return Keys::F6; break;
			case VK_F7:         return Keys::F7; break;
			case VK_F8:         return Keys::F8; break;
			case VK_F9:         return Keys::F9; break;
			case VK_F10:        return Keys::F10; break;
			case VK_F11:        return Keys::F11; break;
			case VK_F12:        return Keys::F12; break;
			case VK_F13:        return Keys::F13; break;
			case VK_F14:        return Keys::F14; break;
			case VK_F15:        return Keys::F15; break;
			case VK_LEFT:       return Keys::Left; break;
			case VK_RIGHT:      return Keys::Right; break;
			case VK_UP:         return Keys::Up; break;
			case VK_DOWN:       return Keys::Down; break;
			case VK_NUMPAD0:    return Keys::Numpad0; break;
			case VK_NUMPAD1:    return Keys::Numpad1; break;
			case VK_NUMPAD2:    return Keys::Numpad2; break;
			case VK_NUMPAD3:    return Keys::Numpad3; break;
			case VK_NUMPAD4:    return Keys::Numpad4; break;
			case VK_NUMPAD5:    return Keys::Numpad5; break;
			case VK_NUMPAD6:    return Keys::Numpad6; break;
			case VK_NUMPAD7:    return Keys::Numpad7; break;
			case VK_NUMPAD8:    return Keys::Numpad8; break;
			case VK_NUMPAD9:    return Keys::Numpad9; break;
			case 'A':           return Keys::A; break;
			case 'Z':           return Keys::Z; break;
			case 'E':           return Keys::E; break;
			case 'R':           return Keys::R; break;
			case 'T':           return Keys::T; break;
			case 'Y':           return Keys::Y; break;
			case 'U':           return Keys::U; break;
			case 'I':           return Keys::I; break;
			case 'O':           return Keys::O; break;
			case 'P':           return Keys::P; break;
			case 'Q':           return Keys::Q; break;
			case 'S':           return Keys::S; break;
			case 'D':           return Keys::D; break;
			case 'F':           return Keys::F; break;
			case 'G':           return Keys::G; break;
			case 'H':           return Keys::H; break;
			case 'J':           return Keys::J; break;
			case 'K':           return Keys::K; break;
			case 'L':           return Keys::L; break;
			case 'M':           return Keys::M; break;
			case 'W':           return Keys::W; break;
			case 'X':           return Keys::X; break;
			case 'C':           return Keys::C; break;
			case 'V':           return Keys::V; break;
			case 'B':           return Keys::B; break;
			case 'N':           return Keys::N; break;
			case '0':           return Keys::Num0; break;
			case '1':           return Keys::Num1; break;
			case '2':           return Keys::Num2; break;
			case '3':           return Keys::Num3; break;
			case '4':           return Keys::Num4; break;
			case '5':           return Keys::Num5; break;
			case '6':           return Keys::Num6; break;
			case '7':           return Keys::Num7; break;
			case '8':           return Keys::Num8; break;
			case '9':           return Keys::Num9; break;
		}

		return Keys::Invalid;
	}

	LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch(uMsg)
		{
			case WM_SYSCOMMAND:
			{
				switch (wParam)
				{
					case SC_SCREENSAVE:
					case SC_MONITORPOWER:
						return 0;
					break;
				}
			} break;

			case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			} break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool Context::GetEvent( Event_t& Event )
	{
		MSG Msg;
		if( PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) )
		{
			switch( Msg.message )
			{
				case WM_SIZE:
				{
					Event.Type = Events::Resized;
					Event.Event.Resized.Width = LOWORD(Msg.lParam);
					Event.Event.Resized.Height = HIWORD(Msg.lParam);
				} break;

				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_SYSKEYUP:
				case WM_SYSKEYDOWN:
				{
					Event.Type = Msg.message == WM_KEYDOWN ? Events::KeyPressed : Events::KeyReleased;
					Event.Event.KeyPressed.Key = TranslateKeycode(Msg.wParam, Msg.lParam);
				} break;

				case WM_LBUTTONUP:
				case WM_LBUTTONDOWN:
				{
					Event.Type = Msg.message == WM_LBUTTONDOWN ? Events::MousePressed : Events::MouseReleased;
					Event.Event.MousePressed.Button = MouseButtons::Left;
					Event.Event.MousePressed.X = LOWORD(Msg.lParam);
					Event.Event.MousePressed.Y = HIWORD(Msg.lParam);
				} break;

				case WM_MBUTTONUP:
				case WM_MBUTTONDOWN:
				{
					Event.Type = Msg.message == WM_MBUTTONDOWN ? Events::MousePressed : Events::MouseReleased;
					Event.Event.MousePressed.Button = MouseButtons::Middle;
					Event.Event.MousePressed.X = LOWORD(Msg.lParam);
					Event.Event.MousePressed.Y = HIWORD(Msg.lParam);
				} break;

				case WM_RBUTTONUP:
				case WM_RBUTTONDOWN:
				{
					Event.Type = Msg.message == WM_RBUTTONDOWN ? Events::MousePressed : Events::MouseReleased;
					Event.Event.MousePressed.Button = MouseButtons::Right;
					Event.Event.MousePressed.X = LOWORD(Msg.lParam);
					Event.Event.MousePressed.Y = HIWORD(Msg.lParam);
				} break;

				case WM_MOUSEMOVE:
				{
					Event.Type = Events::MouseMoved;
					Event.Event.MouseMoved.X = LOWORD(Msg.lParam);
					Event.Event.MouseMoved.Y = HIWORD(Msg.lParam);
				} break;

				case WM_QUIT:
				{
					Impl->IsOpen = false;
					return false;
				} break;

				default: Event.Type = Events::Invalid; break;
			}

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			return true;
		}

		return false;
	}
#endif

	void Context::Clear( float Depth )
	{
		glClearDepth(Depth);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Context::Clear( const Color& Color, float Depth )
	{
		glClearColor(Color.R * 0.00392156863f, Color.G * 0.00392156863f, Color.B * 0.00392156863f, Color.A * 0.00392156863f);
		glClearDepth(Depth);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Context::Display( void )
	{
		#ifdef PLATFORM_WINDOWS
		SwapBuffers(Impl->hDC);
		#endif
	}

	void Context::Clip( const Math::Vector2& Min, const Math::Vector2& Max )
	{
		int X, Y, W, H;

		X = Min.X;
		Y = Min.Y;

		W = Max.X - Min.X;
		H = Max.Y - Max.Y;

		int View[4];
		glGetIntegerv(GL_VIEWPORT, &View[0]);
		Y = View[3] - (Y + H);

		glScissor(X, Y, W, H);
		glEnable(GL_SCISSOR_TEST);
	}

	void Context::Clip( void )
	{
		glDisable(GL_SCISSOR_TEST);
	}

	void Context::SetBlendingMode( BlendingModes::Mode Mode )
	{
		switch( Mode )
		{
			case BlendingModes::None: glBlendFunc(GL_ONE, GL_ZERO); break;
			case BlendingModes::Additive: glBlendFunc(GL_ONE, GL_ONE);  break;
			case BlendingModes::Normal: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
		}
	}

	void Context::SetDepthWrite( bool Toogle )
	{
		if( Toogle )
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	unsigned int Context::GetWidth( void )
	{
		return GetVideoMode().Width;
	}

	unsigned int Context::GetHeight( void )
	{
		return GetVideoMode().Height;
	}

	const VideoMode_t Context::GetVideoMode( void )
	{
		return Graphics::VideoModes[Impl->VideoMode];
	}

#ifdef PLATFORM_WINDOWS
	float Context::GetTickCount( void )
	{
		if( Impl->UseHighPreformance )
		{
			LARGE_INTEGER Current;
			QueryPerformanceCounter(&Current);
			return ((double)(Current.QuadPart - Impl->HighPreformanceStart.QuadPart) / (double)(Impl->HighPreformanceFreq.QuadPart * 0.001)) * 0.001;
		}
		else
		{
			return (float)((double)Impl->LowPreformanceStart - (double)GetTickCount()) / 1000.0f;
		}
	}
#endif
}
