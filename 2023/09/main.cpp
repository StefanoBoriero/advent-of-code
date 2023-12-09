#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
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
  for (auto stringNumber : splitByDelimiter(listOfNumbers, ' ')) {
    numbers.push_back(std::atol(stringNumber.c_str()));
  }
  return numbers;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  long result{0};

  if (inputFile.is_open()) {
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        bool keepIterating = true;
        auto lineNumbers = parseStringOfNumbers(line);
        std::stack<long> lastNumbers;
        long lineResult{0};
        // std::reverse(lineNumbers.begin(), lineNumbers.end());
        while (keepIterating) {
          keepIterating = false;
          lastNumbers.push(lineNumbers[0]);
          std::vector<long> nextLine;
          for (int i{0}; i < lineNumbers.size() - 1; i++) {
            size_t diff = lineNumbers[i] - lineNumbers[i + 1];
            nextLine.push_back(-diff);
            if (diff != 0) {
              keepIterating = true;
            }
          }
          lineNumbers = nextLine;
        }
        while (!lastNumbers.empty()) {
          auto n = lastNumbers.top();
          lastNumbers.pop();
          lineResult = n - lineResult;
        }
        std::cout << lineResult << std::endl;
        result += lineResult;
      }
    }
    std::cout << result << std::endl;
    return 0;
  }
}
