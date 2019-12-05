#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/stat.h>

using namespace std;

class Dump
{
    public:
        void createDumpDir(string);
        fstream spiderFile;
        string hostname;
        void dump(string, string);
        string getRemainingPath(string);
        string getSubPath(string);
        string getPath(string);
        void run(string);
        Dump(){};
        ~Dump(){};
};