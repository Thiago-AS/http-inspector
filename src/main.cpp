#include "../include/proxy.hpp"
#include "../include/helper.hpp"

int main(int argc, const char **argv) {
    unsigned int port = get_port(argc, argv);
    Proxy* web_proxy = new Proxy(port);
    try{
        web_proxy->create_socket();
        cout << "[INFO] - Socket created successfully on port: " 
        << port << endl;
        web_proxy->loop();
    } catch (const Error& e) {
        cout << e.what() << endl;
        delete web_proxy;
    }
}
