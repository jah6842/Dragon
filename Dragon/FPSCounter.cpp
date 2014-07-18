#include <cstring>
#include <algorithm>
#include "FPSCounter.h"

// Constructor
FPSCounter::FPSCounter(size_t numBufferedFrames){
	_frameBuffer = new uint64_t[numBufferedFrames];
	_frameBufferSize = numBufferedFrames;
	_currentBufferPosition = 0;
	_fps = 0;
}

// Destructor
FPSCounter::~FPSCounter(){
	delete[] _frameBuffer;
}

// Copy Constructor
FPSCounter::FPSCounter(const FPSCounter& other) : _frameBufferSize(other._frameBufferSize),
												_currentBufferPosition(other._currentBufferPosition),
												_frameBuffer(new uint64_t[_frameBufferSize]),
												_fps(0)
{
	//std::copy(other._frameBuffer, other._frameBuffer + _frameBufferSize, _frameBuffer); // unsafe?
	memcpy(_frameBuffer, other._frameBuffer, _frameBufferSize * sizeof(uint64_t));
}

// Copy Assignment
FPSCounter& FPSCounter::operator=(const FPSCounter& other){
	if (this != &other){
		delete[] _frameBuffer;

		_frameBufferSize = other._frameBufferSize;
		_frameBuffer = new uint64_t[_frameBufferSize];
		_currentBufferPosition = other._currentBufferPosition;
		_fps = other._fps;
		//std::copy(other._frameBuffer, other._frameBuffer + _frameBufferSize, _frameBuffer);
		memcpy(_frameBuffer, other._frameBuffer, _frameBufferSize * sizeof(uint64_t));
	}

	return *this;
}

// Move constructor
FPSCounter::FPSCounter(FPSCounter&& other) : _frameBufferSize(0), _currentBufferPosition(0), _fps(0), _frameBuffer(nullptr) {
	_frameBufferSize = other._frameBufferSize;
	_frameBuffer = other._frameBuffer;
	_currentBufferPosition = other._currentBufferPosition;
	_fps = other._fps;

	other._fps = 0;
	other._currentBufferPosition = 0;
	other._frameBuffer = nullptr;
	other._frameBufferSize = 0;
}

// Move assignment
FPSCounter& FPSCounter::operator=(FPSCounter&& other){
	if (this != &other){
		delete[] _frameBuffer;

		_frameBufferSize = other._frameBufferSize;
		_frameBuffer = other._frameBuffer;
		_currentBufferPosition = other._currentBufferPosition;
		_fps = other._fps;

		other._fps = 0;
		other._currentBufferPosition = 0;
		other._frameBuffer = nullptr;
		other._frameBufferSize = 0;
	}

	return *this;
}

// Fill framebuffer with 0's
void FPSCounter::Clear(){
	if (_frameBuffer != nullptr){
		memset(_frameBuffer, 0, _frameBufferSize);
	}
}

// Call once per frame
void FPSCounter::Update(){
	++_currentBufferPosition;
	if (_currentBufferPosition >= _frameBufferSize){
		_currentBufferPosition = 0;
	}

	_frameBuffer[_currentBufferPosition] = __rdtsc();

	size_t tempIndex = _currentBufferPosition;
	int64_t fps = 0;
	uint64_t now, previous = 0;

	for (int i = 0; i < _frameBufferSize; i++){
		++tempIndex;
		if (tempIndex >= _frameBufferSize){
			tempIndex -= _frameBufferSize;
		}

		if (tempIndex == 0){
			now = _frameBuffer[tempIndex];
			previous = _frameBuffer[tempIndex - 1];
		}
		else{
			now = _frameBuffer[tempIndex];
			previous = _frameBuffer[_frameBufferSize - 1];
		}

		fps += now - previous;
	}

	fps /= _frameBufferSize;
	// Divide by processor frequency
	_fps = static_cast<double>(fps) / 800000000.0;
	// Milliseconds to seconds
	_fps /= 1000.0;
}

double FPSCounter::GetFPS(){
	return _fps;
}