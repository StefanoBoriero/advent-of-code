#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
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

std::vector<long> parseStringOfNumbers(std::string listOfNumbers) {
  std::vector<long> numbers;
  for (auto stringNumber : splitByDelimiter(listOfNumbers, ',')) {
    numbers.push_back(std::atol(stringNumber.c_str()));
  }
  return numbers;
}

bool test(std::string line, std::vector<long> constraint) {
  size_t groupNumber{0};
  size_t groupSize{1};
  for (int i{1}; i < line.size(); i++) {
    char previousChar = line[i - 1];
    char currentChar = line[i];
    if (currentChar == '?') {
      if (previousChar == '#') {
        // std::cout << "Completed # group number " << groupNumber << " of size"
        // << groupSize << std::endl;
        return groupSize <= constraint[groupNumber];
      }
      return true;
    }
    if (currentChar != previousChar) {
      if (previousChar == '#') {
        //  std::cout << "Completed # group number " << groupNumber << " of size
        //  "
        //            << groupSize << std::endl;
        if (groupNumber >= constraint.size()) {
          return false;
        }
        if (constraint[groupNumber] != groupSize) {
          return false;
        }
        groupNumber++;
      }
      groupSize = 1;
    } else {
      groupSize++;
    }
  }

  return line[line.size() - 1] == '#' ? groupNumber + 1 == constraint.size() &&
                                            groupSize == constraint[groupNumber]
                                      : groupNumber == constraint.size();
}

size_t calculateNumberOfCombinations(std::string line,
                                     std::vector<long> faultyArrangments) {
  std::queue<std::string> possibleCombinations;
  possibleCombinations.push(line);
  for (int i{0}; i < line.size(); i++) {
    if (line[i] == '?') {
      size_t currentPossibleCombinations = possibleCombinations.size();
      for (size_t j{0}; j < currentPossibleCombinations; j++) {
        auto possibleCombination = possibleCombinations.front();
        possibleCombinations.pop();

        possibleCombination[i] = '.';
        // std::cout << "Testing line " << possibleCombination << std::endl;
        if (test(possibleCombination, faultyArrangments)) {
          // std::cout << "is valid" << std::endl;
          possibleCombinations.push(possibleCombination);
        }

        possibleCombination[i] = '#';
        // std::cout << "Testing line " << possibleCombination << std::endl;
        if (test(possibleCombination, faultyArrangments)) {
          // std::cout << "is valid" << std::endl;
          possibleCombinations.push(possibleCombination);
        }
      }
    }
  }

  return possibleCombinations.size();
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  size_t result{0};

  if (inputFile.is_open()) {
    long i{0};
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        auto tmp = splitByDelimiter(line, ' ');
        auto constraint = parseStringOfNumbers(tmp[1]);
        std::vector<long> constraints = constraint;
        std::string unfolded = tmp[0];
        for (int i{0}; i < 0; i++) {
          for (auto x : constraint) {
            constraints.push_back(x);
          }
          unfolded = unfolded + '?';
          unfolded = unfolded + tmp[0];
        }
        size_t originalNumberOfPossibleConfigs =
            calculateNumberOfCombinations(tmp[0], constraint);
        size_t numberOfPossibleConfigs =
            calculateNumberOfCombinations(unfolded, constraints);
        double rateOfGrowth =
            numberOfPossibleConfigs / (double)originalNumberOfPossibleConfigs;
        result += originalNumberOfPossibleConfigs * std::pow(rateOfGrowth, 4); // this actually works only for the test input
        if (numberOfPossibleConfigs % originalNumberOfPossibleConfigs != 0) {
          std::cout << "Growing from " << originalNumberOfPossibleConfigs
                    << " to " << numberOfPossibleConfigs << " at a rate of "
                    << rateOfGrowth << std::endl;
          std::cout << i << std::endl;
        }
        i++;
      }
    }

    std::cout << std::endl;
    std::cout << result << std::endl;
  }
  return 0;
}
