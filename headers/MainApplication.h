#ifndef __MAINAPPLICATION_H__
#define __MAINAPPLICATION_H__

#include <iostream>
#include <memory>

#include <QApplication>

class MainApplication {
public:
    MainApplication(int& argc, char* argv[]);
    ~MainApplication();

    int run();

private:
    int argc;
    char** argv;
};

#endif // __MAINAPPLICATION_H__
