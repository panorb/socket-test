#include "memory_stream.hpp"

void OutputMemoryBitStream::WriteBits(uint8_t inData,
	uint32_t inBitCount)
{
	uint32_t nextBitHead = bit_head_ + static_cast<uint32_t>(inBitCount);

	if (nextBitHead > bit_capacity_)
	{
		ReallocBuffer(std::max(bit_capacity_ * 2, nextBitHead));
	}

	//calculate the byteOffset into our buffer
	//by dividing the head by 8
	//and the bitOffset by taking the last 3 bits
	uint32_t byteOffset = bit_head_ >> 3;
	uint32_t bitOffset = bit_head_ & 0x7;

	uint8_t currentMask = ~(0xff << bitOffset);
	buffer_[byteOffset] = (buffer_[byteOffset] & currentMask) | (inData << bitOffset);

	//calculate how many bits were not yet used in
	//our target byte in the buffer
	uint32_t bitsFreeThisByte = 8 - bitOffset;

	//if we needed more than that, carry to the next byte
	if (bitsFreeThisByte < inBitCount)
	{
		//we need another byte
		buffer_[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	bit_head_ = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, uint32_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);
	//write all the bytes
	while (inBitCount > 8)
	{
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}
	//write anything left
	if (inBitCount > 0)
	{
		WriteBits(*srcByte, inBitCount);
	}
}

void OutputMemoryBitStream::Write(const Vector3& in_vector)
{
	Write(in_vector.x);
	Write(in_vector.y);
	Write(in_vector.z);
}

void InputMemoryBitStream::Read(Vector3& out_vector)
{
	Read(out_vector.x);
	Read(out_vector.y);
	Read(out_vector.z);
}

void OutputMemoryBitStream::Write(const Quaternion& in_quat)
{
	float precision = (2.f / 65535.f);
	Write(ConvertToFixed(in_quat.x, -1.f, precision), 16);
	Write(ConvertToFixed(in_quat.y, -1.f, precision), 16);
	Write(ConvertToFixed(in_quat.z, -1.f, precision), 16);
	Write(in_quat.w < 0);
}



void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewBitLength)
{
	if (buffer_ == nullptr)
	{
		//just need to memset on first allocation
		buffer_ = static_cast<char*>(std::malloc(inNewBitLength >> 3));
		memset(buffer_, 0, inNewBitLength >> 3);
	}
	else
	{
		//need to memset, then copy the buffer
		char* tempBuffer = static_cast<char*>(std::malloc(inNewBitLength >> 3));
		memset(tempBuffer, 0, inNewBitLength >> 3);
		memcpy(tempBuffer, buffer_, bit_capacity_ >> 3);
		std::free(buffer_);
		buffer_ = tempBuffer;
	}

	//handle realloc failure
	//...
	bit_capacity_ = inNewBitLength;
}


void test1()
{
	OutputMemoryBitStream mbs;

	mbs.WriteBits(11, 5);
	mbs.WriteBits(52, 6);
}

void InputMemoryBitStream::ReadBits(uint8_t& out_data, uint32_t in_bit_count)
{
	uint32_t byte_offset = bit_head_ >> 3;
	uint32_t bit_offset = bit_head_ & 0x7;

	out_data = static_cast<uint8_t>(buffer_[byte_offset]) >> bit_offset;

	uint32_t bits_free_this_byte = 8 - bit_offset;
	if (bits_free_this_byte < in_bit_count)
	{
		//we need another byte
		out_data |= static_cast<uint8_t>(buffer_[byte_offset + 1]) << bits_free_this_byte;
	}

	//don't forget a mask so that we only read the bit we wanted...
	out_data &= (~(0x00ff << in_bit_count));

	bit_head_ += in_bit_count;
}

void InputMemoryBitStream::ReadBits(void* out_data, uint32_t in_bit_count)
{
	uint8_t* dest_byte = reinterpret_cast<uint8_t*>(out_data);
	//write all the bytes
	while (in_bit_count > 8)
	{
		ReadBits(*dest_byte, 8);
		++dest_byte;
		in_bit_count -= 8;
	}
	//write anything left
	if (in_bit_count > 0)
	{
		ReadBits(*dest_byte, in_bit_count);
	}
}

void InputMemoryBitStream::Read(Quaternion& out_quat)
{
	float precision = (2.f / 65535.f);

	uint32_t f = 0;

	Read(f, 16);
	out_quat.x = ConvertFromFixed(f, -1.f, precision);
	Read(f, 16);
	out_quat.y = ConvertFromFixed(f, -1.f, precision);
	Read(f, 16);
	out_quat.z = ConvertFromFixed(f, -1.f, precision);

	out_quat.w = sqrtf(1.f -
		out_quat.x * out_quat.y +
		out_quat.y * out_quat.y +
		out_quat.z * out_quat.z);
	bool is_negative;
	Read(is_negative);

	if (is_negative)
	{
		out_quat.w *= -1;
	}
}
