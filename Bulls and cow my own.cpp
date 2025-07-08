#include <iostream>
#include <string>
#include <cstdlib>  // for rand()
/*
 	TO DO LIST 
 	- function for rng (COMPLETE)
 	- function for check if code has no repitions 
 	- function for bulls and cows
*/

using namespace std;

//Function to check for any repetitions
bool checker(int code[],int CurrentDigit, int digit){
 
 	for (int i = 0; i < 4; i++){
	 
	 	if (code[i] == digit){	
	    	return true;
	 	}
	 }
	 return false;
}

// Function for generating numbers + converting to string. 
string Generator() {
    int code[4];
	
	//Makes sure that the first digit is not 0.
    code[0] = rand() % 9 + 1;
	
	//loop to get remaining last digits
    for (int i = 1; i < 4; i++) {
	int digit; //used to hold placement of remaining digits	
    do {
       digit = rand() % 10;
    }while (checker(code, i, digit));
    code [i] = digit;
    }

    // Converts to String
    string result = "";
    for (int i = 0; i < 4; i++) {
        result += to_string(code[i]);
    }
    return result;
}


int main() {

    cout << "Hello World\n";

    string AiCode = Generator();
    cout << "Computers Code: " << AiCode << endl;

    return 0;
}
