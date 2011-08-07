#ifndef _MAIN_H
#define _MAIN_H

#include <cstdio>
#include <iostream>
#include <cstring>
#include <set>
#include <map>
#include <vector>
#include "win.hpp"

using namespace std;

#define LEARN_MODE 0                                   // データ学習モード
#define EXTRACT_MODE 1                                 // 特徴量抽出モード
#define EXTRACT_MODE_APPEND 2                          // 特徴量抽出モード（差分のみ追加）
#define PLAY_MODE 3                                    // プレイモード
#define SHOT_SPAN 10                                   // スナップショット取得スパン（学習データ）
#define HASH_DATA_FILE "hash.txt"                      // ハッシュ値を格納したファイル
#define R0 320                                         // 文字列表示部分座標
#define R1 390                                         // 文字列表示部分座標
#define C0 570                                         // 文字列表示部分座標
#define C1 765                                         // 文字列表示部分座標
#define KEY_INPUT_TIME 0.3                             // キー入力に要する時間
#define TEMP_FILE "data/tmp.bmp"                       // ビットマップの一時ファイル名
#define TYPING_ERR_RATE 0.00                           // タイピングのエラー率
#define PLAY_TIME (120*6)                              // 寿司打プレー時間

void learnData();                                      // メイン処理（データ学習モード）
void extractData();                                    // メイン処理（特徴量抽出モード）
void extractDataAppend();                              // メイン処理（特徴量抽出（差分追加）モード）
void playGame();                                       // メイン処理（ゲームプレイモード）
HWND getHandleOfSushida();                             // 寿司打のハンドルを取得
long long int bmpHashValue(string filename);           // ビットマップデータのハッシュ値を計算

#endif
