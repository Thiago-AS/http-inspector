#include "../include/proxy.hpp"
#include "../include/helper.hpp"
#include "../include/spider.hpp"

int main(int argc, const char **argv) {
    unsigned int port = get_port(argc, argv);
    Proxy* web_proxy = new Proxy(port);
    Spider* web_spider = new Spider(web_proxy);
    try{
        web_proxy->create_server();
        cout << "[PROXY INFO] - Socket created successfully on port: " 
        << port << endl;
        web_proxy->loop();
    } catch (const Error& e) {
        cout << e.what() << endl;
        delete web_proxy;
    }
}
