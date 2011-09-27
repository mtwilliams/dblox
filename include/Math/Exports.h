/*
	math.dll
	Copyright (c) 2011 Michael Williams
	See license in License.txt
*/

#ifndef DBLOX_MATH_EXPORTS_H
#define DBLOX_MATH_EXPORTS_H
#pragma once

#if defined( DBLOX_MATH_COMPILE )

	#ifdef __GNUC__
		#define DBLOX_MATH_EXPORT __attribute__((dllexport))
	#else
		#define DBLOX_MATH_EXPORT __declspec(dllexport)
	#endif

	#define NDBLOX_MATH_TEMPLATE_EXPORT
#else 
	#ifdef __GNUC__
		#define DBLOX_MATH_EXPORT __attribute__((dllimport))
	#else
		#define DBLOX_MATH_EXPORT __declspec(dllimport)
	#endif

	#define DBLOX_MATH_TEMPLATE_EXPORT extern
#endif

#endif
