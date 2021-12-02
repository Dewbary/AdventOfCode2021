#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Read in input into a vector of ints. As a parameter it takes in the filename and returns a vector of ints
vector<int> readFileInputInt(string fileName);

// Read in input into a vector of strings. As a parameter it takes in the filename and return a vector of strings
vector<string> readFileInputString(string fileName);

// Takes vector<int> as input, returns total number of times the depth increased
int getDepthIncrease(vector<int>& measurements);

// Takes vector<int> as input, returns total number of times the sliding window of 3 increased
int getSlidingIncrease(vector<int>& measurements);

// Takes in vector<string> of course instructions, and returns the final horizontal and depth by each other
int calculatePosition(vector<string> &course);

// Takes in an instruction string, and parses it for the number of units to return
int getNumUnits(string &instruction);

// Increases horizontal by int numUnits
void forward(int &aim, int &depth, int &horizontalUnits, int numUnits);

// Increases depth by int numUnits
void down(int &aim, int numUnits) { aim += numUnits; }

// Decreases depth by int numUnits
void up(int &aim, int numUnits) { aim -= numUnits; }

// Main
int main(int argc, char* argv[]) {
    // Get input file 1
    vector<int> measurements = readFileInputInt("measurements.txt");

    // Get input file 2
    vector<string> course = readFileInputString("course.txt");

    // Test that input has been read successfully
//    for (auto i: course) {
//        cout << i << endl;
//    }

    // Call getDepthIncrease() to store total number of increases
    int numIncreases = getDepthIncrease(measurements);
    cout << "Number of Increases: " << numIncreases << endl;

    // Call getSlidingIncreaase() to store the total number of increases between sliding windows
    int numSlidingIncreases = getSlidingIncrease(measurements);
    cout << "Number of Sliding Increases: " << numSlidingIncreases << endl;

    // Get Horizontal Position and depth
    int position = calculatePosition(course);
    cout << "Position: " << position << endl;

    return 0;
}

vector<int> readFileInputInt(string fileName) {
    ifstream myFile (fileName);
    vector<int> measurements;

    string myLine;
    if (myFile.is_open() ) { // check whether the file is open
        while (myFile) {
            getline (myFile, myLine);
            measurements.push_back(stoi(myLine));
        }
    }
    else {
        cout << "Couldn't open file\n";
    }

    return measurements;
}

vector<string> readFileInputString(string fileName) {
    ifstream myFile (fileName);
    vector<string> course;

    string myLine;
    if (myFile.is_open() ) {
        while (getline(myFile, myLine)) {
            course.push_back(myLine);
        }
    }
    else {
        cout << "Couldn't open file\n";
    }

    return course;

}

int getDepthIncrease(vector<int>& measurements) {
    int previous = measurements[0];  // Store value on the first line into previous
    int current = 0;
    int numIncreasedCount = 0;

    unsigned int numMeasurements = measurements.size();
    unsigned int i;

    // Loop through all measurements provided by the input
    for (i=1; i < numMeasurements; i++) {
        current = measurements[i];  // Get Current value
        if (current > previous) {  // if current > previous
            numIncreasedCount++;  // Increment numIncreased counter
        }
        previous = current;  // Store current into previous
    }

    return numIncreasedCount;  // return numIncreased counter
}

int getSlidingIncrease(vector<int>& measurements) {
    unsigned int numMeasurements = measurements.size();  // Get size of measurement list
    int increasedCount = 0;
    unsigned int i;

    if (numMeasurements < 3) { return 0; }  // If there are less than 3 measurements, return 0 increases

    int currentSum = measurements[0] + measurements[1] + measurements[2];  // Initialize the first sum of 3
    int nextSum;
    int firstIndex = 0;  // Store 1st value
    int nextIndex = 3;  // Store Next value for the sliding window

    for (i = nextIndex; i < numMeasurements-1; i++) {  // for all measurements starting at the nextIndex
        nextSum = (currentSum - measurements[firstIndex]) + measurements[nextIndex];  // Get the next sum and store it in nextSum

        if (nextSum > currentSum) {  // if nextSum is greater than firstSum
            increasedCount++;  // increased count goes up by 1
        }

        // Set values for next loop
        firstIndex++;  // Set 1st value to index + 1
        nextIndex++;  // Set next value to next value + 1
        currentSum = nextSum;  // Set first sum to next sum
    }

    return increasedCount;  // Return total number of increases in the sliding window
}

int calculatePosition(vector<string> &course) {
    int horizontalPosition = 0;
    int depth = 0;
    int aim = 0;
    int numDirections = course.size();
    int numUnits = 0;
    char instruction = ' ';
    unsigned int i;

    // Loop through all directions
    for (i=0; i < numDirections; i++) {
        // Get instruction type
        instruction = course[i][0];

        // Get numUnits
        numUnits = getNumUnits(course[i]);

        if (instruction == 'f') {  // If Forward instruction
            forward(aim, depth, horizontalPosition, numUnits);
        }
        else if (instruction == 'd') {  // If Down instruction
            down(aim, numUnits);
        }
        else if (instruction == 'u') {  // If Up instruction
            up(aim, numUnits);
        }
    }

    return horizontalPosition * depth;
}

int getNumUnits(string &instruction) {
    int numUnits = 0;
    stringstream ss;
    ss << instruction;  // convert the string instruction into a stringstream
    string tmpStr;
    int tmpInt;
    while (!ss.eof()) {
        ss >> tmpStr;  // take words into temp_str one by one
        if (stringstream(tmpStr) >> tmpInt) {  // try to convert string to int
            numUnits = tmpInt;
        }
        tmpStr = "";  // clear temp string
    }

    return numUnits;
}

void forward(int &aim, int &depth, int &horizontalUnits, int numUnits) {
    horizontalUnits += numUnits;
    depth += aim * numUnits;
}