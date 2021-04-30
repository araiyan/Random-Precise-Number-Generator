#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <string>

struct NumberGenerateInfo
{
    int minimum;
    int maximum;
    int precision;
};

void getNumbersInfo(NumberGenerateInfo& info, int& numbersToGenerate,
    int& numberOnEachRow);
int getInput(std::string prompt, bool canBeNegative);
double getRandomNumber(NumberGenerateInfo info);

int main()
{
    std::ofstream outputFile;
    NumberGenerateInfo info;

    int numbersToGenerate, numberOnEachRow;
    double generatedNumber;

    outputFile.open("generatedNumbers.txt");
    srand(time(0));

    getNumbersInfo(info, numbersToGenerate, numberOnEachRow);
    for (int i = 1; i <= numbersToGenerate; i++)
    {
        generatedNumber = getRandomNumber(info);
        outputFile << generatedNumber << " ";
        if ((i % numberOnEachRow) == 0)
            outputFile << std::endl;
    }
}

// gets a random number based on the info of that number
double getRandomNumber(NumberGenerateInfo info)
{
    int range = abs(info.maximum - info.minimum);
    int precisionMultipier = (int)(pow(10, info.precision));

    return (double)((rand() % (range * precisionMultipier)) + info.minimum)
        / precisionMultipier;
}

// gets the information fromt he user about the number they want to generate
void getNumbersInfo(NumberGenerateInfo& info, int& numbersToGenerate,
    int& numberOnEachRow)
{
    numbersToGenerate = 
        getInput("Amount of numbers you want to generate: ", false);
    numberOnEachRow = 
        getInput("Numbers you want to have on each row: ", false);
    info.minimum = getInput("Minimum number: ", true);
    info.maximum = getInput("Maximum number: ", true);
    info.precision = 
        getInput("Number of decimal place precision for your number: ", false);
    
}

// function to check user input
int getInput(std::string prompt, bool canBeNegative)
{
    int userInput;
    std::string userStringInput;

    do
    {
        std::cout << prompt;
        std::cin >> userStringInput;

    } while(([userStringInput, canBeNegative, &userInput](){
        int i = 0;
        while (userStringInput[i])
            if (!std::isdigit(userStringInput[i++]))
            {
                std::cout << "The input has to be an integer" << std::endl;
                return true;
            }
        userInput = std::stoi(userStringInput);
        if (!canBeNegative && (userInput < 0)) return true;
        return false;
        })());

    return userInput;
}