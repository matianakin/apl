#pragma once

#ifdef FUNCTIONS_EXPORTS
#define FUNCTIONS_API __declspec(dllexport)
#else
#define FUNCTIONS_API __declspec(dllimport)
#endif

/**
 * @brief The Answer to the Ultimate Question of Life, the Universe, and Everything.
 */
extern "C" FUNCTIONS_API int answer();