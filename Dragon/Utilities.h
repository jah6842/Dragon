#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <Windows.h>
#include <fstream>
#include <sstream>
#include <assert.h>

void dgnLoadTextFromFile(const char* path, std::string& data);

double dgnGetHighResTimestamp();


#endif // __UTILITIES_H