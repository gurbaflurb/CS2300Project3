/*
Sean Chen
Last modified: Nov. 8, 2019
Written for CS 2300 Sec. 002
source.cpp

This program reads in two files, one that contains a matrix, and the other that contains the answers to said matrix in row echelon form. Then the program outputs the matrix
with the product to the screen. Then the program figures out what each of the variables, x1, x2,..., xn. Then it performs gaussian elimination on the matrix and the product 
to find all the values for each of the variables. Then the program outputs the matrix and product with the found zeros. Then the program outputs the solution to each of the 
variables to the screen.

*/
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <tuple>

bool verbose = false;
/*
readData function, takes in a file name as a string for both the prodvec and sysmat files. Then it attempts to open the file and read data from the 
file in the given format. Then it returns a tuple containing a vector for the data in the file and the dimension of the matrix read in.
*/
std::tuple<std::vector<float>, int> readData(std::string fileName, 
                                             std::string productFileName) {
    std::vector<float> matrix;
    std::fstream file;
    std::fstream file2;
    std::string line;
    int dimensions;

    file.open(fileName);
    file2.open(productFileName);

    if(file.is_open() && file2.is_open()) {
        std::getline(file, line, ' ');
        dimensions = stoi(line);
        std::getline(file2, line, ' ');
        if(dimensions != stoi(line)) {
            throw std::invalid_argument("Matrix dimensions don't match!");
        }
        while(!file.eof()) {
            for(int i = 0; i < dimensions; i++) {
                std::getline(file, line, ' ');
                matrix.push_back(stof(line));
            }
            std::getline(file2, line, ' ');
            matrix.push_back(stof(line));
        }
    }
    else {
        throw std::invalid_argument("File couldn't be opened!");
    }

    file.close();
    file2.close();

    return std::make_tuple(matrix, dimensions);
}

/*
printMatrix function takes in a vectors for the matrix and an integer for the dimensions, then it attempts to output the matrix to the users screen
with the regular matrix and the product matrix conjoined in one grid.
*/
void printMatrix(std::vector<float> &matrix, int dimensions) {
    int count = 0;
    for (std::vector<float>::iterator it = matrix.begin(); it != matrix.end(); it++) {
        if(count < dimensions) {
            std::cout << *it << ", ";
            count++;
        }
        else {
            std::cout << " | " << *it <<std::endl;
            count = 0;
        }
    }
}

/*
reduce function takes a matrix by reference, the number of dimensions and a pass through value to reduce all the values in the given row. It then returns
the reduced row.
*/
std::vector<float> reduce(std::vector<float> &matrix, int dimensions, 
                          int passThrough) {
    std::vector<float> newRow;
    int count = 0;
    std::vector<float>::iterator it = matrix.begin();
    while(*it == 0) {
        it++;
        count++;
    }
    float firstValue = *it;
    for(int i = 0; i < count; i++) {
        newRow.push_back(0);
    }
    for(it; it != matrix.end(); it++) {
        newRow.push_back(*it/firstValue);
    }

    return newRow;
}

/*
makeRow function takes a pointer to the beginning of two vectors that will be used to determine the row being replaced and return a vector which
contains said row.
*/
std::vector<float> makeRow(std::vector<float>::iterator firstColumn, 
                           std::vector<float>::iterator secondColumn, 
                           int dimensions, 
                           int passThrough) {
    std::vector<float> newRow;
    float firstValue = *(firstColumn);
    float secondValue = *(secondColumn);
    
    for(int i = 0; i < dimensions+1-passThrough; i++) {
        float value = (firstValue*secondColumn[i]-secondValue*firstColumn[i]);
        newRow.push_back(value);
    }
    newRow = reduce(newRow, dimensions, passThrough);
    return newRow;
}

/*
pivot function takes in a matrix in the form of a vector and an integer for the dimensions of said matrix. Then it attempts to pivot the matrix so that the largest
value is on top. It then returns the new matrix after the pivot.
*/
void pivot(std::vector<float> &matrix, int dimensions) {
    std::vector<float>::iterator firstRow = matrix.begin();
    std::vector<float>::iterator nextRow = matrix.begin()+dimensions+1;
    float temp;
    for(int i = 0; i < dimensions-1; i++) {
        if(std::abs(*firstRow) < std::abs(*nextRow)) {
            for(int j = 0; j <= dimensions; j++) {
                temp = *(nextRow+j);
                *(nextRow +j) = *(firstRow+j);
                *(firstRow+j) = temp;
            }
        }
        nextRow += dimensions+1;
    }
}

/*
gaussianElimination function takes in a vector and the dimensions for the matrix and then attempts to perform gaussian elimination on the
matrix and solving the system of equations. 
*/
std::vector<float> gaussianElimination(std::vector<float> &matrix, 
                                       int dimensions) {
    std::vector<float> newMatrix;
    std::vector<float> tempMatrix;
    pivot(matrix, dimensions);
    std::cout << "\nAfter pivot" << std::endl;
    printMatrix(matrix, dimensions);
    std::vector<float>::iterator it = matrix.begin();
    std::vector<float>::iterator next = it+dimensions+1;
    std::vector<float>::iterator nextColumn = next;
    
    for (int i = 0; i < dimensions-1; i++) {
        for (int j = i; j < dimensions-1; j++) {
            tempMatrix = makeRow(it, nextColumn, dimensions, i);
            int count = 0;
            for(std::vector<float>::iterator iterator = tempMatrix.begin(); 
                iterator != tempMatrix.end(); 
                iterator++) {
                nextColumn[count] = *iterator;
                count++;
            }
            nextColumn = nextColumn+dimensions+1;
        }
        if(verbose) {
            std::cout << "\nMatrix after Pass " << i+1 << std::endl;
            printMatrix(matrix, dimensions);
        }
        it = next+1;
        nextColumn = it+dimensions+1;
        next = nextColumn;
    }
    for(std::vector<float>::iterator iter = matrix.begin(); 
        iter != matrix.end(); iter++) {
        newMatrix.push_back(*iter);
    }
    return newMatrix;
}

/*
solveMatrix function is passed a matrix and then attempts to solve said matrix by working backwards to determine all the elements in the matrix and then
solving the systems of equations using such.
*/
void solveMatrix(std::vector<float> &matrix, int dimensions) {
    float solvedValue[dimensions];
    float currentRow[dimensions];
    std::fill_n(solvedValue, dimensions, 1);
    int count = 0;

    for (std::vector<float>::iterator it = matrix.end()-1;
         it != matrix.begin()-1; it--) {
        solvedValue[count] = *it;
        for(int i = 1; i < dimensions+1; i++) {
            currentRow[i-1] = *(it-i);
        }
        for(int i = 0; i < dimensions; i++) {
            if(i == count) {
                continue;
            }
            else {
                float value = currentRow[i]*solvedValue[i];
                if(currentRow[i] == 0) {
                    continue;
                }
                else if(value > 0) {
                    solvedValue[count] -= value;
                }
                else if(value < 0) {
                    solvedValue[count] += (-1*value);
                }
                
            }
        }
        solvedValue[count] = solvedValue[count]/currentRow[count];
        it -= dimensions;
        count++;
    }
    for(int i = dimensions-1; i > -1; i--) {
        std::cout << solvedValue[i] << std::endl;
    }
    std::cout << std::endl;
}

/*
printUsage function prints the usage of the program to the screen
*/
void printUsage() {
    std::cout << "Usage:\n$./main [options]" << std::endl;
    std::cout << "Options\n    -v --verbose | Prints verbose information" << std::endl;
    std::cout << "    -h --help    | Prints this menu" << std::endl;
    exit(0);
}

/*
parseArgs function parses the user input and either sets the program to be verbose or prints the usage of the program to the user
*/
void parseArgs(int argc, char **argv) {
    if(argc > 1) {
        if(std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
            printUsage();
        }
        if(std::string(argv[1]) == "-v" || std::string(argv[1]) == "--verbose") {
            verbose = true;
        }
    }
}

/*
runGaussian function simply takes in the matrix name, and two file names for the files to be opened. It then runs gaussian elimination on the matrix
*/
void runGaussian(std::string matrixRun, std::string file1Name, 
                 std::string file2Name) {
    std::vector<float> matrix;
    int matrixDimentions = 0;
    
    std::cout << matrixRun << std::endl;
    std::tie(matrix, matrixDimentions) = readData(file1Name, file2Name);
    printMatrix(matrix, matrixDimentions);
    matrix = gaussianElimination(matrix, matrixDimentions);
    std::cout << "\nAfter Gaussian" << std::endl;
    printMatrix(matrix, matrixDimentions);
    std::cout << "\nSolved values from the matrix" << std::endl;
    solveMatrix(matrix, matrixDimentions);
}

/*
Start of the main function for the program
*/
int main(int argc, char **argv) {
    parseArgs(argc, argv);
    
    runGaussian("Matrix 1", "sysmat1.txt", "prodvec1.txt");
    std::cout << "Press enter to run the 10x10 matrix..." << std::endl;
    std::cin.ignore();
    runGaussian("Matrix 2", "sysmat2.txt", "prodvec2.txt");

    return 0;
}