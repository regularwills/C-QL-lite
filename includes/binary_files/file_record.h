#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <string.h>
#include "utilities.h"


using namespace std;

class FileRecord{
public:
    FileRecord(){
        _record[0][0] = '\0';
        recno = -1;
    }


    FileRecord(vector<string> str){
        for (int i = 0; i < str.size(); i++){
            strncpy(_record[i], str[i].c_str(), MAX);
        }
        _record[str.size()+1][0] = '\0';
    }

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file


    friend ostream& operator<<(ostream& outs, const FileRecord& r);


//private:
    static const int MAX = 100;
    int recno;
    char _record[MAX+1][MAX+1]; //NULL char
};

#endif