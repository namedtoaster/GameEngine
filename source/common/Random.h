#ifndef _RANDOM_
#define _RANDOM_

#include <random>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "CommonExport.h"

// Defines methods to generate random numbers
class Random
{
public:

	COMMON_API static Random& Instance();

	// Returns a random int between [0, max]
	COMMON_API int Generate(int max);

	// Returns a random float between [0, max]
	COMMON_API float Generate(float max);

	// Returns a random int between [min, max]
	COMMON_API int Generate(int min, int max);

	// Returns a random float between [min, max]
	COMMON_API float Generate(float min, float max);

	// Returns a random 2D vector between [min, max]
	COMMON_API glm::vec2 GenerateVector(const glm::vec2& min, const glm::vec2& max);

	// Returns a random 3D vector between [min, max]
	COMMON_API glm::vec3 GenerateVector(const glm::vec3& min, const glm::vec3& max);

private:

	std::mt19937 m_generator;

	Random();
	Random(const Random&) = delete;
	Random& operator=(const Random&) = delete;
};

#endif // _RANDOM_
