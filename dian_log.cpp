#include "dian_log.h"
#include <iostream>
using namespace std;

DianLog::DianLog()
{
}

DianLog::~DianLog()
{
}

void DianLog::logRed(string str)
{
    printf("\033[31m");
    cout << str;
    printf("\033[0m\n");
}

void DianLog::logRedLine()
{
    logRed("--------------------------------华丽的分割线----------------------------------");
}
