#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

struct Range {
  size_t begin;
  size_t end;

  std::vector<Range> intersect(Range other) {
    std::vector<Range> intersections;

    if (other.end < this->begin) {
    } else if (other.begin >= this->begin && other.end <= this->end) {
      intersections.push_back(Range{other.begin, other.end});
    } else if (other.begin >= this->begin && other.end >= this->end) {
      intersections.push_back(Range{other.begin, this->end});
    } else if (other.begin <= this->begin && other.end <= this->end) {
      intersections.push_back(Range{this->begin, other.end});
    } else {
      intersections.push_back(Range{this->begin, this->end});
    }

    return intersections;
  }

  std::vector<Range> difference(Range other) {
    std::vector<Range> difference;

    if (other.end < this->begin) {
      difference.push_back(Range{other.begin, other.end});
    } else if (other.begin >= this->begin && other.end <= this->end) {
    } else if (other.begin >= this->begin && other.end >= this->end) {
      difference.push_back(Range{this->end + 1, other.end});
    } else if (other.begin <= this->begin && other.end <= this->end) {
      difference.push_back(Range{other.begin, this->end - 1});
    } else if (other.begin <= this->begin && other.end >= this->end) {
      difference.push_back(Range{other.begin, this->begin - 1});
      difference.push_back(Range{this->end + 1, other.end});
    }
    return difference;
  }

  bool contains(Range other) {
    return !(other.end < begin) && !(other.begin > end);
  }

  bool equals(Range other) { return other.begin == begin && other.end == end; }
};

struct Map {
  size_t destinationStart{0};
  size_t sourceStart{0};
  size_t range{0};

  Range getSourceRange() { return Range{sourceStart, sourceStart + range}; }

  bool contains(size_t source) {
    return source >= sourceStart && (source - sourceStart) < range;
  }

  bool containsFully(Range r) {
    return r.begin >= sourceStart && r.end <= sourceStart + range;
  }

  Range getTransformedRange(Range r) {
    size_t offset = r.begin - sourceStart;
    size_t rangeLength = r.end - r.begin;
    Range resultingRange{destinationStart + offset,
                         destinationStart + offset + rangeLength};
    std::cout << "Transforming range " << r.begin << ":" << r.end << " into "
              << resultingRange.begin << ":" << resultingRange.end << std::endl;
    return resultingRange;
  }

  size_t getDestination(size_t source) {
    size_t offset = source - sourceStart;
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

std::queue<Range> generateSeeds(std::vector<size_t> seedsInfo) {
  std::queue<Range> seeds;
  for (int i{0}; i < seedsInfo.size(); i += 2) {
    std::cout << "Pushing range " << seedsInfo[i] << ":"
              << seedsInfo[i] + seedsInfo[i + 1] - 1 << std::endl;
    seeds.push(Range{seedsInfo[i], seedsInfo[i] + seedsInfo[i + 1] - 1});
  }
  return seeds;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("test_input.txt");
  std::string line;
  std::vector<size_t> elements;

  if (inputFile.is_open()) {
    std::string seedsLine;
    std::getline(inputFile, seedsLine);
    elements = parseStringOfNumbers(splitByDelimiter(seedsLine, ':')[1]);
    // Should be a queue because difference should be stacked back in
    std::queue<Range> sourceSeedRanges = generateSeeds(elements);
    std::queue<Range> destinationSeedRanges;

    std::getline(inputFile, line);
    while (inputFile) {
      // Get map type
      std::getline(inputFile, line);
      std::cout << line << std::endl;
      std::vector<Map> mappings;
      while (!line.empty()) {
        std::getline(inputFile, line);
        if (!line.empty()) {
          std::vector<size_t> numbers = parseStringOfNumbers(line);
          Map m{numbers[0], numbers[1], numbers[2]};
          mappings.push_back(m);
        }
      }
      while (!sourceSeedRanges.empty()) {
        std::cout << "Source size: " << sourceSeedRanges.size() << std::endl;
        Range sourceRange = sourceSeedRanges.front();
        std::cout << "Analysing source range " << sourceRange.begin << ":"
                  << sourceRange.end << std::endl;
        bool found = false;
        for (auto m : mappings) {
          auto r = m.getSourceRange();
          if (r.contains(sourceRange)) {
            found = true;
            for (auto ra : r.intersect(sourceRange)) {
              destinationSeedRanges.push(m.getTransformedRange(ra));
            }
            for (auto ra : r.difference(sourceRange)) {
              // if (!sourceRange.equals(ra)) {
              std::cout << "Pushing range " << ra.begin << ":" << ra.end
                        << std::endl;
              sourceSeedRanges.push(ra);
              //}
            }
          }
        }
        if (!found) {
          std::cout << "No match found" << std::endl;
          destinationSeedRanges.push(sourceRange);
        }
        std::cout << "Popping analysed range.." << std::endl;
        std::cout << "Source size: " << sourceSeedRanges.size() << std::endl;
        sourceSeedRanges.pop();
      }
      while (!destinationSeedRanges.empty()) {
        sourceSeedRanges.push(destinationSeedRanges.front());
        destinationSeedRanges.pop();
      }
      std::cout << "Mapping done.." << std::endl;
    }

    size_t result{sourceSeedRanges.front().begin};
    while (!sourceSeedRanges.empty()) {
      size_t candidate = sourceSeedRanges.front().begin;
      if (candidate < result) {
        result = candidate;
      }
      sourceSeedRanges.pop();
    }

    std::cout << "" << std::endl;
    std::cout << result << std::endl;
  }
  return 0;
}
