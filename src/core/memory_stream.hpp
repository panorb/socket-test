#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <spdlog/spdlog.h>

class OutputMemoryStream
{
public:
	OutputMemoryStream() : mBuffer(nullptr), mHead(0), mCapacity(0)
	{
		ReallocBuffer(32);
	}
	~OutputMemoryStream() { std::free(mBuffer); }

	//get a pointer to the data in the stream
	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetLength() const { return mHead; }

	void Write(const void* inData, size_t inByteCount);
	void Write(uint32_t inData) { Write(&inData, sizeof(inData)); }
	void Write(int32_t inData) { Write(&inData, sizeof(inData)); }

private:
	void ReallocBuffer(uint32_t inNewLength);

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount) :
		mCapacity(inByteCount), mHead(0) {}

	~InputMemoryStream() { std::free(mBuffer); }

	uint32_t GetRemainingDataSize() const { return mCapacity - mHead; }

	void Read(void* outData, uint32_t inByteCount);
	void Read(uint32_t& outData) { Read(&outData, sizeof(outData)); }
	void Read(int32_t& outData) { Read(&outData, sizeof(outData)); }

private:
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};
