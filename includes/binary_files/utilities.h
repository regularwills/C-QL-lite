#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

/*
    Useful functions for binary files:
    Handling binary files to read and write information to
*/

bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);

void write_info(const char filename[], vector<string>& v);
void read_info(const char filename[], vector<string>& v);

#endif