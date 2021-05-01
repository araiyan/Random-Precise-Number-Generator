#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <windows.h>
#include <iomanip>

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
void displayLoadingScreen(std::future<bool>& future);

int main()
{
    std::ofstream outputFile;
    NumberGenerateInfo info;

    /*srand(time(0));
    info.maximum = 5;
    info.minimum = -10;
    info.precision = 2;
    unsigned long int range = abs(info.maximum - info.minimum);
    unsigned long int precisionMultiplier = (long int)(pow(10, info.precision));
    unsigned long long int trueRange = range * precisionMultiplier;
    int numberOfTimesToUseRand = round(((double)trueRange / RAND_MAX) + 0.5);
    int randomNumber = rand();
    unsigned long long int trueRandom = 0;
    for (int i = 0; i < numberOfTimesToUseRand * 3; i++)
        trueRandom += rand();

    double randomlyGeneratedNumber = ((double)(rand() % (range * precisionMultiplier))
        / precisionMultiplier) + min(info.minimum, info.maximum);
    
    std::cout << std::setprecision(10);
    std::cout << "range: " << range << std::endl;
    std::cout << "precisionMultiplier: " << precisionMultiplier << std::endl;
    std::cout << "trueRange: " << trueRange << std::endl;
    std::cout << "numberOfTimesToUseRand: " << numberOfTimesToUseRand << std::endl;
    std::cout << "randomNumber: " << randomNumber << std::endl;
    std::cout << "trueRandom: " << trueRandom << std::endl;
    std::cout << "randomlyGeneratedNumber: " << randomlyGeneratedNumber << std::endl;*/

    int numbersToGenerate, numbersOnEachRow;

    outputFile.open("generatedNumbers.txt");
    srand(time(0));

    getNumbersInfo(info, numbersToGenerate, numbersOnEachRow);
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();
    std::thread th([&promise, info, numbersToGenerate, numbersOnEachRow, &outputFile]()
        {
            outputFile << std::setprecision(8);
            double generatedNumber;
            for (int i = 1; i <= numbersToGenerate; i++)
            {
                generatedNumber = getRandomNumber(info);
                outputFile << generatedNumber << " ";
                if ((i % numbersOnEachRow) == 0)
                    outputFile << std::endl;
            }
            promise.set_value(true);
        });

    displayLoadingScreen(future);
    th.join();
}

// gets a random number based on the info of that number
double getRandomNumber(NumberGenerateInfo info)
{
    long int range = abs(info.maximum - info.minimum);
    long int precisionMultiplier = (long int)(pow(10, info.precision));

    return ((double)(rand() % (range * precisionMultiplier))
        / precisionMultiplier) + min(info.minimum, info.maximum);
}

// gets the information fromt he user about the number they want to generate
void getNumbersInfo(NumberGenerateInfo& info, int& numbersToGenerate,
    int& numberOnEachRow)
{
    numbersToGenerate = 
        getInput("Amount of numbers you want to generate: ", false);
    do
    {
        numberOnEachRow =
            getInput("Numbers you want to have on each row: ", false);
        if (numberOnEachRow <= 0)
            std::cout << "The numbers on each row must be a positive integer"
                << std::endl;
    } while (numberOnEachRow <= 0);
    
    do {
        info.minimum = getInput("Minimum number: ", true);
        info.maximum = getInput("Maximum number: ", true);
        if (info.minimum == info.maximum)
        {
            std::cout
                << "Your minimum number and maximum number cannot be the same"
                << std::endl;
        }     
    } while (info.minimum == info.maximum);
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
        {
            if (i == 0 && userStringInput[i] == '-')
            {
                if(canBeNegative)
                    i++;
                else
                    std::cout << "Number cannot be negative" << std::endl;
            }
            else if (!std::isdigit(userStringInput[i++]))
            {
                std::cout << "The input has to be an integer" << std::endl;
                return true;
            }
        }
        
        userInput = std::stoi(userStringInput);
        if (!canBeNegative && (userInput < 0)) return true;
        return false;
        })());

    return userInput;
}

// a function that displays a loading screen depending if a thread is joinable
void displayLoadingScreen(std::future<bool>& future)
{
    using namespace std::chrono_literals;

    auto status = future.wait_for(0ms);

    while (!(status == std::future_status::ready))
    {
        std::cout << "Loading";
        status = future.wait_for(0ms);
        for (int i = 0; (i < 3) && !(status == std::future_status::ready); i++)
        {
            Sleep(200);
            std::cout << ".";
            status = future.wait_for(0ms);
                
        }
        system("CLS");
    }

    std::cout << "Numbers generated!" << std::endl
        << "You can find the generated numbers on the file"
        << "\"generatedNumbers.txt\"";
    std::cout << std::endl;

    system("pause");
}