#include <iostream>
#include <fstream>
#include <vector>
#include "d_matrix.h"
#include "d_except.h"

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;   // Indicates that a cell is blank
const int SquareSize = 3; // The number of cells in a small square
const int BoardSize = SquareSize * SquareSize;
const int MinValue = 1;
const int MaxValue = 9;
int numSolutions = 0;


class board
{
public:
    board(int);
    void clear();
    void initialize(ifstream& fin);
    void print();
    void printConflicts();
    bool isBlank(int, int);
    ValueType getCell(int, int);
    void setCell(int, int, ValueType);
    void clearCell(int, int);
    bool isSolved();
    bool solveSudoku();
    bool isValid(int row,int col, int num);
    bool findempty(int &row,int &col);

private:
    matrix<ValueType> value;
    matrix<vector<int>> rowConflicts;
    matrix<vector<int>> colConflicts;
    matrix<vector<int>> squareConflicts;
    void updateConflicts(int, int, ValueType);
    int squareNumber(int, int);
};

board::board(int sqSize)
    : value(BoardSize + 1, BoardSize + 1), rowConflicts(BoardSize + 1, BoardSize + 1),
    colConflicts(BoardSize + 1, BoardSize + 1), squareConflicts(BoardSize + 1, BoardSize + 1)
{
    clear();
}

void board::clear()
{
    for (int i = 1; i <= BoardSize; i++)
        for (int j = 1; j <= BoardSize; j++)
        {
            value[i][j] = Blank;
        }

    for (int i = 1; i <= BoardSize; i++)
        for (int j = 1; j <= BoardSize; j++)
        {
            rowConflicts[i][j].clear();
            colConflicts[i][j].clear();
            squareConflicts[i][j].clear();
        }
}

void board::initialize(ifstream& fin)
{
    char ch;
    clear();

    for (int i = 1; i <= BoardSize; i++)
        for (int j = 1; j <= BoardSize; j++)
        {
            fin >> ch;
            if (ch != '.')
                setCell(i, j, ch - '0');
        }
}

int board::squareNumber(int i, int j)
{
    // Return the square number of cell i,j (counting from left to right,
// top to bottom. Note that i and j each go from 1 to BoardSize
    return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

void board::updateConflicts(int i, int j, ValueType newValue)
{
    int squareNum = squareNumber(i, j);

    for (int col = 1; col <= BoardSize; col++)
    {
        if (col != j && isBlank(i, col))
            rowConflicts[i][col].push_back(newValue);
    }

    for (int row = 1; row <= BoardSize; row++)
    {
        if (row != i && isBlank(row, j))
            colConflicts[row][j].push_back(newValue);
    }

    for (int row = 1; row <= BoardSize; row++)
        for (int col = 1; col <= BoardSize; col++)
        {
            if (squareNumber(row, col) == squareNum && (row != i || col != j) && isBlank(row, col))
                squareConflicts[row][col].push_back(newValue);
        }
}


void board::setCell(int i, int j, ValueType newValue)
{
    if (value[i][j] != Blank)
        throw rangeError("Attempting to set a non-blank cell");

    value[i][j] = newValue;
    updateConflicts(i, j, newValue);
}

void board::clearCell(int i, int j)
{
    if (isBlank(i, j))
        throw rangeError("Attempting to clear a blank cell");

    ValueType oldValue = value[i][j];
    value[i][j] = Blank;

    for (int col = 1; col <= BoardSize; col++)
        rowConflicts[i][col].clear();

    for (int row = 1; row <= BoardSize; row++)
        colConflicts[row][j].clear();

    int squareNum = squareNumber(i, j);
    for (int row = 1; row <= BoardSize; row++)
        for (int col = 1; col <= BoardSize; col++)
            if (squareNumber(row, col) == squareNum)
                squareConflicts[row][col].clear();
}

bool board::isBlank(int i, int j)
{
    if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
        throw rangeError("Bad value in setCell");
    return (getCell(i, j) == Blank);
}

ValueType board::getCell(int i, int j)
{
    if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
        return value[i][j];
    else
        throw rangeError("Bad value in getCell");
}

void board::print()
{
    for (int i = 1; i <= BoardSize; i++)
    {
        if ((i - 1) % SquareSize == 0)
        {
            cout << " -";
            for (int j = 1; j <= BoardSize; j++)
                cout << "---";
            cout << "-";
            cout << endl;
        }
        for (int j = 1; j <= BoardSize; j++)
        {
            if ((j - 1) % SquareSize == 0)
                cout << "|";
            if (!isBlank(i, j))
                cout << " " << getCell(i, j) << " ";
            else
                cout << " ";
        }
        cout << "|";
        cout << endl;
    }
    cout << " -";
    for (int j = 1; j <= BoardSize; j++)
        cout << "---";
    cout << "-";
    cout << endl;

}

void board::printConflicts()
{
    cout << "Row Conflicts:" << endl;
    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = 1; j <= BoardSize; j++)
        {
            cout << "(" << i << "," << j << "): ";
            for (int k = 0; k < rowConflicts[i][j].size(); k++)
                cout << rowConflicts[i][j][k] << " ";
            cout << endl;
        }
    }

    cout << "Column Conflicts:" << endl;
    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = 1; j <= BoardSize; j++)
        {
            cout << "(" << i << "," << j << "): ";
            for (int k = 0; k < colConflicts[i][j].size(); k++)
                cout << colConflicts[i][j][k] << " ";
            cout << endl;
        }
    }

    cout << "Square Conflicts:" << endl;
    for (int i = 1; i <= BoardSize; i++)
    {
        for (int j = 1; j <= BoardSize; j++)
        {
            cout << "(" << i << "," << j << "): ";
            for (int k = 0; k < squareConflicts[i][j].size(); k++)
                cout << squareConflicts[i][j][k] << " ";
            cout << endl;
        }
    }
}

bool board::isSolved()
{
    for (int i = 1; i <= BoardSize; i++)
        for (int j = 1; j <= BoardSize; j++)
            if (isBlank(i, j))
                return false;
    return true;
}

int main()
{
    ifstream fin;

    string fileName = "sudoku.txt";
    fin.open(fileName.c_str());
    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }
    try
    {
        board b1(SquareSize);
        while (fin && fin.peek() != 'Z')
        {
            b1.initialize(fin);
            b1.print();
            b1.printConflicts();
            cout << "Is Solved: " << (b1.isSolved() ? "Yes" : "No") << endl;
        }
    }
    catch (indexRangeError& ex)
    {
        cout << ex.what() << endl;
        exit(1);
    }
}