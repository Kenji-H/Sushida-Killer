#ifndef _BMP_H
#define _BMP_H

#include <cstdio>
#include <iostream>
#include <cstring> 

#define FILE_HEADER_SIZE 14                // �t�@�C���w�b�_�̃T�C�Y
#define INFO_HEADER_SIZE 40                // ���w�b�_�̃T�C�Y

using namespace std;

/*
 * �t�@�C���w�b�_�[�\����
 */
typedef struct FileHeader {
  uint8_t data[FILE_HEADER_SIZE];  // ���H�O�f�[�^�i�����o���̂��ߕK�v�j
  string fileType;                 // �t�@�C���^�C�v
  int fileSize;                    // �t�@�C���T�C�Y
} FileHeader;

/*
 * ���w�b�_�[�\����
 */
typedef struct InfoHeader {
  uint8_t data[INFO_HEADER_SIZE];  // ���H�O�f�[�^�i�����o���̂��ߕK�v�j
  int infoHeaderSize;              // ���w�b�_�̃T�C�Y
  int width;                       // �摜�̕�
  int height;                      // �摜�̍���
  int clrPerPixel;                 // 1�s�N�Z��������̐F��
  int dataSize;                    // �摜�f�[�^�̃T�C�Y
} InfoHeader;

/* 
 * �s�N�Z���̐F���\����
 */
typedef struct Color {
  int r;                // �Ԑ���
  int g;                // �ΐ���
  int b;                // ����
} Color;

/*
 * 4�o�C�g�̒l��int�ɕϊ�
 */
static int bit2Integer(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);

/*
 * �r�b�g�}�b�v�����N���X
 */
class BitMapProcessor {
  FILE *bmp;               // �r�b�g�}�b�v�̃t�@�C���|�C���^
  uint8_t *img;            // �r�b�g�}�b�v�f�[�^�i���H�p�j
  uint8_t *org;            // �r�b�g�}�b�v�f�[�^�i�ǂݍ��ݎ��j
  FileHeader fHeader;      // �t�@�C���w�b�_
  InfoHeader iHeader;      // ���w�b�_

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
