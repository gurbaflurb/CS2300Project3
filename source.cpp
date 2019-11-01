/*
Sean Chen
Last modified: Oct. 30, 2019
Written for CS 2300 Sec. 002
source.cpp

This program reads in two files, one that contains a matrix, and the other that contains the answers to said matrix in row echelon form. Then the program outputs the matrix
with the product to the screen. Then the program figures out what each of the variables, x1, x2,..., xn. Then it performs gaussian elimination on the matrix and the product 
to find all the values for each of the variables. Then the program outputs the matrix and product with the found zeros. Then the program outputs the solution to each of the 
variables to the screen.

*/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <tuple>
#include <vector>

/*
readData function, takes in a file name as a string for both the prodvec and sysmat files. Then it attempts to open the file and read data from the 
file in the given format. Then it returns a tuple containing a vector for the data in the file and the dimension of the matrix read in.
*/
std::tuple<std::vector<float>, int> readData(std::string fileName, std::string productFileName) {
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
gaussianElimination function takes in a vector and the dimentions for the matrix and then attempts to perform gaussian elimination on the
matrix and solving the system of equations. 
*/
std::vector<float> gaussianElimination(std::vector<float> matrix, int dimensions) {
    std::vector<float> newMatrix;
    std::vector<float>::iterator it = matrix.begin();
    std::vector<float>::iterator nextColumn = it+dimensions;

    std::cout << "First Pass" << std::endl;
    for(int j = 0; j < dimensions; j++) {
        float firstValue = *it;
        float secondValue = *nextColumn;
        if(j == 0) {
            for(int i = 0; i < dimensions; i++) {
                newMatrix.push_back(it[i]);
                std::cout << it[i] << ' ';
            }
            std::cout << std::endl;
        }
        else {
            for(int i = 0; i < dimensions; i++) {
                float value = (int)((100000*(firstValue*nextColumn[i]))-(100000*(secondValue*it[i])));
                value = value/100000;
                newMatrix.push_back(value);
                std::cout << value << ' ';
            }
            std::cout << std::endl;
            nextColumn = nextColumn+dimensions;
        }
    }

    return newMatrix;
}

/*
printMatrix function takes in a vectors for the matrix and an integer for the dimensions, then it attempts to output the matrix to the users screen
with the regular matrix and the product matrix conjoined in one grid.
*/
void printMatrix(std::vector<float> matrix, int dimensions) {
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
Start of the main function for the program
*/
int main() {
    std::vector<float> matrix;
    int matrixDimentions = 0;

    std::cout << "Matrix 1" << std::endl;
    std::tie(matrix, matrixDimentions) = readData("sysmat1.txt", "prodvec1.txt");
    printMatrix(matrix, matrixDimentions);
    gaussianElimination(matrix, matrixDimentions);
    
    /*
    std::cout << "Matrix 2" << std::endl;
    std::tie(matrix, matrixDimentions) = readData("sysmat2.txt", "prodvec2.txt");
    printMatrix(matrix, matrixDimentions);
    gaussianElimination(matrix, matrixDimentions);
    */
    /*
    Do Gaussian elimination
    */

    return 0;
}