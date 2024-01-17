#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <spdlog/spdlog.h>

class OutputMemoryStream
{
public:
	OutputMemoryStream() : buffer_(nullptr), head_(0), capacity_(0)
	{
		ReallocBuffer(32);
	}
	~OutputMemoryStream() { std::free(buffer_); }

	//get a pointer to the data in the stream
	const char* GetBufferPtr() const { return buffer_; }
	uint32_t GetLength() const { return head_; }

	void Write(const void* inData, size_t inByteCount);
	void Write(uint32_t inData) { Write(&inData, sizeof(inData)); }
	void Write(int32_t inData) { Write(&inData, sizeof(inData)); }

private:
	void ReallocBuffer(uint32_t inNewLength);

	char* buffer_;
	uint32_t head_;
	uint32_t capacity_;
};

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount) :
		buffer_(inBuffer), capacity_(inByteCount), head_(0) {}

	~InputMemoryStream() { std::free(buffer_); }

	uint32_t GetRemainingDataSize() const { return capacity_ - head_; }

	void Read(void* outData, uint32_t inByteCount);
	void Read(uint32_t& outData) { Read(&outData, sizeof(outData)); }
	void Read(int32_t& outData) { Read(&outData, sizeof(outData)); }

private:
	char* buffer_;
	uint32_t head_;
	uint32_t capacity_;
};
