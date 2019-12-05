#include "../include/spider.hpp"
#include "../include/proxy.hpp"
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#define PROXY_PORT 8228

using namespace std;

void Spider::run(string file_name, string addr, int tree_h, int act_h) {
	string path = file_name;
	replace( path.begin(), path.end(), '_', '/');
	this->references.push_back(path);
	if (act_h < tree_h){
		fstream file;
		string f_name = "http://"+addr+path;
		replace( f_name.begin(), f_name.end(), '/', '_');
		file.open("../cache/response_" + f_name);
		if(file.is_open()) {
			get_page_references(addr, act_h, tree_h, file);
		} else {
			proxy->send_http_request("GET " + path + " HTTP/1.1\r\nHost: " + addr + "\r\n\r\n\r\n", f_name);
		}
		file.close();
		if (act_h != 0) {
			// string cmd = "rm ../cache/response_" + f_name;
			// system(cmd.c_str());
		}
	}
	
}

void Spider::get_page_references(string addr, int act_h, int tree_h, fstream& req_file) {
	unsigned int position = 0;
	string line;
	while(getline(req_file, line)) { 
		while (line.find("href=",  position) != string::npos) {
			position = get_line_reference("href=", 6, line, position, addr, act_h, tree_h);
		}
		position = 0;
		while (line.find("src=\"", position) != string::npos) {
			position = get_line_reference("src=\"", 5 ,line, position, addr, act_h, tree_h);
		}
	}
}

int Spider::get_line_reference(string search_token, int offset, string line, int position, 
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
		if (value.find("http://" + addr) != string::npos || value.find("http") == string::npos) {
			if(value.find("http://" + addr) != string::npos){
				value = value.substr(7, value.size());
				value = value.substr(value.find('/'), value.size());
			}else{
				value.insert(0, 1, '/');
			}
			int flag = 0;
			for(unsigned int i = 0; i < this->references.size() ; i++)
			{
				if (this->references[i] == value) {
					flag = 1;
					break;
				}
			}
			if (!flag) {
				for(int i = 0; i < act_h + 1; i++) {
					this->file << "\t";
				}
				this->file << value << endl;
				this->references.push_back(value);
				if (value.find(".jpg") == string::npos && value.find(".png") == string::npos
				&& value.find(".ico") == string::npos && value.find(".gif") == string::npos
				&& value.find(".js") == string::npos && value.find(".css") == string::npos) {
					replace( value.begin(), value.end(), '/', '_');
					this->run(value, addr, tree_h, act_h+1);
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