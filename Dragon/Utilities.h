#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <fstream>
#include <sstream>
#include <assert.h>

void LoadTextFromFile(const char* path, std::string& data){
	std::ifstream file(path);
	assert(file.is_open());
	data = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

#endif // __UTILITIES_H