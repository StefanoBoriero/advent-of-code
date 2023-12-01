#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

int stringLiteralToIn(std::string stringLiteral) {
  if (stringLiteral == "zero")
    return 0;
  if (stringLiteral == "one")
    return 1;
  if (stringLiteral == "two")
    return 2;
  if (stringLiteral == "three")
    return 3;
  if (stringLiteral == "four")
    return 4;
  if (stringLiteral == "five")
    return 5;
  if (stringLiteral == "six")
    return 6;
  if (stringLiteral == "seven")
    return 7;
  if (stringLiteral == "eight")
    return 8;
  if (stringLiteral == "nine")
    return 9;

  return -1;
}

bool spellsADigit(std::string myString) {
  return stringLiteralToIn(myString) >= 0;
}

size_t findFirstDigitInString(std::string s) {
  for (size_t i = 0; i < s.length(); i++) {
    if (std::isdigit(s[i])) {
      return s[i] - '0';
    }

    // This loop is for part 2
    for (int l = 3; l <= 5; l++) {
      std::string substring = s.substr(i, l);
      if (spellsADigit(substring)) {
        return stringLiteralToIn(substring);
      }
    }
  }
  return 0;
}

size_t findLastDigitInString(std::string s) {
  for (size_t i = s.length() - 1; i >= 0; i--) {
    if (std::isdigit(s[i])) {
      return s[i] - '0';
    }

    // This loop is for part 2
    for (int l = 3; l <= 5; l++) {
      if (i > l - 1) {
        std::string substring = s.substr(i - l + 1, l);
        if (spellsADigit(substring)) {
          return stringLiteralToIn(substring);
        }
      }
    }
  }
  return 0;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  std::vector<size_t> numbers;

  if (inputFile.is_open()) {
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        size_t firstDigit = findFirstDigitInString(line);
        size_t secondDigit = findLastDigitInString(line);
        size_t number = firstDigit * 10 + secondDigit;
        numbers.push_back(number);
      }
    }
    size_t sum = 0;
    for (size_t number : numbers) {
      sum += number;
      std::cout << number << std::endl;
    }
    std::cout << sum << std::endl;
  }

  return 0;
}
