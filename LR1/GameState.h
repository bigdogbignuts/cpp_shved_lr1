#pragma once

typedef int population_t;
typedef int area_t;
typedef int bushels_t;

struct GameConfig {
	static const int number_of_rounds = 10;
};

class PlayerRoundInput
{
public:
	area_t area_to_buy;
	area_t area_to_sell;
	bushels_t bushels_to_eat;
	area_t area_to_sow;
};

class GameRoundState
{
public:
	population_t population;
	area_t area;
	bushels_t wheat;

	int years_passed;

	population_t subjects_died;
	population_t subjects_arrived;
	bool plague_occurred;
	bushels_t wheat_avg;
	bushels_t wheat_added;
	bushels_t wheat_destroyed;
	bushels_t acre_cost;

	float total_died_percent;

	static GameRoundState MakeDefault();

	bool canPerform(PlayerRoundInput input);
};

class RoundPerformResult {
public:
	GameRoundState new_state;
	bool loss_immediately;
};

RoundPerformResult PerformRound(GameRoundState previous, PlayerRoundInput input);

typedef enum { RESULT_NONE, RESULT_BAD, RESULT_NORMAL, RESULT_GOOD, RESULT_EXCELLENT } result_t;

result_t determineResult(GameRoundState roundState);
