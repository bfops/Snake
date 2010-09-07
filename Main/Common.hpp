#pragma once

#define countof(array) (sizeof(array) / sizeof(array[0]))
#define nullptr NULL

void DebugOutput(const char* format, ...);
