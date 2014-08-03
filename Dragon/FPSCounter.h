#ifndef __FPS_COUNTER_H
#define __FPS_COUNTER_H

#include <cinttypes>

class FPSCounter {
private:
	uint64_t* _frameBuffer;
	uint64_t _rdtscBegin;
	size_t _frameBufferSize;
	size_t _currentBufferPosition;
	double _fps;

public:
	// Constructor
	FPSCounter(size_t numBufferedFrames = 1000);
	// Destructor
	~FPSCounter();
	// Copy Constructor
	FPSCounter(const FPSCounter& other);
	// Copy Assignment
	FPSCounter& operator=(const FPSCounter& other);
	
	// Move constructor
	FPSCounter(FPSCounter&& other);
	// Move assignment
	FPSCounter& operator=(FPSCounter&& other);

	// Fill framebuffer with 0's
	void Clear();

	// Call once per frame
	void Update();

	// Get current fps
	double GetFPS();
};

#endif __FPS_COUNTER_H