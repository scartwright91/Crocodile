#include "UUID.h"

#include <random>
#include <unordered_map>

namespace Crocodile
{

	static std::random_device rd;
	static std::mt19937_64 engine(rd());
	static std::uniform_int_distribution<uint64_t> unif;

    UUID::UUID() : uuid(unif(engine))
    {
    }
    
}