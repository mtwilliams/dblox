/*
	graphics.dll
	Copyright (c) 2011 Michael Williams
	See license in License.txt
*/

#ifndef DBLOX_GRAPHICS_EXPORTS_H
#define DBLOX_GRAPHICS_EXPORTS_H
#pragma once

#if defined( DBLOX_GRAPHICS_COMPILE )

	#ifdef __GNUC__
		#define DBLOX_GRAPHICS_EXPORT __attribute__((dllexport))
	#else
		#define DBLOX_GRAPHICS_EXPORT __declspec(dllexport)
	#endif

	#define NDBLOX_GRAPHICS_TEMPLATE_EXPORT
#else 
	#ifdef __GNUC__
		#define DBLOX_GRAPHICS_EXPORT __attribute__((dllimport))
	#else
		#define DBLOX_GRAPHICS_EXPORT __declspec(dllimport)
	#endif

	#define DBLOX_GRAPHICS_TEMPLATE_EXPORT extern
#endif

#endif
