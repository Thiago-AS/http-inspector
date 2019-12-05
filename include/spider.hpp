#include<vector>
#include "./proxy.hpp"
using namespace std;

class Spider
{
    private:
    public:
        ofstream file;
        Proxy *proxy;
        vector<string> references;

        void run(string, string, int, int, int);
        void getPageReference(string, int, int, fstream&);
        int searchLineReference(string, int, string, int, string, int, int);
        Spider(Proxy* proxy):proxy(proxy){};
        ~Spider(){};
};