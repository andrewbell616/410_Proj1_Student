/***
 * KP Utils
 */
#include <numeric>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "utilities.h"
#include "stdlib.h"

using namespace std;

//********************** private to this compilation unit **********************

vector<process_stats> process_stats_vector;

//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

int loadData(const char* filename, bool ignoreFirstRow) {

	process_stats_vector.clear();

	ifstream inputFile;
	inputFile.open(filename);

	if (!inputFile.is_open()) { return COULD_NOT_OPEN_FILE; }

	string line, value;
	vector<string> lineValues;

	while ( getline(inputFile, line) )
	{
		if (ignoreFirstRow)
		{
			ignoreFirstRow = false;
			continue;
		}

		lineValues.clear();
		stringstream ss(line);

		while ( getline(ss, value, ',') )
		{
			lineValues.push_back(value);
		}

		if (lineValues.size() < 4)
		{
			continue;
		}

		bool valid_line = true;

		for(string val : lineValues)
		{
			bool valid = false;

			for( const char c : val)
			{
				if ( isdigit(c) )
				{
					valid = true;
				}
			}

			if (!valid)
			{
				valid_line = false;
			}
		}

		if (!valid_line)
		{
			continue;
		}

		process_stats stat =
		{
			.process_number = stringToInt(lineValues[0].c_str()),
			.start_time = stringToInt(lineValues[1].c_str()),
			.cpu_time = stringToInt(lineValues[2].c_str()),
			.io_time = stringToInt(lineValues[3].c_str())
		};

		process_stats_vector.push_back(stat);
	}

	return SUCCESS;
}

bool compareCpuTime(process_stats ps1, process_stats ps2)
{
	return ps1.cpu_time < ps2.cpu_time;
}

bool compareProcessNumber(process_stats ps1, process_stats ps2)
{
	return ps1.process_number < ps2.process_number;
}

bool compareStartTime(process_stats ps1, process_stats ps2)
{
	return ps1.start_time < ps2.start_time;
}

bool compareIoTime(process_stats ps1, process_stats ps2)
{
	return ps1.io_time < ps2.io_time;
}

//will sort according to user preference
void sortData(SORT_ORDER mySortOrder) {
	switch (mySortOrder) {
		case SORT_ORDER::CPU_TIME:
			sort(process_stats_vector.begin(), process_stats_vector.end(), compareCpuTime);
			break;
		case SORT_ORDER::PROCESS_NUMBER:
			sort(process_stats_vector.begin(), process_stats_vector.end(), compareProcessNumber);
			break;
		case SORT_ORDER::START_TIME:
			sort(process_stats_vector.begin(), process_stats_vector.end(), compareStartTime);
			break;
		case SORT_ORDER::IO_TIME:
			sort(process_stats_vector.begin(), process_stats_vector.end(), compareIoTime);
			break;
		default:
			break;
	}
}

process_stats getNext() {
	process_stats myFirst = process_stats_vector[0];

	process_stats_vector.erase(process_stats_vector.begin());

	return myFirst;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows(){
	return process_stats_vector.size();
}


