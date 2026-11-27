#pragma once

#include <random>

#include "GameState.h"

class RandomGenerator
{
public:
	RandomGenerator() = default;
	RandomGenerator(int seed) : generator(seed) {}

	bushels_t getAcreCost();
	bushels_t getAcrePerformance();
	bushels_t getWheatDestroyed(bushels_t old_wheat);
	bool getPlagueOccurred();

private:
	std::mt19937 generator{ std::random_device{}() };

	int getInt(int min, int max);

	float getFloat(float min, float max);

	template<typename TOutput>
	TOutput getValue(TOutput min, TOutput max);
};

RandomGenerator& getRandomGenerator();
void initRandomGenerator();
