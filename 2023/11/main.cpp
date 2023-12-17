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

struct Coordinates {
  long x{0};
  long y{0};
};

size_t calculate(std::vector<Coordinates> galaxies,
                 std::vector<long> emptyLines, std::vector<long> emptyColumns) {
  size_t expansionFactor{1000000};
  size_t result{0};
  for (long i{0}; i < galaxies.size() - 1; i++) {
    for (long j{i + 1}; j < galaxies.size(); j++) {
      Coordinates start = galaxies[i];
      Coordinates finish = galaxies[j];
      size_t standardDistance =
          std::abs(start.x - finish.x) + std::abs(start.y - finish.y);

      size_t numberOfEmptyLinesCrossed{0};
      for (auto x : emptyLines) {
        size_t distanceFromStart = std::abs(start.x - x);
        size_t distanceFromFinish = std::abs(finish.x - x);
        if (distanceFromStart + distanceFromFinish ==
            std::abs(start.x - finish.x)) {
          numberOfEmptyLinesCrossed++;
        }
      }

      size_t numberOfEmptyColumnsCrossed{0};
      for (auto y : emptyColumns) {
        size_t distanceFromStart = std::abs(start.y - y);
        size_t distanceFromFinish = std::abs(finish.y - y);
        if (distanceFromStart + distanceFromFinish ==
            std::abs(start.y - finish.y)) {
          numberOfEmptyColumnsCrossed++;
        }
      }

      result += standardDistance +
                numberOfEmptyColumnsCrossed * (expansionFactor - 1) +
                numberOfEmptyLinesCrossed * (expansionFactor - 1);
    }
  }

  return result;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  size_t originalMatrixLength{140};
  char originalMatrix[originalMatrixLength][originalMatrixLength];
  std::vector<long> indexesOfEmptyLines;
  std::vector<long> indexesOfEmptyColumns;
  std::vector<Coordinates> galaxies;

  if (inputFile.is_open()) {
    long i{0};
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        bool hasNoGalaxy = true;
        for (int j{0}; j < line.size(); j++) {
          originalMatrix[i][j] = line[j];
          if (line[j] == '#') {
            hasNoGalaxy = false;
            galaxies.push_back({i, j});
          }
        }
        if (hasNoGalaxy) {
          indexesOfEmptyLines.push_back(i);
        }
      }
      i++;
    }

    for (int j{0}; j < originalMatrixLength; j++) {
      bool hasNoGalaxy = true;
      for (int i{0}; i < originalMatrixLength; i++) {
        if (originalMatrix[i][j] == '#') {
          hasNoGalaxy = false;
        }
      }
      if (hasNoGalaxy) {
        indexesOfEmptyColumns.push_back(j);
      }
    }

    std::cout << calculate(galaxies, indexesOfEmptyLines, indexesOfEmptyColumns)
              << std::endl;
  }
  return 0;
}
