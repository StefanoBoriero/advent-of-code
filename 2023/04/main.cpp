#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> splitByDelimiter(std::string stringToSplit,
                                          char delimiter) {
  std::vector<std::string> tokens;
  int currentDelimiterPosition = 0;
  int nextDelimiterPosition = 0;


  while ((nextDelimiterPosition = stringToSplit.find(
              delimiter, currentDelimiterPosition)) != std::string::npos) {
    if (stringToSplit[currentDelimiterPosition] == delimiter) {
        currentDelimiterPosition += 1;
        continue;
    }
    std::string token =
        stringToSplit.substr(currentDelimiterPosition,
                             nextDelimiterPosition - currentDelimiterPosition);
    tokens.push_back(token);
    currentDelimiterPosition = nextDelimiterPosition + 1;
  }
  if (currentDelimiterPosition != stringToSplit.length()) {
    tokens.push_back(stringToSplit.substr(currentDelimiterPosition));
  }
  return tokens;
}

std::vector<int> parseStringOfNumbers(std::string listOfNumbers) {
  std::vector<int> numbers;
  for (auto stringNumber : splitByDelimiter(listOfNumbers, ' ')) {
    numbers.push_back(std::atoi(stringNumber.c_str()));
  }
  return numbers;
}

int getNumberOfMatches(std::vector<int> playedNumbers,
                       std::vector<int> winningNumbers) {
  int numberOfMatches = 0;
  for (int n : playedNumbers) {
    if (std::find(winningNumbers.begin(), winningNumbers.end(), n) !=
        winningNumbers.end()) {
      std::cout << "Found a match for " << n << std::endl;
      numberOfMatches++;
    }
  }
  return numberOfMatches;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  std::vector<size_t> numbers;
  int totalPoints = 0;

  if (inputFile.is_open()) {
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        auto splitTokens = splitByDelimiter(line, ':');
        std::string cardInfo = splitTokens.at(0);
        std::cout << cardInfo << std::endl;
        auto stringListOfNumbers = splitByDelimiter(splitTokens.at(1), '|');
        std::vector<int> winningNumbers =
            parseStringOfNumbers(stringListOfNumbers.at(0));
        std::vector<int> playedNumbers =
            parseStringOfNumbers(stringListOfNumbers.at(1));

        int numberOfMatchesForCard =
            getNumberOfMatches(playedNumbers, winningNumbers);
        int cardPoints = 0;
        if (numberOfMatchesForCard > 0) {
          cardPoints = pow(2, numberOfMatchesForCard - 1);
        }
        std::cout << cardPoints << std::endl;
        totalPoints += cardPoints;
      }
    }
    std::cout << totalPoints << std::endl;
  }

  return 0;
}
