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

std::vector<size_t> parseStringOfNumbers(std::string listOfNumbers) {
  std::vector<size_t> numbers;
  for (auto stringNumber : splitByDelimiter(listOfNumbers, ' ')) {
    numbers.push_back(std::atol(stringNumber.c_str()));
  }
  return numbers;
}

size_t calculateTravelledDistance(size_t timePressed, size_t totalDuration) {
  size_t speed = timePressed;
  size_t timeLeftToTravel = totalDuration - timePressed;

  size_t distanceTravelled = speed * timeLeftToTravel;
  return distanceTravelled;
}

size_t findCutoffTime(size_t raceDuration, size_t distanceToBeat) {
  size_t cutoffTime{0};
  bool isRaceDurationEven = raceDuration % 2 == 0;
  cutoffTime = raceDuration / 2;

  std::cout << "Evaluating cutoff time of " << cutoffTime << std::endl;
  size_t travelledDistance =
      calculateTravelledDistance(cutoffTime, raceDuration);
  std::cout << "Would travel " << travelledDistance << " m by pressing "
            << cutoffTime << " seconds" << std::endl;
  bool a = travelledDistance > distanceToBeat;
  std::cout << travelledDistance << std::endl;
  std::cout << distanceToBeat << std::endl;
  std::cout << a << std::endl;
  while (calculateTravelledDistance(cutoffTime, raceDuration) >
         distanceToBeat) {
    std::cout << "Evaluating cutoff time of " << cutoffTime << std::endl;
    cutoffTime = cutoffTime / 2;
  }

  while (calculateTravelledDistance(cutoffTime, raceDuration) <=
         distanceToBeat) {
    cutoffTime++;
  }

  return cutoffTime;
}

size_t calculateMaxTime(size_t cutoffTime, size_t raceDuration) {
  return raceDuration - cutoffTime;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  std::vector<size_t> matches;
  std::vector<size_t> cardCopies;
  int totalPoints = 0;

  if (inputFile.is_open()) {
    std::vector<size_t> times;
    std::vector<size_t> distances;
    std::getline(inputFile, line);
    times = parseStringOfNumbers(splitByDelimiter(line, ':')[1]);
    std::getline(inputFile, line);
    distances = parseStringOfNumbers(splitByDelimiter(line, ':')[1]);

    size_t result{1};

    for (int i{0}; i < times.size(); i++) {
      size_t raceDuration = times[i];
      size_t distanceToBeat = distances[i];
      size_t winningCombinationsCount{0};

      size_t cutoffTime = findCutoffTime(raceDuration, distanceToBeat);
      std::cout << "Cutoff time: " << cutoffTime << std::endl;
      std::cout << "Max time: " << calculateMaxTime(cutoffTime, raceDuration)
                << std::endl;
      winningCombinationsCount =
          calculateMaxTime(cutoffTime, raceDuration) - cutoffTime + 1;
      std::cout << "Winning combinations: " << winningCombinationsCount
                << std::endl;
      result = result * winningCombinationsCount;
    }
    std::cout << "Result: " << result << std::endl;
  }
  return 0;
}
