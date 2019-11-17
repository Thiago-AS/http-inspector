#include "../include/proxy.hpp"
#include "../include/helper.hpp"

int main(int argc, const char **argv) {
    Proxy* web_proxy = new Proxy(get_port(argc, argv));
}