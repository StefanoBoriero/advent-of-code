#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
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

struct LinkedList {
  std::string label;
  int focalLenght;
  LinkedList *next;

  LinkedList(std::string label, int focalLenght) {
    this->label = label;
    this->focalLenght = focalLenght;
    this->next = nullptr;
  }
};

LinkedList *remove(LinkedList *head, std::string label) {
  if (!head) {
    return head;
  }
  if (head->label == label) {
    return head->next;
  }
  LinkedList *currentNode = head;
  while (currentNode->next) {
    if (currentNode->next->label == label) {
      currentNode->next = currentNode->next->next;
      break;
    }
    currentNode = currentNode -> next;
  }
  return head;
}

LinkedList *update(LinkedList *head, std::string label, int focalLenght) {
  LinkedList *currentNode = head;
  LinkedList *previousNode = nullptr;
  bool found = false;
  if (!head) {
    return new LinkedList(label, focalLenght);
  }
  while (currentNode && !found) {
    if (currentNode->label == label) {
      found = true;
      currentNode->focalLenght = focalLenght;
    }
    previousNode = currentNode;
    currentNode = currentNode->next;
  }

  if (!found) {
    previousNode->next = new LinkedList(label, focalLenght);
  }
  return head;
}

size_t power(LinkedList *head) {
  size_t sum = 0;
  size_t index = 1;
  LinkedList *currentNode = head;
  while (currentNode) {
    sum += currentNode->focalLenght * index;
    index++;
    currentNode = currentNode->next;
  }
  return sum;
}

int hash(std::string line) {
  int currentValue = 0;
  for (int i = 0; i < line.length(); i++) {
    currentValue += line[i];
    currentValue = currentValue * 17;
    currentValue = currentValue % 256;
  }
  return currentValue;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  size_t result = 0;
  LinkedList *boxes[256];
  for (int i = 0; i < 256; i++) {
    boxes[i] = nullptr;
  }

  if (inputFile.is_open()) {
    while (inputFile) {
      std::getline(inputFile, line);
      std::vector<std::string> tokens = splitByDelimiter(line, ',');
      for (int i = 0; i < tokens.size(); i++) {
        if (i % 500 == 0) {
          std::cout << "Processed 500 token" << std::endl;
        }
        std::string token = tokens[i];
        if (token.find('=') != std::string::npos) {
          std::vector<std::string> parsedToken = splitByDelimiter(token, '=');
          std::string label = parsedToken[0];
          int focalLenght = std::atoi(parsedToken[1].c_str());

          int h = hash(label);
          LinkedList *head = boxes[h];
          boxes[h] = update(head, label, focalLenght);

        } else {
          std::vector<std::string> parsedToken = splitByDelimiter(token, '-');
          std::string label = parsedToken[0];
          int h = hash(label);
          LinkedList *head = boxes[h];
          boxes[h] = remove(head, label);
        }
      }
    }

    for (int i = 0; i < 256; i++) {
      LinkedList *head = boxes[i];
      result += power(head) * (i + 1);
    }

    std::cout << result << std::endl;
  }
  return 0;
}
