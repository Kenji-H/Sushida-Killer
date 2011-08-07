#include "bmp.hpp"

/*
 * 4�r�b�g����Int�����l�ɕϊ�
 */
static int bit2Integer(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
  return b1 +
         b2 * 256 +
         b3 * 256 * 256 +
         b4 * 256 * 256 * 256; 
}

/* 
 * �r�b�g�}�b�v�f�[�^�����[�h����
 */
void BitMapProcessor::loadData(string filename) {
  if (bmp != NULL)
    fclose(bmp);

  bmp = fopen(filename.c_str(), "rb");
  if (bmp == NULL)
    printf("�t�@�C���I�[�v���Ɏ��s���܂����B\n");

  readFileHeader();
  readInfoHeader();
  readBmpData();
}

/* 
 * �t�@�C���w�b�_��ǂ�
 */
void BitMapProcessor::readFileHeader() {
  uint8_t data[FILE_HEADER_SIZE];
  size_t size = fread(data, sizeof(uint8_t), FILE_HEADER_SIZE, bmp); 
  
  memcpy(fHeader.data, data, sizeof(data));
  fHeader.fileType = "";
  fHeader.fileType += data[0];
  fHeader.fileType += data[1];
  fHeader.fileSize = bit2Integer(data[2], data[3], data[4], data[5]);
}

/*
 * ���w�b�_��ǂ�
 */
void BitMapProcessor::readInfoHeader() {
  uint8_t data[INFO_HEADER_SIZE];
  size_t size = fread(data, sizeof(uint8_t), INFO_HEADER_SIZE, bmp);
  
  memcpy(iHeader.data, data, sizeof(data));
  iHeader.infoHeaderSize = bit2Integer(data[0], data[1], data[2], data[3]);
  iHeader.width          = bit2Integer(data[4], data[5], data[6], data[7]);
  iHeader.height         = bit2Integer(data[8], data[9], data[10], data[11]);
  iHeader.clrPerPixel    = bit2Integer(data[14], data[15], 0, 0);
  iHeader.dataSize       = bit2Integer(data[20], data[21], data[22], data[23]);
}

/* 
 * �摜�f�[�^��ǂ�
 */
void BitMapProcessor::readBmpData() {
  if (img != NULL)
    delete []img;

  int sz = iHeader.dataSize;
  img = new uint8_t [sz];
  size_t size = fread(img, sizeof(uint8_t), sz, bmp);
  if (size != sz)
    printf("�摜�f�[�^�ǂݍ��݂̃T�C�Y���������Ă��܂��B");

  // �o�b�N�A�b�v�p��org�ɉ摜�f�[�^���R�s�[
  if (org != NULL)
    delete []org;
  org = new uint8_t [sz];
  memcpy(org, img, sz);
}

/* 
 * �r�b�g�}�b�v���̕\���i�f�o�b�O�p�j
 */
void BitMapProcessor::dispBmpInfo() {
  cout << "���t�@�C���w�b�_���" << endl;
  cout << "�t�@�C���^�C�v: " << fHeader.fileType << endl;
  cout << "�t�@�C���T�C�Y: " << fHeader.fileSize << endl;

  cout << "�����w�b�_���" << endl;
  cout << "���w�b�_�T�C�Y: " << iHeader.infoHeaderSize << endl;
  cout << "�摜��: " << iHeader.width << endl;
  cout << "�摜��: " << iHeader.height << endl;
  cout << "�P�s�N�Z��������̐F��: " << iHeader.clrPerPixel << endl;
  cout << "�摜�f�[�^�̃T�C�Y: " << iHeader.dataSize << endl;
}

/*
 * �r�b�g�}�b�v�f�[�^�̃t�@�C�������o��
 */
void BitMapProcessor::writeData(string filename) {
  FILE *out = fopen(filename.c_str(), "wb");
  
  if (out == NULL)
    printf("�����o����̃t�@�C�����J���܂���B\n");

  fwrite(fHeader.data, sizeof(uint8_t), FILE_HEADER_SIZE, out);
  fwrite(iHeader.data, sizeof(uint8_t), INFO_HEADER_SIZE, out);
  fwrite(img, sizeof(uint8_t), iHeader.dataSize, out);
  
  fclose(out);
}

/*
 * �w�肳�ꂽ�s�N�Z���̐F���擾
 */
Color BitMapProcessor::getColor(int row, int col) {
  if (row < 0 || row >= iHeader.height)
    printf("getColor(): row���͈͊O�ł��B\n");
  if (col < 0 || col >= iHeader.width )
    printf("getColor(): col���͈͊O�ł��B\n");

  int width = 3 * iHeader.width;
  while (width % 4)        // �r�b�g�}�b�v��1���4�̔{���r�b�g����Ȃ�
    ++width;
  
  int bPos = row * width + 3 * col;
  int gPos = bPos + 1;
  int rPos = bPos + 2;

  Color color;
  color.r = img[rPos];
  color.g = img[gPos];
  color.b = img[bPos];

  return color;
}

/* 
 * �w�肳�ꂽ�s�N�Z���ɐF��ݒ�
 */
void BitMapProcessor::setColor(int row, int col, int r, int g, int b) {
  if (row < 0 || row >= iHeader.height)
    printf("getColor(): row���͈͊O�ł��B\n");
  if (col < 0 || col >= iHeader.width )
    printf("getColor(): col���͈͊O�ł��B\n");

  int width = 3 * iHeader.width;
  while (width % 4)        // �r�b�g�}�b�v��1���4�̔{���r�b�g����Ȃ�
    ++width;
  
  int bPos = row * width + 3 * col;
  int gPos = bPos + 1;
  int rPos = bPos + 2;

  img[rPos] = r;
  img[gPos] = g;
  img[bPos] = b;
}

/* 
 * �r�b�g�}�b�v�f�[�^�����H�O�ɕ�������
 */
void BitMapProcessor::restore() {
  memcpy(img, org, iHeader.dataSize);
}
