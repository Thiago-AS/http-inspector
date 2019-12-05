#include "../include/dump.hpp"
using namespace std;

void Dump::createDumpDir(string filename) {
    string path = "../dump/" + filename;
    mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
}

void Dump::run(string filename) {
    //Proxy proxy;
	unsigned int currrentLine = 0, position = 0;
	string line;
    int i = 0;
	while(getline(this->spiderFile, line)) {
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        this->dump(line, "../dump/" + filename);
	}
    //.sendHttpRequest("../dump/" + filename + "/index.html", this->hostname, 2);

}

void Dump::dump(string url, string filename) {
    fstream file;
    size_t n = count(url.begin(), url.end(), '/');
    if (n > 2) {
        string path = filename + this->getSubPath(url);
        mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        this->dump(this->getRemainingPath(url), path);
    } else if (n == 2) {
        string path = filename + this->getSubPath(url);
        mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        if (url[url.length() - 1] == '/') {
            file.open(filename + "/index.html",ios::out);
            if (file) file.close();
        } else {
            file.open(path + this->getRemainingPath(url),ios::out);
            if (file) file.close();
        }
        return;
    } else {
        string path = filename + url;
        file.open(filename+ url,ios::out);
        if (file) file.close();
        return;
    }
}

string Dump::getRemainingPath(string url) {
    for(int i = 1; i < url.length(); i++)
	{
		if (url[i] == '/') {
			url = url.substr(i, url.length());
			return url;
		}
	}
}

string Dump::getSubPath(string url) {
    int i;
    for(int i = 1; i < url.length(); i++) {
        if (url[i] == '/' || i == url.length()) {
            url = url.substr(0, i);
            return url;
        }
    }
}