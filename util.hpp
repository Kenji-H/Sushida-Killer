#ifndef _UTIL_H
#define _UTIL_H

#include <sys/time.h>
#include <string>
#include <sstream>

using namespace std;

double gettimeofday_sec();
string getDate();
string replaceString(string str, char s, char t);
string removeString(string str, char s);
void sleep(double sec);
#endif
