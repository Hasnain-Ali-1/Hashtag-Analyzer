/********************
** File: proj1.cpp
** Project: CMSC 202 Project 1, Fall 2022
** Author: Hasnain Ali
** Date: 9/24/22
** Section: 10/15 
** E-mail: hali6@umbc.edu
**
The program takes two files that are user specified, one with just hashtags and another with hashtags and messages. The program then goes through the messages file and checks to see if any
of the messages in the message file contain any hashtags that are found in the hashtag file. The program then prints out the messages with any matching hashtags. After this, the program
prints out the hashtags form the hashtag file as well as an incremented count stating how many times each hashtag was used in the messages file.
********************/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

//--------------------
//Function declarations
//--------------------

void readHashtagFile();
void readMessageFile(string hashArray[], int hashtagSize);
void findMatches(string hashesArray[], int hashesSize, string messagesArray[], int messagesSize);
int messagesLoaded(int messageSize);
void displayCounts(string hashesArray[], int hashesSize, string messagesArray[], int messagesSize);

//--------------------
//Global Constant
//--------------------

const int arraySize = 20;    // Constant for the size of the array
const int spacing = 20;    //Constant for formatting the output

//--------------------
// Main
//--------------------

int main() {
  string userContinue;    // A string that represents the user's choice to continue the program or not
  
  cout << "\n";
  cout << "Welcome to the Hashtag Analyzer \n";
  cout << "You will choose a file of hashtags to search for \n";
  cout << "Then you will choose a file to search for the hashtags \n";
  cout << "\n";
  
  do {
    readHashtagFile();
    cout << "\n";
    cout << "Would you like to analyze another file? \n";
    cin >> userContinue;
    cout << "\n";
    // If the user doesn't enter "n" when prompted to analyze another file, then they will go through the program again unitl they enter "n"
  } while (userContinue != "n"); 
  cout << "Thank you for using the hashtag analyzer! \n";
  cout << "\n";
  return 0;
}

// --------------------
// Name: readHashtagFile
// PreCOndition: None, the program prints a couple statements and then calls this function as soon as the program starts
// PostConsition: The user will have entered a valid hashtag file, and those hashtags have been added to the hashtagArray
// --------------------

void readHashtagFile() {
  bool validHashtagFile = 0;    // If the hashtag file is a valid file or not (0 for no, 1 for yes)
  string line;    // Used when going through the text file
  string hashtagFileName;    // The name of the hashtag file
  int currentSize = 0;    // An int that will be used as the i value when adding hashtags to hashtagArray
  ifstream hashtagFile;    // Used to open the file
  string hashtagArray[arraySize];    // An array that will contain the hashtags from the hashtag file
  char hashtag = '#';    // The character "hashtag"

  // The value of validHashtagFile is currently 0, so the program goes through this loop at least once
  while (validHashtagFile == 0) {  
    cout << "Please enter the file with the hashtags you would like to use: \n";
    cin >> hashtagFileName;    
    hashtagFile.open(hashtagFileName);
    if (hashtagFile.is_open()) {   
      char firstCharacter = hashtagFile.peek();
      // If the first character of the first line isn't a hashtag
      if (firstCharacter != hashtag) { 
	hashtagFile.close();
	cout << "The file entered was not a valid hashtag file \n";
	// Takes the user back to the beginning of the while loop
	validHashtagFile = 0;
      }
      else {
      while (getline (hashtagFile, line)) {
	// Takes each line from the hashtag file and adds it to hashtagArray[currentSize], and each time it does so currentSize is incremented by 1
	hashtagArray[currentSize] = line;
	currentSize ++;
      }
      hashtagFile.close();
      // Takes the user out of the while loop since a valid hashtag file has been entered
      validHashtagFile = 1;
      }
    }
    else {
      cout << "The file entered is not in the directory \n"; 
      // If the file can't open, the user will be taken back to the beginning of the while loop
      validHashtagFile = 0;
    }
  }
  cout << "Your file was imported! \n";
  readMessageFile(hashtagArray, currentSize);
}

//--------------------
// Name: readMessageFile
// PreCondition: hashArray has values that are lines from the user's hashtag file and hashtagSize is the current size of hashArray
// PostCondition: messageArray will have values that are from the user's message file, currentSize is the size of messageArray, and hashArray and hashtagSize retain the same values
//--------------------

void readMessageFile(string hashArray[], int hashtagSize) {
  bool validMessageFile = 0;    // If the message file is a valid file or not (0 for no, 1 for yes)
  string line;    // Used to go through the message file
  string messageFileName;    // The name of the message file
  int currentSize = 0;    // An int that will be used as the i value when adding hashtags to messagesArray
  ifstream messageFile;    // Used to open the file
  string messageArray[arraySize];    // A message that will contain the hashtags from the message file
  char hashtag = '#';    // The character "hashtag"

  // The value of validMessageFile is 0, so the while loop is run at least once
  while (validMessageFile == 0) {   
    cout << "Please enter the file with the messages you would like to parse: \n";
    cin >> messageFileName;
    messageFile.open(messageFileName);
    if (messageFile.is_open()) {
      char firstCharacter = messageFile.peek();
      // If the first character of the first line isn't a hashtag
      if (firstCharacter != hashtag) {
        messageFile.close();
        cout << "The file entered was not a valid message file \n";
        // Takes the user back to the beginning of the while loop
        validMessageFile = 0;
      }
      else {
	while (getline (messageFile, line)) {
	  // Takes each line from the message file and adds it to messageArray[currentSize], and each time it does so currentSize is incremented by 1
	  messageArray[currentSize] = line;
	  currentSize ++;
	}
	messageFile.close();
	// Takes the user out of the while loop since a valid message file is entered
	validMessageFile = 1;
      }
    }
    else {
      cout << "The file entered is not in the directory \n";
      // Keeps the user in the while loop because the program was unable to open the message file the user entered
      validMessageFile = 0;
    }
  }
  cout << "Your file was imported! \n";
  findMatches(hashArray, hashtagSize, messageArray, currentSize); 
}

//--------------------
// Name: findMatches
// PreCondition: hashesArray has values that are lines from the hashtag file, messagesArray has values that are lines from the message file, and hashesSize is the size of hashesArray and
//               messagesSize is the size of messagesArray
// PostCondition: The number of messages and hashtags from the files will be printed out. Additionally, the messages that have matching hashtags will be numbered and printed out
//--------------------

void findMatches(string hashesArray[], int hashesSize, string messagesArray[], int messagesSize) {
  int matchCount = 0;    // An int that will be used to count the number of messages with matching hashtags from the hashtag list
  int messageLoaded;    // An int that will tell us how many messages were loaded from the message file
  
  // Calls the function messagesLoaded with the size of the messageArray, messagesSize, being the parameter
  messageLoaded = messagesLoaded(messagesSize); 
  cout << "\n";
  cout << messageLoaded << " messages loaded \n"; 
  cout << hashesSize << " hashtags loaded \n";    
  cout << "\n";
  cout << "Messages with matching hashtags:\n";
  // Iterates through both hashesArray and messagesArray
  for (int i = 0; i < hashesSize; i++) {
    for (int j = 0; j < messagesSize; j++) {  
      if (hashesArray[i] == messagesArray[j]) {
	// Keeps track of the current count of the number of messages with matching hashtags
	matchCount = matchCount + 1;
	// Prints out current count, the hashtag, and the message using the hashtag (the first line in the message file is the hashtag, the following line will be the message, hence [j+1]
	cout << matchCount << "." << "\t" <<  messagesArray[j] << " " << messagesArray[j+1] << "\n";   
      }
    }
  }
  displayCounts(hashesArray, hashesSize, messagesArray, messagesSize);    
}

//--------------------
// Name: messagesLoaded
// PreCondition: That the value for messageSize is an integer
// PostCondition; Returns a value for the number of messages that have been loaded to the findMatches function
//--------------------

int messagesLoaded(int messageSize) {
  // Divides the original messageSize by 2 because the message file contains the hashtag in the first line and then the actual message in the second; 2 lines per message esentially
  messageSize = messageSize/2; 
  return messageSize;
}

//--------------------
// Name: displayMatches
// PreCondition: hashesArray has values that are lines from the hashtag file, messagesArray has values that are lines from the message file, and hashesSize is the size of hashesArray and
//               messagesSize is the size of messagesArray
// PostCondition: Prints out the hashtags from the hashtag list as well as the number of times the hashtag was used in matching messages
//--------------------

void displayCounts(string hashesArray[], int hashesSize, string messagesArray[], int messageSize) {
  int countMatches[hashesSize] = {0};    // A new array that counts how many matches occur for each hashtag
  int hashtagNum = 1; // Keeps track of the hashtag number when printing them
  
  // Iterates through hashesArray and messagesArray
  for (int i = 0; i < hashesSize; i++) {
    for (int j = 0; j < messageSize; j++) {
      // If a match is found between hashesArray[i] and messagesArray[j], countMatches[i] increases by one
      if (hashesArray[i] == messagesArray[j]) {
	countMatches[i] = countMatches[i] + 1;
      }
      else {
	countMatches[i] = countMatches[i] + 0;
      }
    }	  
  }
  cout << "\n";
  for (int i = 0; i < hashesSize; i++) {
    // Prints the hashtag number, the hashtag itself, and the number of matches each hashtag has
    cout << left << setw(spacing) << hashtagNum << setw(spacing) << hashesArray[i] << setw(spacing) << countMatches[i] << "\n";
    hashtagNum = hashtagNum + 1;
  }
}
