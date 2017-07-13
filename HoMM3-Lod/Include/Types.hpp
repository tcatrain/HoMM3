#pragma once

typedef unsigned char byte;
typedef unsigned short int usint;
typedef unsigned int uint;

#ifdef HoMM3_Lod_EXPORTS
#  define HOMM3_LOD_LIB __declspec(dllexport)
#else
#  define HOMM3_LOD_LIB __declspec(dllimport)
#endif // HoMM3_Lod_EXPORTS
