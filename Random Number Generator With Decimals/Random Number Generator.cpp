#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <cmath>

struct NumberGenerateInfo
{
    int minimum;
    int maximum;
    int precision;
};

double getRandomNumber(NumberGenerateInfo info);

int main()
{
    // Change this value to generate a specified amount of numbers
    const int NUMBERS_TO_GENERATE = 100;
    // Change this value to handle how many numbers are on each row
    const int NUMBERS_ON_ROW = 10;

    std::ofstream outputFile;
    NumberGenerateInfo info;

    // Define all the bounds and precision of the generated numbers
    info.minimum = 0;
    info.maximum = 100;
    info.precision = 2;
    double generatedNumber;

    outputFile.open("generatedNumbers.txt");
    srand(time(0));

    for (int i = 1; i <= NUMBERS_TO_GENERATE; i++)
    {
        generatedNumber = getRandomNumber(info);
        outputFile << generatedNumber << " ";
        if ((i % NUMBERS_ON_ROW) == 0)
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