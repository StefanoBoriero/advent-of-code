#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> findPossibleMirrorRows(std::vector<std::string> rows) {
  std::vector<int> possibleMirrorRows;
  for (int i = 1; i < rows.size(); i++) {
    if (rows[i - 1] == rows[i]) {
      possibleMirrorRows.push_back(i);
    }
  }
  return possibleMirrorRows;
}

bool isMirrorRow(std::vector<std::string> rows, int row) {
  for (int i = row, j = row - 1; i < rows.size() && j >= 0; i++, j--) {
    if (rows[i] != rows[j]) {
      return false;
    }
  }
  return true;
}

std::vector<int> findPossibleMirrorColumns(std::vector<std::string> rows) {
  std::vector<int> possibleMirrorColumns;
  for (int j = 1; j < rows[0].size(); j++) {
    bool areEquals = true;
    for (int i = 0; i < rows.size(); i++) {
      if (rows[i][j - 1] != rows[i][j]) {
        areEquals = false;
      }
    }
    if (areEquals) {
      possibleMirrorColumns.push_back(j);
    }
  }
  return possibleMirrorColumns;
}

bool isMirrorColumn(std::vector<std::string> rows, int col) {
  for (int k = col, j = col - 1; k < rows[0].size() && j >= 0; k++, j--) {
    for (int i = 0; i < rows.size(); i++) {
      if (rows[i][k] != rows[i][j]) {
        return false;
      }
    }
  }
  return true;
}

int main() {
  std::ifstream inputFile;
  inputFile.open("input.txt");
  std::string line;

  if (inputFile.is_open()) {
    size_t result = 0;
    std::vector<std::string> rows;
    while (inputFile) {
      std::getline(inputFile, line);
      if (!line.empty()) {
        rows.push_back(line);
      } else {
        std::vector<int> possibleMirrorsRows = findPossibleMirrorRows(rows);
        std::vector<int> possibleMirrorsColumsn = findPossibleMirrorColumns(rows);
        for (int r : possibleMirrorsRows) {
          if (isMirrorRow(rows, r)) {
            result += r*100;
          }
        }
        for (int c : possibleMirrorsColumsn) {
          if (isMirrorColumn(rows, c)) {
            result += c;
          }
        }
        rows.clear();
      }
    }

    std::cout << std::endl << result << std::endl;
  }
  return 0;
}
