#include "Utilities.h"

void dgnLoadTextFromFile(const char* path, std::string& data){
	std::ifstream file(path);
	assert(file.is_open());
	data = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

double dgnGetHighResTimestamp(){
	LARGE_INTEGER nowTime, Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&nowTime);

	//double secondsElapsed = static_cast<double>(ElapsedMicroseconds.QuadPart) / 1000000.0L;
	double secondsElapsed = static_cast<double>(nowTime.QuadPart) / static_cast<double>(Frequency.QuadPart);

	return secondsElapsed;
}