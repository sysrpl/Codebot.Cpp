#pragma once
#ifndef ThisMethod

// Targets are MacOS, LinuxOS, and WindowsOS

#ifdef TARGET_OS_MAC
#define MacOS
#elif defined __linux__
#define LinuxOS
#elif defined _WIN32 || defined _WIN64
#define WindowsOS
#endif

// this will create: main.cpp(47): Reminder: Fix this problem!
// usage: #pragma message(Reminder "Fix this problem!")

#define Stringize( L ) #L
#define MakeString( M, L ) M( L )
#define $Line  MakeString( Stringize, __LINE__ )
#define Reminder __FILE__ "(" $Line ") : Reminder: "

#ifdef _MSC_VER
#define MSVC
#elif defined __GNUC__
#define GCC
#endif

// MSVC and GCC Differences

#ifdef MSVC
#define ThisMethod __FUNCSIG__
#define import __declspec(dllimport)
#define threadvar __declspec(thread)
#define cdecl __cdecl
#define stdcall __stdcall
#define fastcall __fastcall

#elif defined GCC
#define ThisMethod __PRETTY_FUNCTION__
#define import
#define threadvar __thread
#define stdcall __attribute__((__stdcall__))
#define cdecl __attribute__((__cdecl__))
#endif

#endif
