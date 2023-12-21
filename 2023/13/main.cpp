#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct PossibleSolution {
  std::vector<std::string> rows;
  int mirrorIndex;
};

std::vector<int> findPossibleMirrorRows(std::vector<std::string> rows) {
  std::vector<int> possibleMirrorRows;
  for (int i = 1; i < rows.size(); i++) {
    if (rows[i - 1] == rows[i]) {
      possibleMirrorRows.push_back(i);
    }
  }
  return possibleMirrorRows;
}

std::vector<PossibleSolution>
findPossibleMirrorRows2(std::vector<std::string> rows) {
  std::vector<PossibleSolution> possibleRows;
  for (int i = 0; i < rows.size() - 1; i++) {
    std::string firstRow = rows[i];
    for (int j = i + 1; j < rows.size(); j += 2) {
      std::string secondRow = rows[j];
      int diffCount = 0;
      int charIndexToChange = -1;
      for (int k = 0; k < firstRow.size() && diffCount < 2; k++) {
        if (firstRow[k] != secondRow[k]) {
          diffCount++;
          charIndexToChange = k;
        }
      }
      if (diffCount == 1) {
        std::vector<std::string> modifiedRows(rows.size());
        std::copy(rows.begin(), rows.end(), modifiedRows.begin());
        std::string copiedFirstRow = modifiedRows[i];
        if (copiedFirstRow[charIndexToChange] == '#') {
          copiedFirstRow[charIndexToChange] = '.';
        } else {
          copiedFirstRow[charIndexToChange];
        }
        modifiedRows[i] = copiedFirstRow;
        PossibleSolution ps;
        ps.rows = modifiedRows;
        ps.mirrorIndex = i + ((j - i) / 2) + 1;
        possibleRows.push_back(ps);
      }
    }
  }
  return possibleRows;
}

bool isMirrorRow(std::vector<std::string> rows, int row) {
  for (int i = row, j = row - 1; i < rows.size() && j >= 0; i++, j--) {
    if (rows[i] != rows[j]) {
      return false;
    }
  }
  return true;
}

std::vector<PossibleSolution>
findPossibleMirrorColumns2(std::vector<std::string> rows) {
  std::vector<PossibleSolution> possibleColumns;

  for (int i = 0; i < rows[0].size() - 1; i++) {
    for (int j = i + 1; j < rows[0].size(); j += 2) {
      int diffCount = 0;
      int charIndexToChange = -1;
      for (int k = 0; k < rows.size() && diffCount < 2; k++) {
        if (rows[k][i] != rows[k][j]) {
          diffCount++;
          charIndexToChange = k;
        }
      }

      if (diffCount == 1) {
        std::vector<std::string> modifiedRows(rows.size());
        std::copy(rows.begin(), rows.end(), modifiedRows.begin());
        if (modifiedRows[charIndexToChange][i] == '.') {
          modifiedRows[charIndexToChange][i] = '#';
        } else {
          modifiedRows[charIndexToChange][i] = '.';
        }
        PossibleSolution ps;
        ps.rows = modifiedRows;
        ps.mirrorIndex = i + ((j - i) / 2) + 1;
        possibleColumns.push_back(ps);
      }
    }
  }

  return possibleColumns;
}

std::vector<std::string> transpose(std::vector<std::string> original) {
  std::vector<std::string> transposed;

  for (int j = 0; j < original[0].size(); j++) {
    std::string row = "";
    for (int i = 0; i < original.size(); i++) {
      row += original[i][j];
    }
    transposed.push_back(row);
  }

  return transposed;
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
  // std::cout << "Checking if column " << col << " is mirror for " <<
  // std::endl;
  for (std::string s : rows) {
    //  std::cout << s << std::endl;
  }
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
    int i = 0;
    while (inputFile) {
      i++;
      std::getline(inputFile, line);
      if (!line.empty()) {
        rows.push_back(line);
      } else {
        std::vector<PossibleSolution> possibleMirrorsSolutions =
            findPossibleMirrorColumns2(rows);
        for (PossibleSolution p : possibleMirrorsSolutions) {
          if (isMirrorColumn(p.rows, p.mirrorIndex)) {
            result += p.mirrorIndex;
          }
        }
        std::vector<std::string> transposed = transpose(rows);
        std::vector<PossibleSolution> possibleMirrorsColumns =
            findPossibleMirrorColumns2(transposed);
        for (PossibleSolution p : possibleMirrorsColumns) {
          if (isMirrorColumn(p.rows, p.mirrorIndex)) {
            result += p.mirrorIndex * 100;
          }
        }
        rows.clear();
      }
    }

    std::cout << std::endl << result << std::endl;
  }
  return 0;
}
