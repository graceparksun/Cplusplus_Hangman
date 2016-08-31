//File Name: Final Project
//Author: Grace Park
//Email Address: graceparksun@gmail.com
//Assignment Number: 1
//Description: Playing Hangman.
//Last Changed: August 7, 2014


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <vector>


using namespace std;


const int STARTING_X= 70, STARTING_Y= 6; //The starting position of the hangman image.


void Opening ();
//Opening Statement for the game.
//Explains how the game is played and how to win or lose.
void Open_File(ifstream& fin, vector<string>& words);
//Opens file named "hangman_words.txt" for the game.
void Get_Word (vector<string>& words, string& word_original, string& word_display);
//Gets a word from the input file. A random function is used so
//the words are different everytime.
void Display_word (string word_display);
//Used in the "Guessing" function to display the changed/unchanged word
//after user's guessing.
void Guessing (string word_original, string& word_display, vector<char>& letter_chosen, int& right, int& wrong);
//User guesses the letter in the word.
//1. Checks if the input is valid: one character.
//2. Checks if the user have already chosen the letter or not.
//3. Checks if user's guess is right.
//4. If the user's guess is not right, tells the user that it is wrong.
//   If the user's guess is right, tells the user how many letters in the word.
void Next_draw (int wrong);
//Used in "Guessing" function when the user's guess is wrong.
//Uses switch statement to draw the next part of the hangman.
void gotoxy(int column, int row);
//Function inputs the position by column and row of the console.
//The next input starts on the decided column and row.
void Hangman_Initial();
//The structure of the hangman without the actual man figure.
//Stays in the initial position.
void Win_Statement(string word_original);
//Outputs that the user won the game.
void Lose_Statement(string word_original);
//Outputs that the user lost the game.
void Repeat (char& ans);
//Asks the user if they wish to play another game.
void No_more_words ();
void Goodbye ();
//Goodbye statement.
void Clear_last ();



int main()
{
    srand(time(0));

    char ans;
    ifstream fin;
    vector<string> words;

    Open_File(fin, words);
    int number_words = words.size();

    do
    {
        system("CLS"); //clears console.

        string word_original, word_display = "";
        vector<char> letter_chosen;
        int right = 0, wrong = 0;
        const int HANGMAN_FINAL = 6;

        Opening ();
        Get_Word (words, word_original, word_display);
        Hangman_Initial();

        while (right < word_original.length() && wrong < HANGMAN_FINAL)
        {
            gotoxy(0, STARTING_Y+9);
            Guessing (word_original, word_display, letter_chosen, right, wrong);
            gotoxy(0, STARTING_Y+4);
            Display_word (word_display);
            gotoxy(0, STARTING_Y+9);
            Clear_last ();

        }

        if (right == word_original.length())
            Win_Statement(word_original);
        else if (wrong == HANGMAN_FINAL)
            Lose_Statement(word_original);

        Repeat (ans);
        number_words--;

    } while ((ans == 'y' || ans == 'Y') && number_words >0);

    if (number_words == 0)
    {
        No_more_words ();
        Goodbye ();
    }
    else
        Goodbye ();

    return 0;
}



void Open_File(ifstream& fin, vector<string>& words)
{
    string word;

    fin.open("hangman_words.txt");
    if (fin.fail())
    {
        cout << "Input file opening failed.\n";
        exit(1);
    }

    while (! fin.eof())
    {
        fin >> word;
        words.push_back(word);
    }
}


void Opening ()
{
    cout << "Hi, welcome to C++ HANGMAN! \n\n"
         << "You are going to guess the letters in a word.\n"
         << "Each time you are wrong, a part of the stick figure will be drawn.\n"
         << "You lose when the stick figure is finished but \n"
         << "if you guess the word correctly before it is, then you win.\n\n"
         << "Let's begin! \n"
         << endl;
}


void Get_Word (vector<string>& words, string& word_original, string& word_display)
{
    string line = "_";;
    int index;

    index = (rand() % words.size());
    word_original = words[index];
    cout << endl;

    for (unsigned int i=0; i<word_original.length(); i++)
    {
        word_display = word_display + line;
        cout << word_display[i] << " " ;
    }
    cout << endl;

    words.erase(words.begin() + index);
}



void Display_word (string word_display)
{
    for (unsigned int i=0; i<word_display.length(); i++)
        cout << word_display[i] << " " ;
    cout << endl << endl;
}


void Guessing (string word_original, string& word_display, vector<char>& letter_chosen, int& right, int& wrong)
{
    char guess_ch;
    string line;
    int right_initial = right;
    int guess_number = 0;

    cout << "Guess a letter: ";
    getline(cin,line);

    //1. Checks if the input is valid: one character.
    if (line.length() > 1 || ! isalpha(line[0]))
    {
        cout << "Wrong input. Try again.\n";
        return;
    }
    else
        guess_ch = line[0];
    guess_ch = tolower(guess_ch);

    //2. Checks if the user have already chosen the letter or not.
    if (wrong == 0 && right == 0)
        letter_chosen.push_back(guess_ch);
    else
    {
        for (unsigned int k=0; k<letter_chosen.size(); k++)
        {
            if (letter_chosen.at(k) == guess_ch)
            {
                cout << "Letter already chosen.          \n";
                return;
            }
        }
    letter_chosen.push_back(guess_ch);
    }

    //3. Checks if user's guess is right.
    for (int i=0; i<word_original.length(); i++)
    {
        if (word_original.at(i) == guess_ch)
        {
            word_display.at(i) = guess_ch;
            right ++;
            guess_number++;
        }
    }

    //4. If the user's guess is not right, tells the user that it is wrong.
    //   If the user's guess is right, tells the user how many letters in the word.
    if (right == right_initial)
    {
        wrong ++;
        cout << "wrong.                         \n";
        gotoxy(STARTING_X - 11, STARTING_Y + 7 + wrong);
        cout << "No '" << guess_ch << "' in this word. \n" << endl;
        Next_draw (wrong);
    }
    else
        cout << guess_number << " '" << guess_ch << "' in this word. \n" << endl;

    cout << endl;
}


void Next_draw (int wrong)
{
    switch (wrong)
    {
    case 1:
        gotoxy(STARTING_X, STARTING_Y+3);
        cout << "O";
        break;
    case 2:
        gotoxy(STARTING_X, STARTING_Y+4);
        cout << "|";
        break;
    case 3:
        gotoxy(STARTING_X-1, STARTING_Y+4);
        cout << "/";
        break;
    case 4:
        gotoxy(STARTING_X+1, STARTING_Y+4);
        cout << "\\";
        break;
    case 5:
        gotoxy(STARTING_X-1, STARTING_Y+5);
        cout << "/";
        break;
    case 6:
        gotoxy(STARTING_X+1, STARTING_Y+5 );
        cout << "\\";
        break;
    }
}


void gotoxy(int column, int row)
{
    COORD coord;
    coord.X = column;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}



void Hangman_Initial()
{
    /*
    cout << "         _______    " << endl
         << "         |     |    " << endl
         << "         |     |    " << endl
         << "         O     |    " << endl
         << "        /|\\    |   " << endl
         << "        / \\    |   " << endl
         << "             __|    " << endl
         << endl;
    */


    for (int i=0; i<7; i++)
    {
        gotoxy(STARTING_X + i, STARTING_Y);
        cout << "_";
    }

    for (int j=0; j<6; j++)
    {
        gotoxy(STARTING_X + 7, STARTING_Y + 1 +j);
        cout << "|";
    }
    for (int k=0; k<2; k++)
    {
        gotoxy(STARTING_X, STARTING_Y + 1 + k);
        cout << "|";
    }
    gotoxy(STARTING_X + 6, STARTING_Y + 6);
    cout << "_" << endl;
}


void Win_Statement(string word_original)
{
    cout << "Congratulations, you WIN!! :) \n"
         << "The word was \""
         << word_original
         << "\"\n" << endl;
}


void Lose_Statement(string word_original)
{
    cout << "Oh no, you LOSE ... :( \n"
         << "The word was \""
         << word_original
         << "\"\n" << endl;
}



void Repeat (char& ans)
{
    cout << "Do you wish to play another game? \n"
         << "If yes, insert Y and if no insert N. Then press enter.\n";
    cout << "Ans: ";
    cin >> ans;
    cin.ignore();
    cout << endl;
}



void No_more_words ()
{
    cout << "Sorry, no more words. \n";
}


void Goodbye ()
{
    cout << "Thank you for playing C++ HANGMAN. Goodbye!\n";
}


void Clear_last ()
{
    int num_space = 20, num_line = 1;

    for (int i=0; i<num_line; i++)
    {
        for (int j=0; j<num_space; j++)
            cout << " ";
        cout << endl;
    }
}




/*
Sample Run:


Hi, welcome to C++ HANGMAN!

You are going to guess the letters in a word.
Each time you are wrong, a part of the stick figure will be drawn.
You lose when the stick figure is finished but
if you guess the word correctly before it is, then you win.
                                                                      _______
Let's begin!                                                          |      |
                                                                      |      |
                                                                      O      |
c o m p u t e r                                                      /|      |
                                                                             |
                                                                            _|

                                                           No 'v' in this word.
                                                           No 'i' in this word.
Congratulations, you WIN!! :)                              No 'z' in this word.
The word was "computer."

Do you wish to play another game?
If yes, insert Y and if no insert N. Then press enter.
Ans: n

Thank you for playing C++ HANGMAN. Goodbye!
Press <RETURN> to close this window...



*/
