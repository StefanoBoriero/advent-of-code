#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct State {
  std::string state;
  int numberOfTilts;

  State(std::string state, int numberOfTilts) {
    this->state = state;
    this->numberOfTilts = numberOfTilts;
  }

  bool operator==(const State &other) { return this->state == other.state; }
};

std::string toString(char *m, size_t inputSize) {
  std::string out = "";
  for (int i = 0; i < inputSize; i++) {
    for (int j = 0; j < inputSize; j++) {
      out += m[i + inputSize + j];
    }
  }
  return out;
}

size_t tiltNorth(char *input, int inputSize) {
  size_t numberOfChanges = 0;

  for (int i = 1; i < inputSize; i++) {
    for (int j = 0; j < inputSize; j++) {
      char current = input[i * inputSize + j];
      if (current == 'O') {
        size_t currentIndex = (i * inputSize + j);
        size_t aboveIndex = (i - 1) * inputSize + j;
        int k = j;
        while (input[aboveIndex] == '.' && aboveIndex >= 0) {
          input[aboveIndex] = 'O';
          input[currentIndex] = '.';
          aboveIndex -= inputSize;
          currentIndex -= inputSize;
          numberOfChanges++;
        }
      }
    }
  }

  return numberOfChanges;
}

size_t tiltSouth(char *input, int inputSize) {
  size_t numberOfChanges = 0;

  for (int i = inputSize - 2; i >= 0; i--) {
    for (int j = 0; j < inputSize; j++) {
      char current = input[i * inputSize + j];
      if (current == 'O') {
        int currentIndex = i * inputSize + j;
        int belowIndex = (i + 1) * inputSize + j;
        while (input[belowIndex] == '.' && belowIndex < inputSize * inputSize) {
          input[belowIndex] = 'O';
          input[currentIndex] = '.';
          belowIndex += inputSize;
          currentIndex += inputSize;
          numberOfChanges++;
        }
      }
    }
  }

  return numberOfChanges;
}

size_t tiltWest(char *input, int inputSize) {
  size_t numberOfChanges = 0;

  for (int j = 1; j < inputSize; j++) {
    for (int i = 0; i < inputSize; i++) {
      char current = input[(i * inputSize) + j];
      if (current == 'O') {
        int currentIndex = (i * inputSize) + j;
        int westIndex = currentIndex - 1;
        while (input[westIndex] == '.' &&
               westIndex % inputSize < inputSize - 1) {
          input[westIndex] = 'O';
          input[currentIndex] = '.';
          currentIndex--;
          westIndex--;
          numberOfChanges++;
        }
      }
    }
  }

  return numberOfChanges;
}

size_t tiltEast(char *input, int inputSize) {
  size_t numberOfChanges = 0;

  for (int j = inputSize - 2; j >= 0; j--) {
    for (int i = 0; i < inputSize; i++) {
      char current = input[i * inputSize + j];
      if (current == 'O') {
        int currentIndex = i * inputSize + j;
        int eastIndex = i * inputSize + (j + 1);
        while (input[eastIndex] == '.' && eastIndex % inputSize > 0) {
          input[eastIndex] = 'O';
          input[currentIndex] = '.';
          eastIndex += 1;
          currentIndex += 1;
          numberOfChanges++;
        }
      }
    }
  }

  return numberOfChanges;
}

void print(char *matrix, int inputSize) {
  for (int i = 0; i < inputSize; i++) {
    for (int j = 0; j < inputSize; j++) {
      std::cout << matrix[i * inputSize + j];
    }
    std::cout << std::endl;
  }
}

int main() {
  std::ifstream inputFile;
  inputFile.open("test_input.txt");
  std::string line;
  int inputSize = 10;
  char matrix[inputSize][inputSize];

  if (inputFile.is_open()) {
    size_t result = 0;
    std::vector<std::string> rows;
    int i = 0;
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        for (int j = 0; j < line.size(); j++) {
          matrix[i][j] = line[j];
        }
        i++;
      }
    }

    bool stop = false;
    int j = 0;
    size_t maxIterations = 1000000000;
    std::vector<State> statesAfterTilts;
    State lastState = State("", 0);
    while (!stop) {
      j++;
      tiltNorth(matrix[0], inputSize);
      std::cout << "N" << std::endl;
      print(matrix[0], inputSize);

      tiltWest(matrix[0], inputSize);
      std::cout << "W" << std::endl;
      print(matrix[0], inputSize);

      tiltSouth(matrix[0], inputSize);
      std::cout << "S" << std::endl;
      print(matrix[0], inputSize);

      tiltEast(matrix[0], inputSize);
      std::cout << "S" << std::endl;
      print(matrix[0], inputSize);

      std::cout << "Finished cycle " << j << std::endl;

      State s = State(toString(matrix[0], inputSize), j);
      std::__wrap_iter<State *> found =
          std::find(statesAfterTilts.begin(), statesAfterTilts.end(), s);
      if (found != statesAfterTilts.end()) {
        std::cout << "Found it after " << j << " iterations" << std::endl;
        maxIterations =
            (maxIterations - found->numberOfTilts) % (j - found->numberOfTilts);
      }
      statesAfterTilts.push_back(s);
      // stop = numberOfChanges == 0;
      stop = lastState == s;
      lastState = s;
      if (j % 100000 == 0) {
        std::cout << j << std::endl;
      }
    }

    print(matrix[0], inputSize);

    for (int i = 0; i < inputSize; i++) {
      for (int j = 0; j < inputSize; j++) {
        if (matrix[i][j] == 'O') {
          result += (inputSize - i);
        }
      }
    }

    std::cout << result << std::endl;
  }
  return 0;
}
