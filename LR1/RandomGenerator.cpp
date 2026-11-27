#include <cmath>

#include "RandomGenerator.h"

int RandomGenerator::getInt(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(generator);
}

float RandomGenerator::getFloat(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(generator);
}

template<typename TOutput>
TOutput RandomGenerator::getValue(TOutput min, TOutput max) {
	std::uniform_real_distribution<TOutput> dist(min, max);
	return dist(generator);
}

template<>
bushels_t RandomGenerator::getValue(bushels_t min, bushels_t max) {
	std::uniform_int_distribution<bushels_t> dist(min, max);
	return dist(generator);
}

bushels_t RandomGenerator::getAcreCost() {
	return getValue<bushels_t>(17, 26);
}

bushels_t RandomGenerator::getAcrePerformance() {
	return getValue<bushels_t>(1, 6);
}

bushels_t RandomGenerator::getWheatDestroyed(bushels_t old_wheat) {
	float destroyed_rate = getFloat(0, 0.07f);

	bushels_t destroyed_wheat = static_cast<bushels_t>(std::roundf(static_cast<float>(old_wheat) * destroyed_rate));
	return destroyed_wheat;
}

bool RandomGenerator::getPlagueOccurred() {
	float random_num = getFloat(0, 1);
	return random_num <= 0.15f;
}

RandomGenerator random_generator;

void initRandomGenerator() {
	random_generator = RandomGenerator();
}

RandomGenerator& getRandomGenerator() {
	return random_generator;
}
