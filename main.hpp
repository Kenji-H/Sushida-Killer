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

#define LEARN_MODE 0                                   // �f�[�^�w�K���[�h
#define EXTRACT_MODE 1                                 // �����ʒ��o���[�h
#define EXTRACT_MODE_APPEND 2                          // �����ʒ��o���[�h�i�����̂ݒǉ��j
#define PLAY_MODE 3                                    // �v���C���[�h
#define SHOT_SPAN 10                                   // �X�i�b�v�V���b�g�擾�X�p���i�w�K�f�[�^�j
#define HASH_DATA_FILE "hash.txt"                      // �n�b�V���l���i�[�����t�@�C��
#define R0 320                                         // ������\���������W
#define R1 390                                         // ������\���������W
#define C0 570                                         // ������\���������W
#define C1 765                                         // ������\���������W
#define KEY_INPUT_TIME 0.3                             // �L�[���͂ɗv���鎞��
#define TEMP_FILE "data/tmp.bmp"                       // �r�b�g�}�b�v�̈ꎞ�t�@�C����
#define TYPING_ERR_RATE 0.00                           // �^�C�s���O�̃G���[��
#define PLAY_TIME (120*6)                              // ���i�Ńv���[����

void learnData();                                      // ���C�������i�f�[�^�w�K���[�h�j
void extractData();                                    // ���C�������i�����ʒ��o���[�h�j
void extractDataAppend();                              // ���C�������i�����ʒ��o�i�����ǉ��j���[�h�j
void playGame();                                       // ���C�������i�Q�[���v���C���[�h�j
HWND getHandleOfSushida();                             // ���i�ł̃n���h�����擾
long long int bmpHashValue(string filename);           // �r�b�g�}�b�v�f�[�^�̃n�b�V���l���v�Z

#endif
