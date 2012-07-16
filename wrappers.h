#pragma once

#include <gim/base/Memory.hpp>

namespace gim { namespace pod {
    
    typedef gim::pod::GMemory<const Gubyte> MemoryC;

} }

void hexDump(const gim::pod::MemoryC& mem);

enum {
    Error_Invalid_Distance = 18,
    Error_Premature_End_Of_Data = 22,
    Error_Memory_Allocation = 83,
    Error_Compression = 256
};

