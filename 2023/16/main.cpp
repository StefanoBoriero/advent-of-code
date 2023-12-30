#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>

struct Position {
  int x{0};
  int y{0};

  bool operator==(const Position &other) {
    return other.x == x && other.y == y;
  }
};

struct Beam {
  Position currentPosition{0, 0};
  Position previousPosition{0, 0};

  bool operator==(const Beam &other) {
    return currentPosition == other.currentPosition &&
           previousPosition == other.previousPosition;
  }
};

std::vector<Position> nextPosition(Position previousPosition,
                                   Position currentPosition, char item) {
  std::vector<Position> nextPositions;
  if (previousPosition.x < currentPosition.x &&
      previousPosition.y == currentPosition.y) {
    // Moving rightway
    if (item == '.' || item == '-') {
      nextPositions.push_back({currentPosition.x + 1, currentPosition.y});
    } else if (item == '\\') {
      nextPositions.push_back({currentPosition.x, currentPosition.y + 1});
    } else if (item == '/') {
      nextPositions.push_back({currentPosition.x, currentPosition.y - 1});
    } else if (item == '|') {
      nextPositions.push_back({currentPosition.x, currentPosition.y + 1});
      nextPositions.push_back({currentPosition.x, currentPosition.y - 1});
    }
    return nextPositions;
  }
  if (previousPosition.x > currentPosition.x &&
      previousPosition.y == currentPosition.y) {
    // Moving leftway
    if (item == '.' || item == '-') {
      nextPositions.push_back({currentPosition.x - 1, currentPosition.y});
    } else if (item == '\\') {
      nextPositions.push_back({currentPosition.x, currentPosition.y - 1});
    } else if (item == '/') {
      nextPositions.push_back({currentPosition.x, currentPosition.y + 1});
    } else if (item == '|') {
      nextPositions.push_back({currentPosition.x, currentPosition.y + 1});
      nextPositions.push_back({currentPosition.x, currentPosition.y - 1});
    }
    return nextPositions;
  }
  if (previousPosition.x == currentPosition.x &&
      previousPosition.y < currentPosition.y) {
    // Moving downward
    if (item == '.' || item == '|') {
      nextPositions.push_back({currentPosition.x, currentPosition.y + 1});
    } else if (item == '\\') {
      nextPositions.push_back({currentPosition.x + 1, currentPosition.y});
    } else if (item == '/') {
      nextPositions.push_back({currentPosition.x - 1, currentPosition.y});
    } else if (item == '-') {
      nextPositions.push_back({currentPosition.x + 1, currentPosition.y});
      nextPositions.push_back({currentPosition.x - 1, currentPosition.y});
    }
    return nextPositions;
  }
  if (previousPosition.x == currentPosition.x &&
      previousPosition.y > currentPosition.y) {
    // Moving upward
    if (item == '.' || item == '|') {
      nextPositions.push_back({currentPosition.x, currentPosition.y - 1});
    } else if (item == '\\') {
      nextPositions.push_back({currentPosition.x - 1, currentPosition.y});
    } else if (item == '/') {
      nextPositions.push_back({currentPosition.x + 1, currentPosition.y});
    } else if (item == '-') {
      nextPositions.push_back({currentPosition.x + 1, currentPosition.y});
      nextPositions.push_back({currentPosition.x - 1, currentPosition.y});
    }
    return nextPositions;
  }
  return nextPositions;
}

bool isValid(Position p, int inputSize) {
  return p.x >= 0 && p.y >= 0 && p.x < inputSize && p.y < inputSize;
}

void print(char *matrix, int inputSize) {
  for (int i{0}; i < inputSize; i++) {
    for (int j{0}; j < inputSize; j++) {
      std::cout << matrix[i * inputSize + j];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  int inputSize{110};
  char map[inputSize][inputSize];
  char energisedMap[inputSize][inputSize];

  int i{0};
  if (inputFile.is_open()) {
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        for (int j{0}; j < line.length(); j++) {
          map[i][j] = line[j];
        }
        i++;
      }
    }

    std::vector<Beam> startingPositions;
    for (int i{0}; i < inputSize; i++) {
      startingPositions.push_back({{0, i}, {-1, i}});
      startingPositions.push_back({{inputSize - 1, i}, inputSize, i});

      startingPositions.push_back({{i, 0}, {i, -1}});
      startingPositions.push_back({{i, inputSize - 1}, {i, inputSize}});
    }

    size_t maxResult{0};
    for (int k{0}; k < startingPositions.size(); k++) {
      Beam beam = startingPositions[k];
      size_t result{0};
      for (int i{0}; i < inputSize; i++) {
        for (int j{0}; j < inputSize; j++) {
          energisedMap[i][j] = '.';
        }
      }
      std::queue<Beam> currentBeams;
      std::vector<Beam> exploredBeams;
      exploredBeams.push_back(beam);
      currentBeams.push(beam);
      energisedMap[beam.currentPosition.y][beam.currentPosition.x] = '#';
      while (!currentBeams.empty()) {
        Beam b = currentBeams.front();
        currentBeams.pop();
        std::vector<Position> nextPositions =
            nextPosition(b.previousPosition, b.currentPosition,
                         map[b.currentPosition.y][b.currentPosition.x]);
        for (auto p : nextPositions) {
          if (isValid(p, inputSize)) {
            Beam newBeam{p, b.currentPosition};
            if (std::find(exploredBeams.begin(), exploredBeams.end(),
                          newBeam) == exploredBeams.end()) {
              // Add it back to the queue only if we didn't explore it already
              exploredBeams.push_back(newBeam);
              currentBeams.push(newBeam);
              energisedMap[p.y][p.x] = '#';
            }
          }
        }
      }
      for (int i{0}; i < inputSize; i++) {
        for (int j{0}; j < inputSize; j++) {
          if (energisedMap[i][j] == '#') {
            result++;
          }
        }
      }
      if (result > maxResult) {
        maxResult = result;
      }
    }

    std::cout << "Done" << std::endl;
    std::cout << maxResult << std::endl;
  }
  return 0;
}
