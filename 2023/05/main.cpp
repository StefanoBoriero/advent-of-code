#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

struct Map {
  size_t destinationStart{0};
  size_t sourceStart{0};
  size_t range{0};

  bool contains(size_t source) {
    return source >= sourceStart && (source - sourceStart) < range;
  }

  size_t getDestination(size_t source) {
    size_t offset = source - sourceStart;
    std::cout << "Offset: " << offset << std::endl;
    return destinationStart + offset;
  }
};

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

std::vector<size_t> parseStringOfNumbers(std::string listOfNumbers) {
  std::vector<size_t> numbers;
  for (auto stringNumber : splitByDelimiter(listOfNumbers, ' ')) {
    numbers.push_back(std::atoi(stringNumber.c_str()));
  }
  return numbers;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  std::vector<size_t> elements;

  if (inputFile.is_open()) {
    std::string seedsLine;
    std::getline(inputFile, seedsLine);
    elements = parseStringOfNumbers(splitByDelimiter(seedsLine, ':')[1]);
    for (auto element : elements) {
      std::cout << element << std::endl;
    }
    std::getline(inputFile, line);
    while (inputFile) {
      // Get map type
      std::getline(inputFile, line);
      std::vector<Map> mappings;
      while (!line.empty()) {
        std::getline(inputFile, line);
        if (!line.empty()) {
          std::vector<size_t> numbers = parseStringOfNumbers(line);
          Map m{numbers[0], numbers[1], numbers[2]};
          mappings.push_back(m);
        }
      }
      for (int i{0}; i < elements.size(); i++) {
        size_t element = elements[i];
        for (auto m : mappings) {
          if (m.contains(element)) {
            elements[i] = m.getDestination(element);
          }
        }
        std::cout << element << std::endl;
      }
      std::cout << "" << std::endl;
    }

    size_t result = elements[0];
    for (auto element : elements) {
      std::cout << element << std::endl;
      if (element < result) {
        result = element;
      }
    }
    std::cout << "" << std::endl;
    std::cout << result << std::endl;
  }
  return 0;
}
