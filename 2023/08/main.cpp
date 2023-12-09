#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
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

std::vector<size_t> parseStringOfNumbers(std::string listOfNumbers) {
  std::vector<size_t> numbers;
  for (auto stringNumber : splitByDelimiter(listOfNumbers, ' ')) {
    numbers.push_back(std::atol(stringNumber.c_str()));
  }
  return numbers;
}

struct GraphNode {
  std::string value;
  GraphNode *left;
  GraphNode *right;

  GraphNode(std::string value) {
    this->value = value;
    this->left = nullptr;
    this->right = nullptr;
  }

  void addLeftNode(GraphNode *left) { this->left = left; }
  void addRightNode(GraphNode *right) { this->right = right; }
};

struct NodeInfo {
  std::string value;
  std::string left;
  std::string right;
};

NodeInfo parseLine(std::string line) {
  std::string value = line.substr(0, 3);
  std::string left = line.substr(7, 3);
  std::string right = line.substr(12, 3);
  return {value, left, right};
};

GraphNode *getNode(std::map<std::string, GraphNode *> *nodes, std::string key) {
  auto it = nodes->find(key);
  if (it != nodes->end()) {
    return it->second;
  }
  GraphNode *newNode = new GraphNode{key};
  nodes->emplace(key, newNode);
  return newNode;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  std::string commands;
  std::map<std::string, GraphNode *> nodes;
  std::vector<std::string> startingNodes;
  std::vector<std::string> endingNodes;

  if (inputFile.is_open()) {
    std::getline(inputFile, commands);
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        auto info = parseLine(line);
        auto startingNode = getNode(&nodes, info.value);
        auto leftNode = getNode(&nodes, info.left);
        auto rightNode = getNode(&nodes, info.right);

        startingNode->left = leftNode;
        startingNode->right = rightNode;
        if (info.value[2] == 'A') {
          startingNodes.push_back(info.value);
        }
        if (info.value[2] == 'Z') {
          endingNodes.push_back(info.value);
        }
      }
    }

    std::vector<size_t> stepsToZ;
    for (auto startingNode : startingNodes) {
      GraphNode *currentNode = getNode(&nodes, startingNode);
      size_t i{0};
      while (std::find(endingNodes.begin(), endingNodes.end(),
                       currentNode->value) == endingNodes.end()) {
        char command = commands[i % commands.size()];
        // std::cout << currentNode->value << std::endl;
        if (command == 'L') {
          currentNode = currentNode->left;
        } else if (command == 'R') {
          currentNode = currentNode->right;
        }
        i++;
      }
      stepsToZ.push_back(i);
    }

    for (auto i : stepsToZ)
      std::cout << "Number of steps: " << i << std::endl;
  }
  return 0;
}
