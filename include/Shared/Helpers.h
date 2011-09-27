#ifndef DBLOX_SHARED_HELPERS_H
#define DBLOX_SHARED_HELPERS_H

#define SAFE_DELETE( p ) \
	{ if( p ) { delete p; p = 0; } }

#endif
