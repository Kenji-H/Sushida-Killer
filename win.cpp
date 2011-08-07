#include "win.hpp"

/*
 * �ړI�̃N���X�������n���h����lParam�Ɋi�[
 */
BOOL CALLBACK EnumWndGetClass (HWND hWnd, LPARAM lParam) 
{
  if (getWindowClass(hWnd) == ((cell*)lParam)->WindowName)
    ((cell*)lParam)->hWnd = hWnd;

  return 1;
}

/*
 * �N���X���ŃE�B���h�E������
 */
HWND searchWndbyClass(string className)
{
  cell c;
  c.hWnd =NULL;
  strcpy(c.WindowName, className.c_str());

  EnumWindows( EnumWndGetClass, (LPARAM)&c);
  
  if (c.hWnd == NULL)
    printf("�E�B���h�E�̌����Ɏ��s���܂����B\n");
  else
    printf("�E�B���h�E�i%s�j�̌����ɐ������܂����B\n", className.c_str());
  
  return c.hWnd;
}

/*
 * �N���X������ړI�̃n���h�����ǂ����𔻒�
 */
BOOL appWndCheck(HWND hWnd, appWndConfig *conf) {
  HWND hWnd_t = hWnd;

  for (int i = 0; i < conf->layerNum; i++) {
    if (conf->className[i] != getWindowClass(hWnd_t))
      return 0;

    hWnd_t = GetWindow(hWnd_t, GW_CHILD);
  }

  return 1;
}

/*
 * n���̎q���̃n���h�����擾����
 */
HWND getChildOf(HWND hWnd, int n) {
  HWND hWnd_t = hWnd;

  for (int i = 0; i < n; i++)
    hWnd_t = GetWindow(hWnd_t, GW_CHILD);

  return hWnd_t;
}

/*
 * �E�C���h�E�񋓂̃R�[���o�b�N�֐�
 * appWndCheck()��True�̃n���h�����擾
 */
BOOL CALLBACK EnumWndGetApp (HWND hWnd, LPARAM lParam)
{
  if (appWndCheck(hWnd, (appWndConfig *)lParam)) {
    ((appWndConfig *)lParam)->hWnd 
      = getChildOf(hWnd, ((appWndConfig *)lParam)->layerNum-1);
  }
  return 1;
}


/*
 * �L�[���͂��V�~�����[�g����
 */
void putString(string str) {
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == '!' || str[i] == '?')
      keybd_event(VK_SHIFT,0,0,0);
    keybd_event(VkKeyScan(str[i]),0,0,0);  
    keybd_event(str[i], 0, KEYEVENTF_KEYUP, 0);
    if (str[i] == '!' || str[i] == '?')
      keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
  }
}

/*
 * �X�y�[�X�L�[����������
 */
void putSpaceKey() {
  keybd_event(VK_SPACE, 0, 0, 0);
  keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
}


/*
 * �^����ꂽ�n���h���̃E�B���h�E�^�C�g�����擾
 */
string getWindowTitle(HWND hWnd) {
  char buff[512];
  GetWindowText(hWnd, buff, sizeof(buff));

  return buff;
}

/*
 * �^����ꂽ�n���h���̃N���X�����擾
 */
string getWindowClass(HWND hWnd) {
  char buff[512];
  GetClassName(hWnd, buff, sizeof(buff));

  return buff;
}

/* 
 * �}�E�X���̂��Ă���A�v���P�[�V�����̃n���h�����擾
 */
HWND searchWndbyPos(BOOL msg) {
  POINT pos;
  GetCursorPos(&pos);
  HWND hWnd = WindowFromPoint(pos);
  
  if (msg) {
    printf("MOUSE POSITION: %d, %d\n", pos.x, pos.y);
    printf("WINDOW CLASS  : %s\n", getWindowClass(hWnd).c_str());
    printf("WINDOW TITLE  : %s\n", getWindowTitle(hWnd).c_str());
  }

  return hWnd;
}

/* 
 * �^����ꂽ�n���h���̃E�B���h�E���A�N�e�B�u�ɂ���
 */
HWND activateWnd(HWND hWnd) {
  if (!SetForegroundWindow(hWnd)) {
    printf("�E�B���h�E�i%s�j�̃t�H�A�O���E���h���Ɏ��s���܂����B\n", 
	   getWindowClass(hWnd).c_str());
  }

  HWND pre = SetActiveWindow(hWnd);
  if (pre == NULL)
    printf("�E�B���h�E�i%s�j�̃A�N�e�B�u���Ɏ��s���܂����B\n", 
	   getWindowClass(hWnd).c_str());

  return pre;
}

/*
 * �N���b�v�{�[�h�̒��g�i�e�L�X�g�j��\��
 */
void DispClipText(void)
{
  HANDLE hText;
  char *pText;
  
  OpenClipboard(NULL);

  hText = GetClipboardData(CF_TEXT);

  if(hText == NULL) {
    printf("�N���b�v�{�[�h�Ƀe�L�X�g�f�[�^�͂Ȃ��B\n");
  } else {
    pText = (char *)GlobalLock(hText);
    printf("%s\n", pText);
    GlobalUnlock(hText);
  }

  CloseClipboard();
}

/*
 * �N���b�v�{�[�h�̒��g�i�r�b�g�}�b�v�j���擾
 */
HBITMAP DispBitMap() 
{
  if (IsClipboardFormatAvailable(CF_BITMAP)) {
    OpenClipboard(NULL);
 
    HBITMAP hData;
    hData = (HBITMAP)GetClipboardData(CF_BITMAP);   

    CloseClipboard();

    return hData;
  }
  else {
    printf("�N���b�v�{�[�h�Ƀr�b�g�}�b�v�͑��݂��܂���B\n");
    return NULL;
  }
}

/*
 * �r�b�g�}�b�v�t�@�C���̕ۑ�
 */
BOOL SaveBitmapFile(const HBITMAP hBmp, const char *szFileName, const int iColors)
{
	HANDLE	hFile;
	HDC	hDC;
	DWORD	dw;
	DWORD	dwHeaderSize;
	DWORD	dwScanDataSize;
	BITMAPFILEHEADER	bmpFileHeader;
	BITMAPINFOHEADER	*pBmpInfoHdr;
	BYTE	*pHeaderBuffer;
	BYTE	*pScanDataBuffer;
	BITMAP	bmp;

	GetObject(hBmp, sizeof(BITMAP), &bmp);

	dwHeaderSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * iColors;
	pHeaderBuffer = new BYTE[dwHeaderSize];
	memset(pHeaderBuffer, 0, dwHeaderSize);

	pBmpInfoHdr = (BITMAPINFOHEADER*)pHeaderBuffer;
	switch (iColors) {
	case 2:
		pBmpInfoHdr->biBitCount = 1;
		break;
	case 16:
		pBmpInfoHdr->biBitCount = 4;
		break;
	case 256:
		pBmpInfoHdr->biBitCount = 8;
		break;
	case 0:
		pBmpInfoHdr->biBitCount = 24;
		break;
	}

	pBmpInfoHdr->biSize = sizeof(BITMAPINFOHEADER);
	pBmpInfoHdr->biWidth  = bmp.bmWidth;
	pBmpInfoHdr->biHeight = bmp.bmHeight;
	pBmpInfoHdr->biPlanes = 1;
	pBmpInfoHdr->biCompression = BI_RGB;
	pBmpInfoHdr->biSizeImage = 0;
	pBmpInfoHdr->biXPelsPerMeter = 0;
	pBmpInfoHdr->biYPelsPerMeter = 0;
	pBmpInfoHdr->biClrUsed = 0;
	pBmpInfoHdr->biClrImportant = 0;

	hDC = GetDC(NULL);
	GetDIBits(hDC, hBmp, 0, bmp.bmHeight, NULL, (LPBITMAPINFO)pBmpInfoHdr, DIB_RGB_COLORS);
	dwScanDataSize = pBmpInfoHdr->biSizeImage;
	pScanDataBuffer = new BYTE[dwScanDataSize];
	GetDIBits(hDC, hBmp, 0, bmp.bmHeight, pScanDataBuffer, (LPBITMAPINFO)pBmpInfoHdr, DIB_RGB_COLORS);
	ReleaseDC(NULL, hDC);

	bmpFileHeader.bfType = 0x4d42;	/* "BM" */
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;

	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + dwHeaderSize;
	bmpFileHeader.bfSize = dwScanDataSize + bmpFileHeader.bfOffBits;

	hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &dw, NULL);
	WriteFile(hFile, pHeaderBuffer, dwHeaderSize, &dw, NULL);
	WriteFile(hFile, pScanDataBuffer, dwScanDataSize, &dw, NULL);
	CloseHandle(hFile);

	delete[] pScanDataBuffer;
	delete[] pHeaderBuffer;

	return TRUE;
}

/*
 * �v�����g�X�N���[���ŃX�N���[���V���b�g���擾
 */
void GetScreeShot() {
  double t = gettimeofday_sec();
  keybd_event(VK_LMENU, 0, 0, 0);
  keybd_event(VK_SNAPSHOT, 0, 0, 0);
  keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
  keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);

  while (gettimeofday_sec() - t < SCREEN_SHOT_TIME)
    ;
}

