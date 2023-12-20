#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <queue>
#include <string>
#include <vector>
#include "../maps/map.h"
#include "../maps/multi_map.h"
#include "../string_tokenizer/token.h"

using namespace std;

//Maps/ MultiMaps:
typedef Map<string, string> map_ss;
typedef MMap<string, string> mmap_ss;
    
typedef Map<string, long> map_sl;
typedef MMap<string, long> mmap_sl;

//Vectors
typedef vector<string> vectorstr;
typedef vector<long> vectorlong;

//Queues
typedef queue<string> queuestr;
typedef queue<Token *> queuetokens;

#endif//TYPEDEFS_H
