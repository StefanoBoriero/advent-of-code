
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

struct Draw {
  int reds{0};
  int greens{0};
  int blues{0};

  Draw() {}

  Draw(int reds, int greens, int blues) {
    this->reds = reds;
    this->greens = greens;
    this->blues = blues;
  }

  void setColor(std::string color, int amount) {
    if (color == "red") {
      this->reds = amount;
    } else if (color == "blue") {
      this->blues = amount;
    } else if (color == "green") {
      this->greens = amount;
    }
  }

  bool isValid(Draw constraints) {
    return constraints.reds >= this->reds &&
           constraints.greens >= this->greens &&
           constraints.blues >= this->blues;
  }
};

struct Game {
  int id;
  std::vector<Draw> draws;

  Game(int id) { this->id = id; }

  void addDraw(Draw draw) { this->draws.push_back(draw); }

  bool isValid(Draw constraints) {
    for (auto draw : this->draws) {
      if (!draw.isValid(constraints)) {
        return false;
      }
    }
    return true;
  }

  Draw getMinimumConstraint() {
    int maxReds{0};
    int maxGreens{0};
    int maxBlues{0};
    for (auto draw : this->draws) {
        if (draw.reds > maxReds) {
            maxReds = draw.reds;
        }
        if (draw.greens > maxGreens) {
            maxGreens = draw.greens;
        }
        if (draw.blues > maxBlues) {
            maxBlues = draw.blues;
        }
    }

    return Draw {maxReds, maxGreens, maxBlues};
  }

  int calculatePower() {
      Draw minimumSetOfCubes = this->getMinimumConstraint();
      return minimumSetOfCubes.reds * minimumSetOfCubes.greens * minimumSetOfCubes.blues;
  }
};

std::vector<std::string> splitByDelimiter(std::string game, char delimiter) {
  std::vector<std::string> draws;
  int currentDelimiterPosition = 0;
  int nextDelimiterPosition = 0;

  while ((nextDelimiterPosition = game.find(
              delimiter, currentDelimiterPosition)) != std::string::npos) {
    std::string draw =
        game.substr(currentDelimiterPosition,
                    nextDelimiterPosition - currentDelimiterPosition);
    draws.push_back(draw);
    currentDelimiterPosition = nextDelimiterPosition + 1;
  }
  draws.push_back(game.substr(currentDelimiterPosition + 1));
  return draws;
}

Draw parseDraw(std::string drawString) {
  auto tokens = splitByDelimiter(drawString, ',');
  Draw draw = {};
  for (auto t : tokens) {
    int amount = 0;
    char color[5];
    sscanf(t.c_str(), "%d %s", &amount, color);
    draw.setColor(color, amount);
  }

  return draw;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;
  std::vector<Game> games;

  if (inputFile.is_open()) {
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        int currentDelimiterPosition = line.find(":");
        int nextDelimiterPosition = 0;
        std::string gameString = line.substr(0, currentDelimiterPosition);
        int gameId{0};
        sscanf(gameString.c_str(), "Game %d:", &gameId);
        Game game = {gameId};

        auto draws = splitByDelimiter(line.substr(line.find(":") + 1), ';');
        for (std::string drawString : draws) {
          Draw draw = parseDraw(drawString);
          game.addDraw(draw);
        }
        games.push_back(game);
      }
    }

    Draw constraints = {12, 13, 14};
    size_t result{0};
    for (auto game : games) {
        size_t power = game.calculatePower();
        result += power;
    }
    std::cout << result << std::endl;
  }
}
