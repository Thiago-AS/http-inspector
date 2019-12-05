#include "../include/spider.hpp"
#include "../include/proxy.hpp"
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#define PROXY_PORT 8228

using namespace std;


void Spider::run(string filename, string addr, int tree_h, int act_h, int type) {
	string path = filename;
	replace( path.begin(), path.end(), '_', '/');
	this->references.push_back(path);
	if (act_h < tree_h){
		fstream file;
        this->proxy->create_http_socket(addr);
		this->proxy->send_http_request("msg");
		file.open("../cache/response_" + filename);
		if(file.is_open()) {
			getPageReference(addr, act_h, tree_h, file);
		} else {
			throw Error("Error opening response file");
		}
		file.close();
		if (act_h != 0) {
			string cmd = "rm ../cache/response_" + filename;
			system(cmd.c_str());
		}
	}
	
}

void Spider::getPageReference(string addr, int act_h, int tree_h, fstream& requestFile) {
	unsigned int position = 0;
	string line;
	while(getline(requestFile, line)) { 
		while (line.find("href=",  position) != string::npos) {
			position = searchLineReference("href=", 6, line, position, addr, act_h, tree_h);
		}
		position = 0;
		while (line.find("src=\"", position) != string::npos) {
			position = searchLineReference("src=\"", 5 ,line, position, addr, act_h, tree_h);
		}
	}
}

int Spider::searchLineReference(string search_token, int offset, string line, int position, 
	string addr, int act_h, int tree_h) {
	string value;
	int pos = line.find(search_token, position), i = 0;
	if (line[pos+offset] != '#') {
		for(i = pos + offset; ; i++)
		{
			if(i >= pos && line[i] != '"') value = value + line[i];
			else if ( i >= pos && line[i] == '"') {
				break;
			}
		}
		if (value.find("http://" + addr) != string::npos || line[pos+offset] == '/') {
			if (line[pos+offset] != '/') value = value.substr(7 + addr.length(), value.length());
			int hasValue = 0;
			for(int i = 0; i < this->references.size() ; i++)
			{
				if (this->references[i] == value) {
					hasValue = 1;
					break;
				}
			}
			if (!hasValue) {
				for(int i = 0; i < act_h + 1; i++) {
					this->file << "  ";
				}
				this->file << value << endl;
				this->references.push_back(value);
				if (value.find(".jpg") == string::npos && value.find(".png") == string::npos
				&& value.find(".ico") == string::npos && value.find(".gif") == string::npos
				&& value.find(".js") == string::npos && value.find(".css") == string::npos) {
					replace( value.begin(), value.end(), '/', '_');
					this->run(value, addr, tree_h, act_h+1, 0);
				}
				
			}
			return i + 1;
		} else {
			return pos + offset;  
		}
	} else {
		return pos + offset;  
	}
}