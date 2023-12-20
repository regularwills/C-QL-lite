#include "file_record.h"

long FileRecord::write(fstream &outs){
    //take the Record r and write it into file f
    //  and return the record number of this Record

    outs.seekg(0, outs.end);    //write to the end of this file

    long pos = outs.tellp();    //retrieves the current position of the file pointer

    outs.write(_record[0], sizeof(_record));

    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //example:    r.read(f, 6)     cout<<r<<endl
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record)) ??
    ins.read(_record[0], sizeof(_record));
    
    //Mark the end of a string in the _record array with a null
    _record[0][ins.gcount()] = '\0';
    return ins.gcount();
}

ostream& operator<<(ostream& outs, const FileRecord& r){
    for (int i = 0; i <= strlen(r._record[i]); i++){
        outs << setw(25) << r._record[i];
    }
    return outs;
}

