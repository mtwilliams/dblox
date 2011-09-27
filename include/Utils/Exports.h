/*
	utils.dll
	Copyright (c) 2011 Michael Williams
	See license in License.txt
*/

#ifndef DBLOX_UTILS_EXPORTS_H
#define DBLOX_UTILS_EXPORTS_H
#pragma once

#if defined( DBLOX_UTILS_COMPILE )

	#ifdef __GNUC__
		#define DBLOX_UTILS_EXPORT __attribute__((dllexport))
	#else
		#define DBLOX_UTILS_EXPORT __declspec(dllexport)
	#endif

	#define NDBLOX_UTILS_TEMPLATE_EXPORT
#else 
	#ifdef __GNUC__
		#define DBLOX_UTILS_EXPORT __attribute__((dllimport))
	#else
		#define DBLOX_UTILS_EXPORT __declspec(dllimport)
	#endif

	#define DBLOX_UTILS_TEMPLATE_EXPORT extern
#endif

#endif
