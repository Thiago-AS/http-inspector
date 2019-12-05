#include<vector>
#include "./proxy.hpp"
using namespace std;

/**
 *  @class Spider
 *  Defines everything needed to run the spider
 */
class Spider
{
    public:
        /** Spider file */
        ofstream file;
        /** Proxy reference */
        Proxy *proxy;
        /** References found in file */
        vector<string> references;

        /**
         *  Run the spider
         *  @param file_name to be searched or saved
         *  @param host name
         *  @param tree depth
         *  @param actual depth (needed for recursion)
         */
        void run(string, string, int, int);
        /**
         *  Search for all references in page
         *  @param host name
         *  @param actual depth (needed for recursion)
         *  @param tree depth
         *  @param data file
         */
        void get_page_references(string, int, int, fstream&);
        /**
         *  Get all references from fields in line
         *  @param Search toke : HREF or SRC
         *  @param offset to be skipped
         *  @param line to be searched
         *  @param position of token in line
         *  @param host name
         *  @param actual depth (needed for recursion)
         *  @param tree depth         
         */
        int get_line_reference(string, int, string, int, string, int, int);
        Spider(Proxy* proxy):proxy(proxy){};
        ~Spider(){};
};