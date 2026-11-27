#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "GameIO.h"

namespace {
	int readIntPrompt(const char* prompt) {
		int value = 0;
		while (true) {
			std::cout << prompt;
			if (std::cin >> value) {
				return value;
			}
			if (std::cin.eof()) {
				return -1;
			}
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			std::cout << "Повелитель, поведай число, а не неведомые письмена!" << std::endl;
		}
	}
}

void GameDisplay::showRoundStatistics(GameRoundState state) {
	std::cout << "Мой Повелитель, соизволь поведать Тебе," << std::endl;
	std::cout << "\tнаступил " << state.years_passed << " год Твоего правления" << std::endl;
	if (state.subjects_died > 0) {
		std::cout << "\tот голода скончались " << state.subjects_died << " жителей;" << std::endl;
	}
	if (state.subjects_arrived > 0) {
		std::cout << "\tв наш великий город приехало " << state.subjects_arrived << " инолюдов;" << std::endl;
	}
	if (state.plague_occurred) {
		std::cout << "\tБоги прокляли нас: чёрная чума убила половину населения;" << std::endl;
	}
	else {
		std::cout << "\tчумы, да хранит нас Шамаш, не было;" << std::endl;
	}
	std::cout << "\tгород населяют " << state.population << " верных Тебе подданных;" << std::endl;
	std::cout
		<< "\tгород собрал " << state.wheat_added << " бушелей пшеницы, по "
		<< state.wheat_avg << " бушелей с каждого засеянного акра;" << std::endl;
	std::cout << "\tно, вот проклятье, крысы уничтожили " << state.wheat_destroyed << " бушелей;" << std::endl;
	std::cout << "\tв закромах упрятаны " << state.wheat << " бушелей пшеницы;" << std::endl;
	std::cout << "\tнаш великий город занимает " << state.area << " акров земли;" << std::endl;
	std::cout << "\tв этом году один акр стоит " << state.acre_cost << " бушелей пшеницы." << std::endl;
}

void GameDisplay::showFirstRound(GameRoundState state) {
	std::cout << "Здравствуй, Повелитель! Боги улыбаются Тебе, ведь Ты только что взошёл на трон. Позволь поведать Тебе о делах города:" << std::endl;
	std::cout << "\tгород населяют " << state.population << " верных Тебе подданных;" << std::endl;
	std::cout << "\tнаш великий город занимает " << state.area << " акров земли;" << std::endl;
	std::cout << "\tв закромах упрятаны " << state.wheat << " бушелей пшеницы;" << std::endl;
	std::cout << "\tстоимость акра земли в этом году - " << state.acre_cost << " бушелей пшеницы." << std::endl;
}

PlayerRoundInput GameDisplay::promptUser() {
	auto input = PlayerRoundInput();

	std::cout << "Что прикажешь делать?" << std::endl;

	input.area_to_buy = readIntPrompt("Сколько акров земли повелеваешь купить? ");
	input.area_to_sell = readIntPrompt("Сколько акров земли повелеваешь продать? ");
	input.bushels_to_eat = readIntPrompt("Сколько бушелей пшеницы повелеваешь съесть? ");
	input.area_to_sow = readIntPrompt("Сколько акров повелеваешь засеять? ");

	return input;
}

void GameDisplay::showIncorrectInput(GameRoundState state) {
	std::cout << "Пожалей нас, о, Повелитель! " << std::endl;
	std::cout << "В городе проживают лишь " << state.population << " человек, " << std::endl;
	std::cout << "осталось " << state.wheat << " бушелей пшеницы, " << std::endl;
	std::cout << "и в нашем распоряжении лишь " << state.area << " акров!.." << std::endl;
}

void GameDisplay::showEndgame(result_t result) {
	switch (result) {
	case RESULT_NONE:
		std::cout << "О, Повелитель! Наши ярчайшие умы не могут дать оценку твоему правлению..." << std::endl;
		break;
	case RESULT_BAD:
		std::cout << "Повелитель! У Твоего дворца собралась толпа! "
			"Твои воины уже выламывают двери. Да простит меня Шамаш за то, "
			"что Ты вынудил меня сделать! *вонзает клинок в спину*" << std::endl;
		break;
	case RESULT_NORMAL:
		std::cout << "Повелитель! Ты правил... Да что сказать? "
			"С твоим уходом, народ вздохнёт с облегчением. "
			"Покинь дворец полюбовно, и народ, может быть, простит Тебя." << std::endl;
		break;
	case RESULT_GOOD:
		std::cout << "Повелитель! Вот и подошло к концу Твое правление. "
			"Наши верные летописцы восславят Твой успех. Горожане будут "
			"с теплотой отзываться о временах, когда Ты вёл нас вперёд." << std::endl;
		break;
	case RESULT_EXCELLENT:
		std::cout << "Повелитель! Твои успехи да послужат примером всей Вавилонии! "
			"Да будет священно имя твоё, и пусть Боги улыбаются, "
			"когда слышат твоё имя! Потомки никогда не забудут Тебя!"
			<< std::endl;
		break;
	}
}

void GameDisplay::showLoss() {
	std::cout << "Повелитель! Подданные бегут из города! Ты погубил нас!" << std::endl;
}

RoundStateReadWriter::RoundStateReadWriter(const std::string& path) : file_path(path) {
}

void RoundStateReadWriter::write(GameRoundState state) {
	std::ofstream out(file_path, std::ios::trunc);
	if (!out.is_open()) {
		throw std::runtime_error("Ошибка: файл недоступен для записи.");
	}

	out << state.population << " ";
	out << state.area << " ";
	out << state.wheat << " ";
	out << state.years_passed << " ";
	out << state.subjects_died << " ";
	out << state.subjects_arrived << " ";
	out << state.plague_occurred << " ";
	out << state.wheat_avg << " ";
	out << state.wheat_added << " ";
	out << state.wheat_destroyed << " ";
	out << state.acre_cost << " ";
	out << state.total_died_percent << "\n";
	out.flush();
}

GameRoundState RoundStateReadWriter::read() {
	std::ifstream in(file_path);
	if (!in.is_open()) {
		return GameRoundState::MakeDefault();
	}

	GameRoundState state = GameRoundState();

	in >> state.population;
	in >> state.area;
	in >> state.wheat;
	in >> state.years_passed;
	in >> state.subjects_died;
	in >> state.subjects_arrived;
	in >> state.plague_occurred;
	in >> state.wheat_avg;
	in >> state.wheat_added;
	in >> state.wheat_destroyed;
	in >> state.acre_cost;
	in >> state.total_died_percent;

	if (!in) {
		return GameRoundState::MakeDefault();
	}

	return state;
}

bool RoundStateReadWriter::fileExists(const std::string& path) {
	return std::filesystem::exists(path);
}

void RoundStateReadWriter::removeFile(const std::string& path) {
	std::error_code ec;
	std::filesystem::remove(path, ec);
}
