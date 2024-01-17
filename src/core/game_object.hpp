#pragma once

#include <memory>

#include "memory_stream.hpp"

class GameObject {
public:
	virtual uint32_t Write(OutputMemoryBitStream& in_output_stream, uint32_t in_dirty_state) = 0;
	virtual void Read(InputMemoryBitStream& in_input_stream) = 0;
private:
	int network_id_ = -1;
};

typedef std::shared_ptr<GameObject> GameObjectPtr;
