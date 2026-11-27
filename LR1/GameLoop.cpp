#include "GameLoop.h"
#include "GameState.h"
#include "GameIO.h"
#include "RandomGenerator.h"

void GameLoop::mainLoop() {
	initRandomGenerator();

	GameRoundState state = GameRoundState();

	bool save_exists = RoundStateReadWriter::fileExists(save_file_path);

	RoundStateReadWriter readWriter = RoundStateReadWriter(save_file_path);

	if (save_exists) {
		state = readWriter.read();

		if (state.years_passed >= GameConfig::number_of_rounds) {
			state = GameRoundState::MakeDefault();
		}
	}
	else {
		state = GameRoundState::MakeDefault();
	}

	readWriter.write(state);

	while (state.years_passed < GameConfig::number_of_rounds) {
		if (state.years_passed == 0) {
			GameDisplay::showFirstRound(state);
		}
		else {
			GameDisplay::showRoundStatistics(state);
		}

		auto input = GameDisplay::promptUser();
		while (!state.canPerform(input)) {
			GameDisplay::showIncorrectInput(state);
			input = GameDisplay::promptUser();
		}

		auto result = PerformRound(state, input);

		if (result.loss_immediately) {
			GameDisplay::showLoss();
			RoundStateReadWriter::removeFile(save_file_path);
			return;
		}

		state = result.new_state;

		readWriter.write(state);
	}

	auto result = determineResult(state);
	GameDisplay::showEndgame(result);
}
