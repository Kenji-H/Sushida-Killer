#include "util.hpp"

/*
 *�@�����擾
 */
string getDate() {
  time_t t = time(NULL);
  struct tm *now = localtime(&t);
  
  stringstream ss;
  ss << (now->tm_year+1900) << (now->tm_mon+1) << now->tm_mday;
  ss << now->tm_hour << now->tm_min << now->tm_sec;

  string ret;
  ss >> ret;

  return ret;
}

/*
 * ���Ԍv��
 */
double gettimeofday_sec() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double) t.tv_sec + (double) t.tv_usec * 1e-6;
}

/*
 * �X���[�v
 */
void sleep(double sec) {
  double start_t = gettimeofday_sec();

  while (gettimeofday_sec() - start_t < sec)
    ;
}

/* 
 * ������u��
 */
string replaceString(string str, char s, char t) {
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == s)
      str[i] = t;
  }
  
  return str;
}

/*
 * ��������
 */
string removeString(string str, char s) {
  string ret = "";

  for (int i = 0; i < str.length(); i++)
    if (str[i] != s)
      ret += str[i];
  
  return ret;
}
