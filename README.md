# HTTP Inspector

## About

Proxy HTTP that handles only get requests and can create a file tree from source using a spider.

##  Requirements
  - [C++](http://www.cplusplus.com/)
  - Linux or Posix System

```bash
$ sudo apt-get install build-essential
```

##  Compile and execute

To compile and execute:

```bash
$ cd src/
$ make
$ ./aracne
```
The default port that is running is 8228, but it can be changed with the following
cli:

```bash
$ ./aracne -p <port number>
```


To clean all temp files and objects
```bash
$ make clean
```

## Team

|         Nome          |               Email               |     GitHub                                                          |
|:---------------------:|:---------------------------------:|:-------------------------------------------------------------------:|
|  Thiago Araújo 15/0149832  |  [thiagos.araujo09@gmail.com]()   |   [@Thiag-AS](https://github.com/Thiago-AS)              