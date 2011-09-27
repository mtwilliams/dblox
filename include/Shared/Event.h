#ifndef SHARED_EVENT_H
#define SHARED_EVENT_H

namespace Events
{
	enum Type
	{
		Closed = 0,
		Resized,
		KeyPressed,
		KeyReleased,
		MousePressed,
		MouseReleased,
		MouseMoved,
		Invalid
	};
}

namespace Keys
{
	enum Key
	{
		A = 0,
		B,            
		C,            
		D,            
		E,            
		F,            
		G,            
		H,            
		I,            
		J,            
		K,            
		L,            
		M,            
		N,            
		O,            
		P,            
		Q,            
		R,            
		S,            
		T,            
		U,            
		V,            
		W,            
		X,            
		Y,            
		Z,            
		Num0,         
		Num1,         
		Num2,         
		Num3,         
		Num4,         
		Num5,         
		Num6,         
		Num7,         
		Num8,         
		Num9,         
		Escape,       
		LControl,     
		LShift,       
		LAlt,         
		LSystem,
		RControl,     
		RShift,       
		RAlt,         
		RSystem,
		Menu,         
		LBracket,     
		RBracket,     
		SemiColon,    
		Comma,        
		Period,       
		Quote,        
		Slash,        
		BackSlash,    
		Tilde,        
		Equal,        
		Dash,         
		Space,        
		Return,       
		Back,         
		Tab,          
		PageUp,       
		PageDown,     
		End,          
		Home,         
		Insert,       
		Delete,       
		Add,
		Subtract,
		Multiply,
		Divide,
		Left,
		Right,
		Up,
		Down,
		Numpad0,      
		Numpad1,      
		Numpad2,      
		Numpad3,      
		Numpad4,      
		Numpad5,      
		Numpad6,      
		Numpad7,      
		Numpad8,      
		Numpad9,      
		F1,           
		F2,           
		F3,           
		F4,           
		F5,           
		F6,           
		F7,           
		F8,           
		F9,           
		F10,          
		F11,          
		F12,          
		F13,          
		F14,
		F15,
		Pause,
		Invalid
	};
}

namespace MouseButtons
{
	enum Button
	{
		Left = 0,
		Middle,
		Right,
		Invalid
	};
}

typedef struct
{
	Keys::Key Key;
} KeyPressedEvent_t;

typedef struct
{
	Keys::Key Key;
} KeyReleasedEvent_t;

typedef struct
{
	unsigned int X;
	unsigned int Y;
	MouseButtons::Button Button;
} MousePressedEvent_t;

typedef struct
{
	unsigned int X;
	unsigned int Y;
	MouseButtons::Button Button;
} MouseReleasedEvent_t;

typedef struct
{
	unsigned int X;
	unsigned int Y;
} MouseMovedEvent_t;

typedef struct
{
	unsigned int Width;
	unsigned int Height;
} ResizedEvent_t;

typedef struct
{
	Events::Type Type;

	union
	{
		ResizedEvent_t Resized;
		KeyPressedEvent_t KeyPressed;
		KeyReleasedEvent_t KeyReleased;
		MousePressedEvent_t MousePressed;
		MouseReleasedEvent_t MouseReleased;
		MouseMovedEvent_t MouseMoved;
	} Event;
} Event_t;

#endif
