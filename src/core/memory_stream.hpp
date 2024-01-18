#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>

#include <raymath.h>

class GameObject;

inline uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision)
{
	return static_cast<int> ((inNumber - inMin) / inPrecision);
}

inline float ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision)
{
	return static_cast<float>(inNumber) * inPrecision + inMin;
}


class OutputMemoryBitStream
{
public:

	OutputMemoryBitStream() :
		bit_head_(0),
		buffer_(nullptr)
	{
		ReallocBuffer(1500 * 8);
	}

	~OutputMemoryBitStream() { std::free(buffer_); }

	void WriteBits(uint8_t inData, uint32_t inBitCount);
	void WriteBits(const void* inData, uint32_t inBitCount);

	const char* GetBufferPtr() const { return buffer_; }
	uint32_t GetBitLength() const { return bit_head_; }
	uint32_t GetByteLength() const { return (bit_head_ + 7) >> 3; }

	void WriteBytes(const void* inData, uint32_t inByteCount) { WriteBits(inData, inByteCount << 3); }

	/*
	void Write( uint32_t inData, uint32_t inBitCount = 32 ) { WriteBits( &inData, inBitCount ); }
	void Write( int inData, uint32_t inBitCount = 32 ) { WriteBits( &inData, inBitCount ); }
	void Write( float inData ) { WriteBits( &inData, 32 ); }

	void Write( uint16_t inData, uint32_t inBitCount = 16 ) { WriteBits( &inData, inBitCount ); }
	void Write( int16_t inData, uint32_t inBitCount = 16 ) { WriteBits( &inData, inBitCount ); }

	void Write( uint8_t inData, uint32_t inBitCount = 8 ) { WriteBits( &inData, inBitCount ); }
	*/

	template< typename T >
	void Write(T inData, uint32_t inBitCount = sizeof(T) * 8)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Write only supports primitive data types");
		WriteBits(&inData, inBitCount);
	}

	void Write(bool inData) { WriteBits(&inData, 1); }

	void Write(const Vector3& in_vector);
	void Write(const Quaternion& in_quat);

	void Write(const std::string& inString)
	{
		uint32_t elementCount = static_cast<uint32_t>(inString.size());
		Write(elementCount);
		for (const auto& element : inString)
		{
			Write(element);
		}
	}

private:
	void		ReallocBuffer(uint32_t inNewBitCapacity);

	char* buffer_;
	uint32_t bit_head_;
	uint32_t bit_capacity_;
};

class InputMemoryBitStream
{
public:

	InputMemoryBitStream(char * in_buffer, uint32_t in_bit_count) :
		buffer_(in_buffer),
		bit_capacity_(in_bit_count),
		bit_head_(0),
		is_buffer_owner_(false) {}

	InputMemoryBitStream(const InputMemoryBitStream& in_other) :
		bit_capacity_(in_other.bit_capacity_),
		bit_head_(in_other.bit_head_),
		is_buffer_owner_(true)
	{
		//allocate buffer of right size
		int byte_count = bit_capacity_ / 8;
		buffer_ = static_cast<char*>(malloc(byte_count));
		//copy
		memcpy(buffer_, in_other.buffer_, byte_count);
	}

	~InputMemoryBitStream() { if (is_buffer_owner_) { free(buffer_); }; }

	const char* GetBufferPtr() const { return buffer_; }
	uint32_t GetRemainingBitCount() const { return bit_capacity_ - bit_head_; }

	void ReadBits(uint8_t& out_data, uint32_t in_bit_count);
	void ReadBits(void* out_data, uint32_t in_bit_count);

	void ReadBytes(void* out_data, uint32_t in_byte_count) { ReadBits(out_data, in_byte_count << 3); }

	template< typename T >
	void Read(T& in_data, uint32_t in_bit_count = sizeof(T) * 8)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Read only supports primitive data types");
		ReadBits(&in_data, in_bit_count);
	}

	void Read(uint32_t& out_data, uint32_t in_bit_count = 32) { ReadBits(&out_data, in_bit_count); }
	void Read(int& out_data, uint32_t in_bit_count = 32) { ReadBits(&out_data, in_bit_count); }
	void Read(float& out_data) { ReadBits(&out_data, 32); }

	void Read(uint16_t& out_data, uint32_t in_bit_count = 16) { ReadBits(&out_data, in_bit_count); }
	void Read(int16_t& out_data, uint32_t in_bit_count = 16) { ReadBits(&out_data, in_bit_count); }

	void Read(uint8_t& out_data, uint32_t in_bit_count = 8) { ReadBits(&out_data, in_bit_count); }
	void Read(bool& out_data) { ReadBits(&out_data, 1); }

	void Read(Quaternion& out_quat);

	void ResetToCapacity(uint32_t in_byte_capacity) { bit_capacity_ = in_byte_capacity << 3; bit_head_ = 0; }


	void Read(std::string& in_string)
	{
		uint32_t element_count;
		Read(element_count);
		in_string.resize(element_count);
		for (auto& element : in_string)
		{
			Read(element);
		}
	}

	void Read(Vector3 & out_vector);

private:
	char* buffer_;
	uint32_t bit_head_;
	uint32_t bit_capacity_;
	bool is_buffer_owner_;
};
