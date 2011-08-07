#ifndef _BMP_H
#define _BMP_H

#include <cstdio>
#include <iostream>
#include <cstring> 

#define FILE_HEADER_SIZE 14                // ファイルヘッダのサイズ
#define INFO_HEADER_SIZE 40                // 情報ヘッダのサイズ

using namespace std;

/*
 * ファイルヘッダー構造体
 */
typedef struct FileHeader {
  uint8_t data[FILE_HEADER_SIZE];  // 加工前データ（書き出しのため必要）
  string fileType;                 // ファイルタイプ
  int fileSize;                    // ファイルサイズ
} FileHeader;

/*
 * 情報ヘッダー構造体
 */
typedef struct InfoHeader {
  uint8_t data[INFO_HEADER_SIZE];  // 加工前データ（書き出しのため必要）
  int infoHeaderSize;              // 情報ヘッダのサイズ
  int width;                       // 画像の幅
  int height;                      // 画像の高さ
  int clrPerPixel;                 // 1ピクセル当たりの色数
  int dataSize;                    // 画像データのサイズ
} InfoHeader;

/* 
 * ピクセルの色情報構造体
 */
typedef struct Color {
  int r;                // 赤成分
  int g;                // 緑成分
  int b;                // 青成分
} Color;

/*
 * 4バイトの値をintに変換
 */
static int bit2Integer(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);

/*
 * ビットマップ処理クラス
 */
class BitMapProcessor {
  FILE *bmp;               // ビットマップのファイルポインタ
  uint8_t *img;            // ビットマップデータ（加工用）
  uint8_t *org;            // ビットマップデータ（読み込み時）
  FileHeader fHeader;      // ファイルヘッダ
  InfoHeader iHeader;      // 情報ヘッダ

public:
  BitMapProcessor() {
    bmp = NULL;
    img = NULL;
    org = NULL;
  };

  ~BitMapProcessor() {
    fclose(bmp);
    delete []img;
    delete []org;
  }
  void loadData(string filename);
  void dispBmpInfo();
  void writeData(string filename);
  Color getColor(int row, int col);
  void setColor(int row, int col, int r, int g, int b);
  void restore();
  int height() { return iHeader.height; };
  int width() { return iHeader.width; };
private:
  void readFileHeader();
  void readInfoHeader();
  void readBmpData();

};

#endif
