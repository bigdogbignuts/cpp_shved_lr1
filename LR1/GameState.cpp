#include "GameState.h"
#include "RandomGenerator.h"

GameRoundState GameRoundState::MakeDefault() {
	auto& rng = getRandomGenerator();

	auto state = GameRoundState();

	state.population = 100;
	state.wheat = 2800;
	state.area = 1000;

	state.subjects_died = 0;
	state.subjects_arrived = 0;
	state.plague_occurred = false;
	state.wheat_avg = 0;
	state.wheat_added = 0;
	state.wheat_destroyed = 0;
	state.acre_cost = rng.getAcreCost();

	state.total_died_percent = 0.0f;

	state.years_passed = 0;

	return state;
}

RoundPerformResult PerformRound(GameRoundState previous, PlayerRoundInput input) {
	auto& rng = getRandomGenerator();

	RoundPerformResult immediate_loss = RoundPerformResult();
	immediate_loss.new_state = previous;
	immediate_loss.loss_immediately = true;

	GameRoundState state = previous;

	// Buy and sell land
	state.area += input.area_to_buy;
	state.area -= input.area_to_sell;
	state.wheat -= input.area_to_buy * state.acre_cost;
	state.wheat += input.area_to_sell * state.acre_cost;

	// Spend wheat on food and seeds (0.5 bushel per acre, rounded up)
	bushels_t seed_cost = (input.area_to_sow + 1) / 2;
	state.wheat -= input.bushels_to_eat;
	state.wheat -= seed_cost;

	// Harvest
	state.wheat_avg = rng.getAcrePerformance();
	state.wheat_added = state.wheat_avg * input.area_to_sow;
	state.wheat += state.wheat_added;

	// Rats destroy wheat
	state.wheat_destroyed = rng.getWheatDestroyed(state.wheat);
	state.wheat -= state.wheat_destroyed;

	// Starvation
	population_t prev_population = state.population;
	state.subjects_died = state.population - (input.bushels_to_eat / 20);
	if (state.subjects_died < 0) {
		state.subjects_died = 0;
	}
	state.population -= state.subjects_died;

	float died_percent = prev_population > 0
		? static_cast<float>(state.subjects_died) / static_cast<float>(prev_population)
		: 1.0f;
	state.total_died_percent += died_percent;

	if (died_percent > 0.45f || state.population <= 0) {
		return immediate_loss;
	}

	// New subjects arrive
	state.subjects_arrived = state.subjects_died / 2 + (5 - state.wheat_avg) * state.wheat / 600 + 1;
	if (state.subjects_arrived < 0) {
		state.subjects_arrived = 0;
	}
	if (state.subjects_arrived > 50) {
		state.subjects_arrived = 50;
	}
	state.population += state.subjects_arrived;

	// Plague
	state.plague_occurred = rng.getPlagueOccurred();
	if (state.plague_occurred) {
		state.population /= 2;
	}

	// Next year's acre cost and year counter
	state.acre_cost = rng.getAcreCost();
	++state.years_passed;

	auto result = RoundPerformResult();
	result.new_state = state;
	result.loss_immediately = false;
	return result;
}

bool GameRoundState::canPerform(PlayerRoundInput input) {
	if (input.area_to_buy < 0
		|| input.area_to_sell < 0
		|| input.area_to_sow < 0
		|| input.bushels_to_eat < 0) {
		return false;
	}

	if (input.area_to_sell > area) {
		return false;
	}

	area_t new_area = area + input.area_to_buy - input.area_to_sell;
	if (input.area_to_sow > new_area) {
		return false;
	}

	if (input.area_to_sow > population * 10) {
		return false;
	}

	long long wheat_after_trade = static_cast<long long>(wheat)
		+ static_cast<long long>(input.area_to_sell) * acre_cost
		- static_cast<long long>(input.area_to_buy) * acre_cost;
	if (wheat_after_trade < 0) {
		return false;
	}

	long long seed_cost = (static_cast<long long>(input.area_to_sow) + 1) / 2;
	if (static_cast<long long>(input.bushels_to_eat) + seed_cost > wheat_after_trade) {
		return false;
	}

	return true;
}

result_t determineResult(GameRoundState roundState) {
	if (roundState.population <= 0) {
		return RESULT_BAD;
	}

	float rounds = static_cast<float>(GameConfig::number_of_rounds);

	float P = roundState.total_died_percent / rounds;
	float L = static_cast<float>(roundState.area) / static_cast<float>(roundState.population);

	if (P > 0.33f || L < 7.0f) {
		return RESULT_BAD;
	}
	else if (P > 0.10f || L < 9.0f) {
		return RESULT_NORMAL;
	}
	else if (P > 0.03f || L < 10.0f) {
		return RESULT_GOOD;
	}
	else {
		return RESULT_EXCELLENT;
	}
}
