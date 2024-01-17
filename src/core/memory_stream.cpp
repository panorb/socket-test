#include "memory_stream.hpp"

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength)
{
	buffer_ = static_cast<char*>(std::realloc(buffer_, inNewLength));
	// TODO: Handle realloc failure
	capacity_ = inNewLength;
}

void OutputMemoryStream::Write(const void* inData,
	size_t inByteCount)
{
	//make sure we have space...
	uint32_t resultHead = head_ + static_cast<uint32_t>(inByteCount);
	if (resultHead > capacity_)
	{
		ReallocBuffer(std::max(capacity_ * 2, resultHead));
	}

	//copy into buffer at head
	std::memcpy(buffer_ + head_, inData, inByteCount);

	//increment head for next write
	head_ = resultHead;
}

void InputMemoryStream::Read(void* outData,
	uint32_t inByteCount)
{
	uint32_t resultHead = head_ + inByteCount;
	if (resultHead > capacity_)
	{
		spdlog::error("InputMemoryStream::Read() - no data to read!");
	}

	std::memcpy(outData, buffer_ + head_, inByteCount);

	head_ = resultHead;
}
