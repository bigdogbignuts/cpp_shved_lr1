#pragma once

#include <string>

#include "GameState.h"

class GameDisplay
{
public:
	static void showRoundStatistics(GameRoundState state);
	static void showFirstRound(GameRoundState state);

	static PlayerRoundInput promptUser();
	static void showIncorrectInput(GameRoundState state);

	static void showEndgame(result_t result);
	static void showLoss();
};

class RoundStateReadWriter
{
public:
	explicit RoundStateReadWriter(const std::string& path);

	void write(GameRoundState state);
	GameRoundState read();

	static bool fileExists(const std::string& path);
	static void removeFile(const std::string& path);
private:
	std::string file_path;
};
