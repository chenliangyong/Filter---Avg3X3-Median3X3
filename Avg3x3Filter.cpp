#include <iostream> 
#include <fstream> 
#include <string>
using namespace std;
/*
--- Please use C++ 11 when you Compile in Venus  ---g++ -std=gnu++11 Histogram.cpp -o histogram.out
*/

class Avg3x3Filter {
private:
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int newMin;
	int newMax;
	//2D array, need to dynamically allocate at run time of size numRows + 2 by numCols + 2.
	int **mirrorFramedAry;
	int **tempAry;
	//1D array for storing the 3 x 3 neighbors of pixel (i,j) 
	int neighborAry[9] = {0};

	string inFile;
	string outFile = "AVG3X3Out.txt"; // Default output file name, will be replaced by argv[2] which is not null
	
	int AVG3X3() {
		int sum = 0;
		for (int x = 0; x < 9; x++) {
			sum += neighborAry[x];
		}
		return sum / 9;
	}

public:
	Avg3x3Filter(char **File) {
		inFile = File[1];
		//Replace the default output file name;
		if (File[2] != NULL) {
			outFile = File[2];
		}
		cout << "Input image file name is :" << inFile << "  Output image file name is :" << outFile << endl;

		// loading the image data
		cout << "Start Reading Image..." << endl;
		HeadLoading();

		//initialize 2D array
		mirrorFramedAry = new int*[numRows + 2];
		tempAry = new int*[numRows + 2];
		for (int i = 0; i < numRows + 2; i++) {
			mirrorFramedAry[i] = new int[numCols + 2];
			tempAry[i] = new int[numCols + 2];
			for (int j = 0; j < numCols + 2; j++) {
				mirrorFramedAry[i][j] = 0;
				tempAry[i][j] = 0;
			}
		}
	}
	~Avg3x3Filter() {
		for (int i = 0; i < numRows + 2; i++) {
			delete[] mirrorFramedAry[i];
			delete[] tempAry[i];
		}
		delete[] mirrorFramedAry;
		delete[] tempAry;
	}

	void HeadLoading() {
		fstream finput;
		finput.open(inFile);
		if (!finput.is_open()) {
			throw "Error ! Open Input File Failed, try again...";
		}
		if (!finput.eof() && finput.peek() != EOF) {
			finput >> numRows;
			finput >> numCols;
			finput >> minVal;
			newMin = minVal;
			finput >> maxVal;
			newMax = maxVal;
		}
		else {
			finput.close();
			throw "Error ! Input File is empty, try again...";
		}
		finput.close();
	}

	void mirrorFramed() {
		loadImage();
		for (int i = 1; i < numRows + 1 ; i++) {
			mirrorFramedAry[i][0] = mirrorFramedAry[i][1];
			mirrorFramedAry[i][numCols + 1] = mirrorFramedAry[i][numCols];
		}
		for (int j = 0; j < numCols + 2; j++) {
			mirrorFramedAry[0][j] = mirrorFramedAry[1][j];
			mirrorFramedAry[numRows + 1][j] = mirrorFramedAry[numRows][j];
		}
		cout << "Finish MirrorFramed..." << endl;
	}
	void loadImage() {
		//Read the original image
		fstream finput;
		finput.open(inFile);
		if (!finput.is_open()) {
			throw "Error ! Open Input File Failed, try again...";
		}
		if (!finput.eof() && finput.peek() != EOF) {
			string temp;
			getline(finput, temp); // skip the image head
			int gVal = 0;
			for (int i = 1; i < numRows + 1; i++) {
				for (int j = 1; j < numCols + 1; j++) {
					if (!finput.eof() && finput.peek() != EOF) {
						finput >> gVal;
					}
					else { break; }
					mirrorFramedAry[i][j] = gVal;
				}
			}
		}
		else {
			finput.close();
			throw "Error ! Input File is empty, try again...";
		}
		finput.close();
		cout << "Finish Reading Original Image..." << endl;
	}

	void loadNeighbors() {
		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				neighborAry[0] = mirrorFramedAry[i - 1][j - 1];
				neighborAry[1] = mirrorFramedAry[i - 1][j];
				neighborAry[2] = mirrorFramedAry[i - 1][j + 1];
				neighborAry[3] = mirrorFramedAry[i][j - 1];
				neighborAry[4] = mirrorFramedAry[i][j];
				neighborAry[5] = mirrorFramedAry[i][j + 1];
				neighborAry[6] = mirrorFramedAry[i + 1][j - 1];
				neighborAry[7] = mirrorFramedAry[i + 1][j];
				neighborAry[8] = mirrorFramedAry[i + 1][j + 1];
				tempAry[i][j] = AVG3X3(); // get Avg result from 1D Array;
				if (tempAry[i][j] < minVal) newMin = tempAry[i][j];
				if (tempAry[i][j] > maxVal) newMax = tempAry[i][j];
			}	
		}
	}

	void printResult() {
		cout << "Printing New Image..." << endl;
		fstream foutput;
		foutput.open(outFile, ios::out);
		foutput << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
		cout << "Image size is " << numRows << "x" << numCols << ", " << "New Grey-Scale Range is " << newMin << "-" << newMax << ";" << endl;

		for (int i = 1; i < numRows + 1; i++) {
			for (int j = 1; j < numCols + 1; j++) {
				cout << tempAry[i][j] << " ";	// Print out the image
				foutput << tempAry[i][j] << " ";
			}
			cout << endl;
			foutput << endl;
		}
		foutput.close();
	}
};

int main(int argc, char *argv[])
{
	try {
		if (argc < 2) {
			cout << "Parameters missing... ( Run as : Avg3x3Filter <input file 1> OR Avg3x3Filter <input file 1> <output file 1> ).";
			return 0;
		}
		Avg3x3Filter Avg(argv);
		Avg.mirrorFramed();
		Avg.loadNeighbors();
		Avg.printResult();

	}
	catch (const char* msg) {
		cerr << msg << endl;
		return 0;
	}
	return 0;
};