#ifndef DBLOX_SHARED_PLATFORM_H
#define DBLOX_SHARED_PLATFORM_H

#if defined(_WIN32) && (!defined(linux) || !defined(__linux))
	#define PLATFORM_WINDOWS
#endif

#ifndef API_OPENGL
	#define API_OPENGL
#endif

#endif