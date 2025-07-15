#include <iostream>
#include <string>
#include <cstdlib>  // for rand()
#include <limits> //for int inputs
#include <vector> // for guess history

/*
 	TO DO LIST 
 	- function for rng (COMPLETE)
 	- function for check if code has no repitions (complete)
 	- function for bulls and cows (complete)
 	- AI Guess checker (complete)
 	- choice for easy or medium ai (complete)

    MEMBERS
    -   Gabriel Barrientos
	-   Danial Ahmed (Zoey)
*/

using namespace std;

enum Difficulty {
        Easy,
        Medium,
        Invalid
};

//just turn any input to lowercasing
string toLower(const string& str) {
    string lowerStr;
    for (char c : str) {
        lowerStr += tolower(c);
    }
    return lowerStr;
}

Difficulty getDifficultyFromInput(const string& input) {
    string lowerInput = toLower(input);

    if (lowerInput == "easy" || lowerInput == "0") {
        return Difficulty::Easy;
    } else if (lowerInput == "medium" || lowerInput == "1") {
        return Difficulty::Medium;
    } else {
        return Difficulty::Invalid;
    }
}

//Function to check for any repetitions + removed unused "CurrentDigit"
bool checker(int code[], int digit){
 
 	for (int i = 0; i < 4; i++){
	 
	 	if (code[i] == digit){	
	    	return true;
	 	}
	 }
	 return false;
}

// Function for generating numbers + converting to string + handling difficulty
string Generator(Difficulty difficulty, const vector<string>& guessHistory = {}) {
    int code[4];
	
	//Makes sure that the first digit is not 0.
    code[0] = rand() % 9 + 1;
	
	//loop to get remaining last digits
    for (int i = 1; i < 4; i++) {
	int digit; //used to hold placement of remaining digits	
    do {
       digit = rand() % 10;
    }   while (checker(code, digit));
    code [i] = digit;
    }

    // Converts to String
    string result = "";
    for (int i = 0; i < 4; i++) {
        result += to_string(code[i]);
    }

    //medium(++) will never repeat a previous guess
    if (difficulty != Easy) {
        bool found = false;
        for (const string& prevGuess : guessHistory) {
            if (prevGuess == result) {
                found = true;
                break;
            }
        }

        if (found) {
            return Generator(difficulty, guessHistory);
        }
    }

    return result;
}

//checks valid 4 digit inputs
bool intInput(string &input)
{
    while(true) {
        
        if (!getline(cin, input)) {
            cin.clear();
            cout << "Error reading input." << endl;
            continue;
        }

        if (input.empty()) {
            cout << "Input cannot be empty." << endl;
            continue;
        }

        if (input.length() != 4) {
            cout << "Input must be exactly 4 digits." << endl;
            continue;
        }
        
        bool allDigits = true;
        for (char c : input) {
            if (!isdigit(c)) {
                allDigits = false;
                break;
            }
        }
        
        if (!allDigits) {
            cout << "Input must contain only digits." << endl;
            continue;
        }
        
        if (input[0] == '0') {
            cout << "First digit cannot be zero." << endl;
            continue;
        }
        
        bool hasDuplicates = false;
        for (int i = 0; i < 4; i++) {
            for (int j = i+1; j < 4; j++) {
                if (input[i] == input[j]) {
                    hasDuplicates = true;
                    break;
                }
            }
            if (hasDuplicates) break;
        }
        
        if (hasDuplicates) {
            cout << "Digits must be unique." << endl;
            continue;
        }
        
        return true;
    }
}

void countBullsAndCows(const string &secret, const string &guess, int &bulls, int &cows) {
    bulls = cows = 0;
    for (int i = 0; i < 4; i++) {
        if (secret[i] == guess[i]) {
            bulls++;
        } else if (secret.find(guess[i]) != string::npos) {
            cows++;
        }
    }
}

//checker for repeating guesses
bool isAlreadyGuessed(const vector<string>& guessHistory, const string& guess) {
    for (const string& prevGuess : guessHistory) {
        if (prevGuess == guess) {
            return true;
        }
    }
    return false;
}


int main() {

    cout << "-----Bulls and Cows!-----\n";


    //difficulty
    string difficultyInput;
    Difficulty selectedDifficulty = Difficulty::Invalid;

    while(selectedDifficulty == Difficulty::Invalid){
        cout << "Please select a difficulty to start the game! (easy/0, medium/1) : ";
        cin >> difficultyInput;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        selectedDifficulty = getDifficultyFromInput(difficultyInput);

        if (selectedDifficulty == Difficulty::Invalid){
            cout << "Invalid input! Please try again." << endl;
        }
    }
    cout << "Your selected difficulty is ";
    switch (selectedDifficulty) {
        case Difficulty::Easy:
            cout << "Easy" << endl;
            break;
        case Difficulty::Medium:
            cout << "Medium" << endl;
            break;
        default:
            break;
    }
    cout << endl;

    //set the code!
    string AiCode = Generator(selectedDifficulty);
    string PlayerCode;
    cout << "Computers Code: " << AiCode << endl;
    cout << "Commit Your Code!" << endl;

    string PlayerCurrGuess;
    string AiCurrGuess;
    vector <string> AiGuessHistory;

    int PlayerBulls = 0;
    int PlayerCows = 0;
    int AiBulls = 0;
    int AiCows = 0;

    while (!intInput(PlayerCode)) {
        cout << "Try again: ";
    }
    cout << "Your secret code is " << PlayerCode << endl;
    cout << endl;
    

    //actual game
    for(int i = 0; PlayerCurrGuess != AiCode && AiCurrGuess != PlayerCode; i++){
        cout << "-----Turn : " << i + 1 << " -----" << endl;

        // player
        cout << "Input your guess!" << endl;
        while (!intInput(PlayerCurrGuess)) {
            cout << "Try again: ";
        }
        countBullsAndCows(AiCode, PlayerCurrGuess, PlayerBulls, PlayerCows);

        // computer
        AiCurrGuess = Generator(selectedDifficulty, AiGuessHistory);
        AiGuessHistory.push_back(AiCurrGuess);
        countBullsAndCows(PlayerCode, AiCurrGuess, AiBulls, AiCows);

        cout << "Your guessed : " << PlayerCurrGuess << ", scoring : " << PlayerBulls << " bulls and " << PlayerCows << " cows!" << endl;
        cout << "Computer guessed : " << AiCurrGuess << ", scoring : " << AiBulls << " bulls and " << AiCows << " cows!" << endl;
    }


    //win or draw
    if (PlayerCurrGuess == AiCode && AiCurrGuess == PlayerCode){
        cout << "It's a tie! Both guessed correctly!" << endl;
    } else if (PlayerCurrGuess == AiCode){
        cout << "Player won!" << endl;
    } else if (AiCurrGuess == PlayerCode){
        cout << "Computer won!" << endl;
    } else {
        cout << "Unable to tally scores." << endl;
    }

    return 0;
}
