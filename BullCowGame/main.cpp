/* This is the console executable that makes use of the FBullCowGame class.
This acts as the view in an MVC pattern, and is responsible for all user interaction. 
For game logic, see the FBullCowGame class.
*/
#pragma once
#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes
int main();
void printIntro();
FText getUserWordLengthChoice();
int32 parseUserWordLengthChoice();
void printGameSummary();
FText getValidGuess();
bool askToPlayAgain();
void playGame();

FBullCowGame bcGame; // instantiate a new game which we re-use across plays

int main() {

	bool bPlayAgain;
	do {
		printIntro();
		playGame();
		bPlayAgain = askToPlayAgain();
	} while (bPlayAgain);

	return 0;
}

void printIntro()
{
	int32 wordLength = bcGame.getHiddenWordLength();
	std::cout << std::endl << "Welcome to Bulls and Cows, a fun word game." << std::endl;
	std::cout << std::endl;
	std::cout << "           }   {              ___ " << std::endl;
	std::cout << "           (o o)             (o o) " << std::endl;
	std::cout << "    /-------\\ /               \\ /-------\\ " << std::endl;
	std::cout << "   / | BULL |o                 o|  COW  |\\ " << std::endl;
	std::cout << "  *  |-,--- |                   |-------| * " << std::endl;
	std::cout << "     ^      ^                   ^       ^  " << std::endl;
	std::cout << "Can you guess the isogram I'm thinking of?" << std::endl;
	return;
}

// plays a single game to completion
void playGame()
{
	bcGame.reset();

	// set the hidden word based on the word length chosen by the user
	bcGame.setHiddenWord(parseUserWordLengthChoice());

	// get the maximum number of tries the user gets based on the hidden word length
	int32 maxTries = bcGame.getMaxTries();

	// loop asking for guesses while game is not won and there are still tries remaining
	while(!bcGame.isGameWon() && bcGame.getCurrentTry() <= maxTries) {
		FText guess = getValidGuess();
		FBullCowCount bullCowCount = bcGame.submitValidGuess(guess);

		std::cout << "Bulls = " << bullCowCount.bulls << "; Cows = " << bullCowCount.cows << std::endl;
	}

	printGameSummary();
}

FText getUserWordLengthChoice() {
	std::cout << "What word length would you like to guess (3 - 7)? ";
	FText response = "";
	std::getline(std::cin, response);
	return response;
}

// continually ask the user for the length of the word the want to guess
int32 parseUserWordLengthChoice() {
	bool validWordLength;
	int32 parsedWordLengthChoice;
	do {
		FText userWordLengthChoice = getUserWordLengthChoice();
		// try to parse the user's word length choice as an integer
		try {
			parsedWordLengthChoice = std::stoi(userWordLengthChoice);
			if (parsedWordLengthChoice >= 3 && parsedWordLengthChoice <= 7) {
				validWordLength = true;
			} else {
				std::cout << "The word length must be between 3 and 7, inclusive." << std::endl << std::endl;
				validWordLength = false;
			}
		} catch (std::invalid_argument e) {
			std::cout << "Please enter an integer." << std::endl;
			validWordLength = false;
		}
	} while (!validWordLength);

	return parsedWordLengthChoice;
}

void printGameSummary()
{
	if (bcGame.isGameWon()) {
		std::cout << "You won!" << std::endl;
	} else {
		std::cout << "You lost!" << std::endl;
	}
}

// loop continually until the user gives a valid guess
FText getValidGuess()
{
	EGuessStatus guessStatus = EGuessStatus::Invalid;
	FText guess = "";
	do {
		int32 currentTry = bcGame.getCurrentTry();
		std::cout << std::endl << "Try " << currentTry << " of " << bcGame.getMaxTries() << ". Enter your guess: ";
		std::getline(std::cin, guess);

		guessStatus = bcGame.isValidGuess(guess);
		switch (guessStatus) {
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << bcGame.getHiddenWordLength() << " letter word." << std::endl;
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter a word using only lowercase letters." << std::endl;
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters." << std::endl;
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (guessStatus != EGuessStatus::OK);

	return guess;
}

bool askToPlayAgain()
{
	std::cout << "Do you want to play again (y/n)? ";
	FText response = "";
	std::getline(std::cin, response);
	return (response[0] == 'y' || response[0] == 'Y');
}
