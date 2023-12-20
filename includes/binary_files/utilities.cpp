#include "utilities.h"

bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename, std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it
    if (!file_exists(filename)){
        //create the file
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else {
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else {
        f.open (filename, std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}

void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}

//Write elements from a vector into a text file
void write_info(const char filename[], vector<string>& v){
    ofstream f(filename);
    if (f.fail()){
        cout<<"textfile open fail: write"<<endl;
        exit(99);
    }
    for (int i = 0; i < v.size(); i++){
        f << v[i] << endl;
    }
    f << endl;
    f.close();
}

//Read data from a file into a vector
void read_info(const char filename[], vector<string>& v){
    fstream f(filename);
    if (f.fail()){
        cout <<"textfile open fail: read"<<endl;
        exit(99);
    }
    string str;
    while (getline(f, str)){        // !f.eof()
        v.push_back(str);
    }

    //Getline adds 1 extra index to the vector. so delete that unneeded index
    v.pop_back();
    f.close();
}
