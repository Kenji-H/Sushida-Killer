#ifndef _WIN_H
#define _WIN_H

#define _WIN32_WINNT 0x500
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <winable.h>
#include <string>
#include <winuser.h>
#include <iostream>
#include<wingdi.h>
#include "util.hpp"

#define SCREEN_SHOT_TIME 0.15

using namespace std;

/*
 * ウィンドウ構造体
 */
typedef struct  cell{
	HWND hWnd;
	char WindowName[256];
} cell;

/*
 * アプリケーションのウィンドウ群構造体
 */
typedef struct appWndConfig {
  HWND hWnd;
  int layerNum;
  char className[15][256];
} appWndConfig;

double gettimeofday_sec();
BOOL CALLBACK EnumWndGetClass (HWND hWnd, LPARAM lParam);
HWND searchWndbyClass(string className);
BOOL appWndCheck(HWND hWnd, appWndConfig *conf);
HWND getChildOf(HWND hWnd, int n);
BOOL CALLBACK EnumWndGetApp (HWND hWnd, LPARAM lParam);
void putString(string str);
void putSpaceKey();
string getWindowTitle(HWND hWnd);
string getWindowClass(HWND hWnd);
HWND searchWndbyPos(BOOL msg);
HWND activateWnd(HWND hWnd);
void DispClipText(void);
void GetScreeShot();
HBITMAP DispBitMap();
BOOL SaveBitmapFile(const HBITMAP hBmp, const char *szFileName, const int iColors);


#endif
