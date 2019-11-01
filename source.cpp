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
#include <iomanip>
#include <tuple>
#include <vector>

/*
readData function, takes in a file name as a string. Then it attempts to open the file and read data from the file in the given format.
Then it returns a tuple containing a vector for the data in the file and the dimention of the matrix read in.
*/
std::tuple<std::vector<float>, float> readData(std::string fileName) {
    std::vector<float> matrix;
    std::fstream file;
    std::string line;
    float dimensions;

    file.open(fileName);

    if(file.is_open()) {
        std::getline(file, line, ' ');
        dimensions = stoi(line);
        while(!file.eof()) {
            std::getline(file, line, ' ');
            matrix.push_back(stof(line));
        }
    }
    else {
        throw std::invalid_argument("File couldn't be opened!");
    }

    file.close();

    return std::make_tuple(matrix, dimensions);
}

/*
gaussianElimination function takes in a vector and the dimentions for the matrix and then attempts to perform gaussian elimination on the
matrix and solving the system of equations. 
*/
std::vector<float> gaussianElimination(std::vector<float> matrix, std::vector<float> product, int dimensions) {
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
printMatrix function takes in two vectors for the matrix and the product and an integer, then it attempts to output the matrix to the users screen
with the regular matrix and the product matrix conjoined in one grid.
*/
void printMatrix(std::vector<float> matrix, std::vector<float> product, int dimensions) {
    int count = 0;
    std::vector<float>::iterator pit = product.begin();
    for (std::vector<float>::iterator it = matrix.begin(); it != matrix.end(); it++) {
        if(count < dimensions-1) {
            std::cout << *it << ' ';
            count++;
        }
        else {
            std::cout << *it << " | " << *pit <<std::endl;
            *pit++;
            count = 0;
        }
    }
}

/*
Start of the main function for the program
*/
int main() {
    std::vector<float> matrix;
    std::vector<float> product;
    int matrixDimentions, productDimentions = 0;

    std::cout << "Matrix 1" << std::endl;
    std::tie(matrix, matrixDimentions) = readData("sysmat1.txt");
    std::tie(product, productDimentions) = readData("prodvec1.txt");
    printMatrix(matrix, product, matrixDimentions);
    gaussianElimination(matrix, product, matrixDimentions);
    /*
    Do Gaussian elimination
    */
    //std::cout << "Matrix 2" << std::endl;
    //std::tie(matrix, matrixDimentions) = readData("sysmat2.txt");
    //std::tie(product, productDimentions) = readData("prodvec2.txt");
    //printMatrix(matrix, product, matrixDimentions);
    /*
    Do Gaussian elimination
    */

    return 0;
}