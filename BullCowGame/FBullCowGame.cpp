/*
* The game logic (no view code or direct user interaction).
* The game is a simple guess the word game based on Mastermind.
*/

#pragma once
#include "FBullCowGame.h"
#include <map>
#include <vector>

// to make syntax Unreal friendly
#define TMap std::map
using FString = std::string;
using int32 = int;

FBullCowGame::FBullCowGame()
{
	// define isograms of length 3-7; MUST be isograms
	hiddenWords = { "ape", "bowl", "sword", "planet", "monkeys" };
	reset();
}

int32 FBullCowGame::getCurrentTry() const { return myCurrentTry; }
bool FBullCowGame::isGameWon() const { return bIsGameWon; }
int32 FBullCowGame::getHiddenWordLength() const { return myHiddenWord.length(); }

void FBullCowGame::reset()
{
	constexpr int32 MAX_TRIES = 8;
	myMaxTries = MAX_TRIES;

	myCurrentTry = 1;
	bIsGameWon = false;
	return;
}

void FBullCowGame::setHiddenWord(int32 wordLengthChoice)
{
	for (FString word : hiddenWords) {
		if (wordLengthChoice == word.length()) {
			myHiddenWord = word;
		}
	}
}

int32 FBullCowGame::getMaxTries() const { 
	TMap<int32, int32> wordLengthToMaxTries{ {3, 5}, {4, 8}, {5, 12}, {6, 15}, {7, 20} };
	return wordLengthToMaxTries[myHiddenWord.length()];
}

EGuessStatus FBullCowGame::isValidGuess(FString guess) const
{
	if (!isIsogram(guess)) {
		return EGuessStatus::Not_Isogram;
	} else if (!isLowercase(guess)) {
		return EGuessStatus::Not_Lowercase;
	} else if (guess.length() != getHiddenWordLength()) {
		return EGuessStatus::Wrong_Length;
	} else {
		return EGuessStatus::OK;
	}
}

// receives a valid guess, increments turn, and returns count
FBullCowCount FBullCowGame::submitValidGuess(FString guess)
{

	myCurrentTry++;
	FBullCowCount bullCowCount;


	int32 myHiddenWordLength = myHiddenWord.length();
	int32 guessLength = guess.length();

	// loop through all letters in guess
	for (int32 guessChar = 0; guessChar < guessLength; guessChar++) {
		// compare letters against the hidden word
		for (int32 hiddenWordChar = 0; hiddenWordChar < myHiddenWordLength; hiddenWordChar++) {
			if (guess[guessChar] == myHiddenWord[hiddenWordChar]) {
				if (guessChar == hiddenWordChar) {
					bullCowCount.bulls++;
				} else {
					bullCowCount.cows++;
				}
			}
		}
	}

	bIsGameWon = (bullCowCount.bulls == myHiddenWordLength);

	return bullCowCount;
}

bool FBullCowGame::isIsogram(FString guess) const
{
	TMap<char, bool> letterMap;

	// treat 0 and 1 letter words as isograms
	if (guess.length() <= 1) {
		return true;
	}
	// for each letter in the guess
	for (auto letter : guess) {
		letter = tolower(letter);
		// get the value for the map using the letter as the key
		bool isSeen = letterMap[letter];
		if (isSeen) {
			return false;
		} else {
			letterMap[letter] = true;
		}
	}
	return true; // for example, in cases where /0 is entered
}

bool FBullCowGame::isLowercase(FString guess) const
{
	if (guess.length() < 1) {
		return true;
	}
	for (auto letter : guess) {
		if (!islower(letter)) {
			return false;
		}
	}
	return true;
}
