#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Card {
  char card;
  int value;

  Card(char card) {
    this->card = card;
    if (std::isdigit(card)) {
      this->value = card - '0';
    } else if (card == 'T') {
      this->value = 10;
    } else if (card == 'J') {
      this->value = 1;
    } else if (card == 'Q') {
      this->value = 12;
    } else if (card == 'K') {
      this->value = 13;
    } else if (card == 'A') {
      this->value = 14;
    }
  }
  bool operator<(const Card &card) const { return this->value < card.value; }

  bool operator==(const Card &card) const { return this->card == card.card; }
};

struct Hand {
  std::vector<Card> cards;
  size_t bet;
  int counts[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  size_t score() {
    for (int i{0}; i <= 14; i++) {
      counts[i] = 0;
    }
    for (auto c : cards) {
      int currentCount = counts[c.value];
      counts[c.value] = currentCount + 1;
    }

    size_t score{0};
    int jokerBonus = counts[1];
    bool bonusUsed = false;

    int maxCount{0};
    int secondToMaxCount{0};
    for (int i{2}; i < 15; i++) {
      if (counts[i] > secondToMaxCount) {
        if (counts[i] > maxCount) {
          secondToMaxCount = maxCount;
          maxCount = counts[i];
        } else {
          secondToMaxCount = counts[i];
        }
      }
    }
    int maxCountWithBonus = maxCount + jokerBonus;
    if (maxCountWithBonus > 3) {
      return pow(10, maxCountWithBonus);
    } else if (maxCountWithBonus == 3) {
      if (jokerBonus == 2) {
        return pow(10, maxCountWithBonus);
      } else if (secondToMaxCount == 2) {
        return pow(10, maxCountWithBonus) + pow(10, secondToMaxCount);
      } else {
        return pow(10, maxCountWithBonus);
      }
    } else if (maxCountWithBonus == 2) {
        if (secondToMaxCount == 2) {
            return pow(10, 2)*2;
        } else {
            return pow(10, 2);
        }
    }
  return score;
}

  int compare(Hand other) {
  size_t thisScore = this->score();
  size_t otherScore = other.score();
  if (thisScore != otherScore) {
    return thisScore > otherScore ? 1 : -1;
  }

  for (int i{0}; i < cards.size(); i++) {
    auto thisCard = cards[i];
    auto otherCard = other.cards[i];

    if (!(thisCard == otherCard)) {
      return otherCard < thisCard ? 1 : -1;
    }
  }

  return 0;
}
}
;

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

int main() {

  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  std::vector<Hand> hands;
  int totalPoints = 0;

  if (inputFile.is_open()) {
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        std::string hand;
        std::vector<Card> cards;
        size_t bet{0};
        auto parsedInputLine = splitByDelimiter(line, ' ');
        hand = parsedInputLine[0];
        for (char c : hand) {
          cards.push_back(Card{c});
        }
        bet = std::atol(parsedInputLine[1].c_str());
        hands.push_back(Hand{cards, bet});
      }
    }

    for (int i{0}; i < hands.size() - 1; i++) {
      for (int j{0}; j < (hands.size() - i - 1); j++) {
        Hand currentHand = hands[j];
        Hand followingHand = hands[j + 1];
        if (currentHand.compare(followingHand) == 1) {
          hands[j + 1] = currentHand;
          hands[j] = followingHand;
        }
      }
    }

    std::cout << "" << std::endl;
    size_t result{0};
    for (int i{0}; i < hands.size(); i++) {
      Hand h = hands[i];
      std::cout << "Hand: ";
      for (auto c : h.cards) {
        std::cout << c.card;
      }
      result += (h.bet * (i + 1));
      std::cout << " Score: " << h.score() << std::endl;
    }
    std::cout << "Result " << result << std::endl;
  }
  return 0;
}
