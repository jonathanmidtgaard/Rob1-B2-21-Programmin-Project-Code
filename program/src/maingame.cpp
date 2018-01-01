#include <iostream>
#include <ctime>
#include <string>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int8.h>
#include <sstream>
using namespace std;

//global variables
#define MAX_TRIES 5
ros::Publisher name_pub;
ros::Publisher score_pub;
int score_temp = 0; //temporary score of game

//define the functions so the order doesn't matter
int letterFill(char, string, string&);
int mainGame(string catPointer[], int arrSize);
void Getplayer();
void save();

//player info gathered in a struct
struct PlayerInfo{
    string              PlayerName;
    unsigned int        PlayerScore;
};

PlayerInfo p1;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "Hangman_maingame_node");
    ros::NodeHandle n;
    name_pub = n.advertise<std_msgs::String>("Name", 10);
    score_pub = n.advertise<std_msgs::Int8>("Score", 10);

    Getplayer();

    srand(time(NULL));

    // welcome the user
    cout << "\n\nHi ";
    cout << p1.PlayerName;
    cout << "!\nWelcome to hangman!" << endl;

    // Ask user for a category
    string cat;
    int lastTry;
    while(1)
    {
        lastTry = 0;
        cout << "\nEnter your prefered category (countries, animals, car_brands, capitals):" << endl;
        cat ="";
        cin >> cat;

        // Check category
        if (cat == "countries")
        {
            //put all the string inside the array here
            string countries[] = { "india", "japan", "nepal", "china", "madagascar", "azerbaijan", "denmark", "argentina", "sweden", "canada", "ireland", "mongolia", "zimbabwe", "venezuela" };
            int numElements = sizeof(countries) / sizeof(countries[0]); //counts the number of elements in my string "countries"
            lastTry = mainGame(countries,numElements);
        }
        else if (cat == "animals")
        {
            string animals[] = { "leopard", "lizard", "tortoise", "flamingo", "bullfrog", "dingo", "hedgehog", "koala", "piranha" };
            int numElements = sizeof(animals) / sizeof(animals[0]);
            lastTry = mainGame(animals,numElements);
        }
        else if (cat == "car_brands")
            {
            string car_brands[] = { "toyota", "citroen", "ferrari", "jeep", "lamborghini", "porsche", "suzuki", "nissan", "mclaren", "mercedes-benz", "mini", "jaguar", "hyundai" };
            int numElements = sizeof(car_brands) / sizeof(car_brands[0]);
            lastTry = mainGame(car_brands,numElements);
        }
        else if (cat == "capitals")
            {
            string capitals[] = { "copenhagen", "london", "brussels", "beijing", "prague", "helsinki", "paris", "berlin", "athens", "nuuk", "baghdad", "rome", "tokyo", "amsterdam", "stockholm" };
            int numElements = sizeof(capitals) / sizeof(capitals[0]);
            lastTry = mainGame(capitals,numElements);
            }

        if (lastTry == 1) //the game returns 1 if you win, so if you won, add the number of guesses left to the player score
        {
            p1.PlayerScore += score_temp; //the number of guesses left will be added to the score
            cout << "Score:";
            cout << p1.PlayerScore << endl;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

//define function that ask for and save the player's name
void Getplayer()
{
    cout << "Enter Name:  ";
    cin >> p1.PlayerName;
    cin.sync();
}

//define function where the main game is played until all 5 guesses are used
int mainGame(string catPointer[],int arrSize)
{
    char letter;
    string word;
    int num_of_wrong_guesses=0;
    int n = rand() % arrSize; //finds  random number between 0 and the arraylength
    word = catPointer[n];

    // Initialize the secret word with the * character.
    string unknown(word.length(), '*');
    cout << "\n\nEach letter is represented by an asterisk.";
    cout << "\n\nYou have to type only one letter in one try.";
    cout << "\n\nYou have " << MAX_TRIES << " tries to try and guess the word.";
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    // Loop until the guesses are used up
    while (num_of_wrong_guesses < MAX_TRIES)
    {
        cout << "\n\n" << unknown;
        cout << "\n\nGuess a letter: ";
        cin >> letter;
        // Fill secret word with letter if the guess is correct,
        // otherwise increment the number of wrong guesses.
        if (letterFill(letter, word, unknown) == 0)
        {
            cout << endl << "Wrong guess" << endl;
            num_of_wrong_guesses++;
        }
        else
        {
            cout << endl << "Correct! You have found a letter!" << endl;
        }
        // Tell user how many guesses it has left.
        cout << "You have " << MAX_TRIES - num_of_wrong_guesses;
        cout << " guesses left." << endl;

	//Set the temporary score to the number of guesses left
        score_temp = MAX_TRIES - num_of_wrong_guesses;

	// Check if user guessed the word.
        if (word == unknown)
        {
            cout << word << endl;
            return 1;
        }
    }
    if (num_of_wrong_guesses == MAX_TRIES)
    {
        cout << "\nSorry, you lose...you've been hanged." << endl;
        cout << "The word was : " << word << endl;

        //publish the score and name
        save();
    }
    cin.ignore();
    cin.get();
    return 0;
}

//define function that checks if the guessed letter is correct or not
int letterFill(char guess, string secretword, string &guessword)
{
    int matches = 0; //number of correct guesses
    for (int i = 0; i<secretword.length(); i++)
    {
        // Did we already match this letter in a previous guess?
        if (guess == guessword[i])
            return 0;
        // Is the guess in the secret word?
        if (guess == secretword[i])
        {
            guessword[i] = guess;
            matches++;
        }
    }
    return matches;
}

void save(){

  std_msgs::String msg_string;
  std_msgs::Int8 msg_int;

  std::stringstream ss;
  ss << p1.PlayerName;
  msg_string.data = ss.str();
  msg_int.data = p1.PlayerScore;

  name_pub.publish(msg_string);
  usleep(100000); //delay 0.1 sec
  score_pub.publish(msg_int);
}
