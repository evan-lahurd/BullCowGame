/* 
 * The game logic (no view code or direct user interaction).
 * The game is a simple guess the word game based on Mastermind.
 */

#pragma once
#include <string>
#include <vector>

// to make syntax Unreal friendly
using FString = std::string;
using int32 = int;

struct FBullCowCount
{
	int32 bulls = 0;
	int32 cows = 0;
};

enum class EGuessStatus
{
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase,
	Invalid
};

class FBullCowGame
{
public:
	FBullCowGame(); // constructor

	int32 getMaxTries() const;
	int32 getCurrentTry() const;
	int32 getHiddenWordLength() const;
	EGuessStatus isValidGuess(FString) const;
	bool isGameWon() const;

	void reset();
	void setHiddenWord(int32);
	FBullCowCount submitValidGuess(FString);
	
private:
	bool bIsGameWon;
	int32 myCurrentTry;
	int32 myMaxTries;
	FString myHiddenWord;
    std::vector<FString> hiddenWords;

	bool isIsogram(FString) const;
	bool isLowercase(FString) const;
};