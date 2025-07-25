#include <iostream>
#include <string>
#include <cstdlib>  // for rand()
#include <limits> //for int inputs
#include <vector> // for guess history
#include <fstream> // for file writing

/*
 	TO DO LIST 
 	- function for rng (COMPLETE)
 	- function for check if code has no repitions (complete)
 	- function for bulls and cows (complete)
 	- AI Guess checker (complete)
 	- choice for easy or medium ai (complete)
 	- File Trackers for AI ( Compelted ) 
 	- File Tracker for Player ( completed ) 
 	- Saving ??? Comment on line 329. ( TLDR: Code automatically saves need to change it to promt to save user guess ) 
 			- My somewhat skewed logic for file saving
 			  	- when asked if user wants to save, if yes then combine the contents of AIGuess.txt with PlayerGuess.txt then ask
 			  	user what to name the file. 
 	- Doesn't show results in Files
 	

    MEMBERS
    -   Gabriel Barrientos
	-   Danial Ahmed (Zoey)
*/

using namespace std;

enum Difficulty {
        Easy,
        Medium,
        InvalidDifficulty
};
enum PlayMode {
        Manual,
        Automatic,
        InvalidPlayMode
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
        return Difficulty::InvalidDifficulty;
    }
}

PlayMode getPlayModeFromInput(const string& input){
    string lowerInput = toLower(input);

    if (lowerInput == "manual" || lowerInput == "0") {
        return PlayMode::Manual;
    } else if (lowerInput == "auto" || lowerInput == "automatic" || lowerInput == "1") {
        return PlayMode::Automatic;
    } else {
        return PlayMode::InvalidPlayMode;
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

// Function to combine guess files 
void combineFilesAndSave(const string& newFilename, const string& resultSummary) {
    ifstream playerFile("PlayerGuess.txt");
    ifstream aiFile("AIGuess.txt");
    ofstream combinedFile(newFilename);

    if (!playerFile.is_open() || !aiFile.is_open() || !combinedFile.is_open()) {
        cout << "Error opening one of the files\n";
        return;
    }

    string line;

    combinedFile << "=== Player Guess Log ===\n";
    while (getline(playerFile, line)) {
        combinedFile << line << '\n';
    }

    combinedFile << "\n=== AI Guess Log ===\n";
    while (getline(aiFile, line)) {
        combinedFile << line << '\n';
    }

    combinedFile << "\n=== Game Result ===\n";
    combinedFile << resultSummary << '\n';

    playerFile.close();
    aiFile.close();
    combinedFile.close();

    cout << "Game log saved to: " << newFilename << '\n';
}
/* ---------------------------------------- */


int main() {

    cout << "-----Bulls and Cows!-----" << endl;
    
    //file input
    string playModeInput;
    PlayMode selectedPlayMode = PlayMode::InvalidPlayMode;
    
    while(selectedPlayMode == PlayMode::InvalidPlayMode){
        cout << "Would you wish to enter your guesses manually or automatically through a pre supplied file?" << endl;
        cout << "0/Manual" << endl;
        cout << "1/Automatic" << endl;
        cin >> playModeInput;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        selectedPlayMode = getPlayModeFromInput(playModeInput);
    
        if (selectedPlayMode == PlayMode::InvalidPlayMode){
            cout << "Invalid input! Please try again." << endl;
        }
    }
    cout << "Your selected input style is ";
    switch (selectedPlayMode) {
        case PlayMode::Manual:
            cout << "Manual" << endl;
            break;
        case PlayMode::Automatic:
            cout << "Automatic" << endl;
            break;
        default:
            break;
    }
    cout << endl;

    //difficulty
    string difficultyInput;
    Difficulty selectedDifficulty = Difficulty::InvalidDifficulty;

    while(selectedDifficulty == Difficulty::InvalidDifficulty){
        cout << "Please select a difficulty to start the game!" << endl;
        cout << "0/Easy" << endl;
        cout << "1/Medium" << endl;
        cin >> difficultyInput;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        selectedDifficulty = getDifficultyFromInput(difficultyInput);

        if (selectedDifficulty == Difficulty::InvalidDifficulty){
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
    
    // Storing AI GUess history
	vector<pair<int, int>> AiResults;


    int PlayerBulls = 0;
    int PlayerCows = 0;
    int AiBulls = 0;
    int AiCows = 0;

    while (!intInput(PlayerCode)) {
        cout << "Try again: ";
    }
    cout << "Your secret code is " << PlayerCode << endl;
    cout << endl;

    // storing player guess history and results 
    vector<string> PlayerGuessHistory;
    vector<pair<int, int>> PlayerResults;
 

    //actual game
    for(int i = 0; PlayerCurrGuess != AiCode && AiCurrGuess != PlayerCode; i++){
        cout << "-----Turn : " << i + 1 << " -----" << endl;

        // player
        cout << "Input your guess!" << endl;
        while (!intInput(PlayerCurrGuess)) {
            cout << "Try again: ";
        }
        countBullsAndCows(AiCode, PlayerCurrGuess, PlayerBulls, PlayerCows);

        // Store player guess and result 
        PlayerGuessHistory.push_back(PlayerCurrGuess);
        PlayerResults.push_back({PlayerBulls, PlayerCows});
       

        // computer
        AiCurrGuess = Generator(selectedDifficulty, AiGuessHistory);
        AiGuessHistory.push_back(AiCurrGuess);
        countBullsAndCows(PlayerCode, AiCurrGuess, AiBulls, AiCows);
        
        // Store AI guess and result 
		AiResults.push_back({AiBulls, AiCows});


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
    
 	//Ask user if they want to save combined log 
    char saveChoice;
    cout << "Do you want to save the game log? (y/n): ";
    cin >> saveChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (tolower(saveChoice) == 'y') {
        string outputFilename;
        cout << "Enter the filename to save the game log(Eg: GameLog.txt): ";
        getline(cin, outputFilename);

        string gameResult;
        if (PlayerCurrGuess == AiCode && AiCurrGuess == PlayerCode){
            gameResult = "It's a tie! Both guessed correctly!";
        } else if (PlayerCurrGuess == AiCode){
            gameResult = "Player won!";
        } else if (AiCurrGuess == PlayerCode){
            gameResult = "Computer won!";
        } else {
            gameResult = "Unable to tally scores.";
        }

        combineFilesAndSave(outputFilename, gameResult);
    }



    /* Save player guesses to a file */
    ofstream playerFile("PlayerGuess.txt");
    if (playerFile.is_open()) {
        playerFile << "Player's Secret Code: " << PlayerCode << "\n";
        playerFile << "Computer's Secret Code: " << AiCode << "\n\n";
        playerFile << "Player Guess History:\n";

        for (size_t i = 0; i < PlayerGuessHistory.size(); ++i) {
            playerFile << "Turn " << (i + 1) << ": Guess = " << PlayerGuessHistory[i]
                       << " | Bulls = " << PlayerResults[i].first
                       << ", Cows = " << PlayerResults[i].second << "\n";
        }

        playerFile.close();

    } else {
        cout << "SYSTEM ERROR\n";
    }

    
    /* Save AI guesses to a file */
	ofstream aiFile("AIGuess.txt");
	
	if (aiFile.is_open()) {	
	    aiFile << "AI Secret Code: " << AiCode << "\n\n";
	    aiFile << "AI Guess History:\n";
	
	    for (size_t i = 0; i < AiGuessHistory.size(); ++i) {
	        aiFile << "Turn " << (i + 1) << ": Guess = " << AiGuessHistory[i]
	               << " | Bulls = " << AiResults[i].first
	               << ", Cows = " << AiResults[i].second << "\n";
	    }
	
	    aiFile.close();
	    
	} else {
	    cout << "SYSTEM ERROR\n";
	}


    return 0;
}
