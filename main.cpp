#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;

struct word_freq
{
    string word;
    int frequency;
};

string to_lower (string instruction);
string strip(string in);
bool readFile (string file_name, vector<string> &words);
int findWord (string word, vector <word_freq> &wordfrequency);
int topWord (vector <word_freq> wordfrequency);
void freq_to_file (int count, vector <word_freq> &wordfrequency);
int getCount (vector <word_freq> vfinal);

int main()
{
    vector <string> words_from_file; // a vector where all the words from the file will be added
    vector <word_freq> vfinal; // vector which will hold the words and their frequency from file
    word_freq new_word;
    
    string file_name;
    cout << "What is the name/path of the file: ";
    cin >> file_name;
    while (!readFile(file_name, words_from_file)) // makes sure that the file can be read, otherwise asks for another input
    {
        cout << "File not found." << endl;
        cout << "What is the name/path of the file: ";
        cin >> file_name;
    }
    
    int index;
    for (int i = 0; i < words_from_file.size(); i++) // this is where the words from the file get counted
    {
        index = findWord(words_from_file[i], vfinal); // if the word exists we get an index value, otherwise we get -1
        if (index >= 0) // if the index is greater than or equal to 0 it means the word exists in the vector
            vfinal[index].frequency++; // increases the count of the word by 1
        else // the word does not currently exist in the vector
        {
            new_word.word = words_from_file[i]; // adds the word to the word struct
            new_word.frequency = 1; // sets the count to 1
            vfinal.push_back(new_word); // pushes it into the vector
        }
    }
    
    cout << "There are " << vfinal.size() << " distinct words in this text." << endl; // tells the user how many words there are in the text
    int howmany = getCount(vfinal); // asks for how many words the user wants
    
    while (howmany > vfinal.size() || howmany < 1) // makes sure the user enters a valid input
    {
        cout << "Invalid input." << endl;
        howmany = getCount(vfinal);
    }
    
    freq_to_file(howmany, vfinal); // writes everything to file
    cout << "Everything you have requested has been put in a file titled: Word Counts.txt" << endl;
    
    return 0;
}

// function to make all letters lowercase
string to_lower(string instruction)
{
    if (instruction.length() == 1 && instruction == "I") // if the word is "I" do not change to lowercase
        return instruction;

    std::locale loc;
    string b = instruction;
    for (std::string::size_type i=0; i<instruction.length(); ++i)
    {
        b[i] = std::tolower(instruction[i],loc); // makes every letter lowercase and puts it into string b
    }
    return b;
}

// function to remove anything that is not a letter in the alphabet
string strip(string in)
{
    string final;
    for(int i = 0; i < in.length(); i++)
    {
        if(isalpha(in[i])) // if it is a letter, it keeps it adds it to the end of the final string, otherwise it ignores it and moves on
            final += in[i];
    }
    return final;
}

// function to read the file and put all the words into an vector
bool readFile (string file_name, vector<string> &words)
{
    ifstream in; // open in stream
    in.open(file_name); // opens the file
    if (!in.is_open())
        return false;
    char* word = new char [100]; // char variable for the word
    string string_of_word; // string variable for the string that will be added to the struct
    while (in.peek() != EOF) // as long as it is not the end of the file
    {
        in.getline(word, 100, ' '); // get the line up until 100 characters or until there is a space
        string_of_word = to_lower(word); // makes everything lowercase and puts it into string_of_word
        string_of_word = strip(string_of_word); // removes anything that is not a letter
        words.push_back(string_of_word); // pushes this to the vector that contains all the individual words from the file
    }
    in.clear(); // clear
    in.close(); // close the stream
    return true;
}

// function to find if a word is in the array, and return its index
int findWord (string word, vector<word_freq> &wordfrequency)
{
    for (int i = 0; i < wordfrequency.size(); i++) // looks through every index for the word
        if (word == wordfrequency[i].word)
            return i; // returns index if found
    return -1; // returns -1 if word not found
}

// function to find the highest frequncy word in the vector and return its index
int topWord (vector <word_freq> wordfrequency)
{
    int index = -1; // to keep track of the index
    int currentHighest = 0; // to keep track of the highest value
    for (int i = 0; i < wordfrequency.size(); i ++) // looks through every index looking at the frequencies of the word
    {
        if (wordfrequency[i].frequency > currentHighest) // compares to see if the current index has a higher frequency than currentHighest variable
        {
            index = i; // remembers its index
            currentHighest = wordfrequency[i].frequency; // and its frequency
        }
    }
    return index; // returns the index of the word with the highest frequency
}

// function to write to file the words and their frequency
void freq_to_file (int count, vector <word_freq> &wordfrequency)
{
    int index;
    ofstream out; // opens out stream
    out.open("Word Counts.txt"); // file name to be output
    out << "Frequency\tWord" << endl;
    out << endl;
    
    int total_words = 0;
    for (int i = 0; i < wordfrequency.size(); i++) // this will count the total number of words from the file
        total_words += wordfrequency[i].frequency;

    for (int i = 0; i < count; i++) // for the number of words the user requested
    {
        index = topWord(wordfrequency); // finds the highest frequency word
        out << wordfrequency[index].frequency << "\t\t" << wordfrequency[index].word << endl; // writes out its info to file
        wordfrequency[index].frequency = 0; // then sets its frequency to 0 so that it will not come back up again
    }
    
    out << endl;
    out << "Total number of words: " << total_words << endl;
    out.clear(); // clear
    out.close(); // close output stream
}

// function that requests number from user for number of words to return
int getCount (vector <word_freq> vfinal)
{
    string count = "-1";
    cout << "Enter number a number between 1 and " << vfinal.size() << " for number of words you would like the frequency of: ";
    cin >> count;
    
    for (int i = 0; i < count.length(); i++) // this will go through each character in the string the user input to ensure that it is numbers only
    {
        if (isalpha(count[i])) // if there is a letter in it, return -1
            return -1;
        if (ispunct(count[i])) // if there is a punction mark in it, return -1
            return -1;
    }
    return stoi(count); // if its only numbers, return the string as an int
}