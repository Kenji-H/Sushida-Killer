// 定数の場所考える（sushida依存の値が汎用ライブラリにあるため）
// 時間追加機能

#include "main.hpp"
#include "bmp.hpp"
#include "util.hpp"
#include "win.hpp"

/*
 * メイン処理
 */
int main(int argc, char *argv[]) {
  int mode = -1;

  for (int i = 0; i < argc; i++) {
    if (!strcmp("-learn", argv[i]))
      mode = LEARN_MODE;
    else if (!strcmp("-extract", argv[i]))
      mode = EXTRACT_MODE;
    else if (!strcmp("-extract-app", argv[i]))
      mode = EXTRACT_MODE_APPEND;
    else if (!strcmp("-play", argv[i]))
      mode = PLAY_MODE;
  }
  
  switch (mode) {
  case LEARN_MODE: 
    learnData();
    break;
  case EXTRACT_MODE:
    extractData();
    break;
  case EXTRACT_MODE_APPEND:
    extractDataAppend();
    break;
  case PLAY_MODE:
    playGame();
    break;
  default:
    printf("コマンドラインのオプションが不正です。\n");
    return -1;
  }
  
  return 0;
}

/* 
 * データ学習モード
 */
void learnData() {
  printf("データ学習を開始しします。\n");

  HWND app = getHandleOfSushida();
  activateWnd(app);
  putSpaceKey();
  double start_t = gettimeofday_sec();
  
  int cnt = 1;
  while (gettimeofday_sec() - start_t < PLAY_TIME) {
    if ((gettimeofday_sec() - start_t) > cnt * SHOT_SPAN) {
      ++cnt;
      GetScreeShot();
      HBITMAP bmp = DispBitMap();
      char filename[256];
      sprintf(filename, "data/_%s.bmp", getDate().c_str());
      printf("Saving to file %s\n", filename);
      sleep(SCREEN_SHOT_TIME);
      SaveBitmapFile(bmp, filename, 0);
    }
  }
}

/*
 * 特徴量抽出モード
 */
void extractData() {
  printf("学習用データから特徴量を抽出します。\n");
  
  FILE *fp;
  fp = fopen(HASH_DATA_FILE, "w");
  

  HANDLE hFind;
  WIN32_FIND_DATA win32fd;

  hFind = FindFirstFile("data/*.bmp", &win32fd);

  if (hFind == INVALID_HANDLE_VALUE) {
    printf("検索ハンドルの取得に失敗しました。\n");
    return;
  }
  
  int cnt = 0;
  set<long long int> v;
  vector<long long int> csns;
  do {
    string filename = win32fd.cFileName;
    printf("%d個目のファイル（%s）を処理中....\n", ++cnt, filename.c_str());

    string text = filename.substr(0, filename.length()-4);
    
    long long int val = bmpHashValue("data/" + filename);
    if (v.count(val))
      csns.push_back(val);
    v.insert(val);
    fprintf(fp, "%I64d %s\n", val, text.c_str());
  } while (FindNextFile(hFind, &win32fd));
  
  if (csns.size()) {
    printf("以下のハッシュ値で衝突が発生しました。\n");
    for (int i = 0; i < csns.size(); i++)
      printf("%I64d\n", csns[i]);
  }
  fclose(fp);
}


/*
 * 特徴量抽出（差分追加）モード
 */
void extractDataAppend () {
  printf("学習用データから特徴量を抽出します。（差分のみ）\n");
  
  // 既存データの取り出し
  set<string> registered;
  FILE *fp;
  char fname[256];
  long long int val;

  fp = fopen(HASH_DATA_FILE, "r");
  while (~fscanf(fp, "%I64d %s", &val, fname))
    registered.insert(fname);
  fclose(fp);

  // 追加情報の書き出し
  fp = fopen(HASH_DATA_FILE, "a");
  
  HANDLE hFind;
  WIN32_FIND_DATA win32fd;

  hFind = FindFirstFile("data/*.bmp", &win32fd);

  if (hFind == INVALID_HANDLE_VALUE) {
    printf("検索ハンドルの取得に失敗しました。\n");
    return;
  }
  
  int cnt = 0;
  set<long long int> v;
  vector<long long int> csns;
  do {
    string filename = win32fd.cFileName;
    string text = filename.substr(0, filename.length()-4);
    if (registered.count(text))
      continue;

    printf("%d個目のファイル（%s）を処理中....\n", ++cnt, filename.c_str());
    long long int val = bmpHashValue("data/" + filename);
    if (v.count(val))
      csns.push_back(val);
    v.insert(val);
    fprintf(fp, "%I64d %s\n", val, text.c_str());
  } while (FindNextFile(hFind, &win32fd));
  
  printf("%d件のデータを新規追加しました。\n", cnt);
  if (csns.size()) {
    printf("以下のハッシュ値で衝突が発生しました。\n");
    for (int i = 0; i < csns.size(); i++)
      printf("%I64d\n", csns[i]);
  }
  fclose(fp);
}


/*
 * プレイモード
 */
void playGame() {
  printf("寿司打をプレイします\n");
 
  // ハッシュデータのロード
  long long int val;
  char key[256];
  map<long long int, string> hash;
  FILE *fp = fopen(HASH_DATA_FILE, "r");
  while (~fscanf(fp, "%I64d %s", &val, key)) {
    string text = key;
    text = replaceString(text, '_', '?');
    text = removeString(text, '@');

    hash[val] = text;
  }
  fclose(fp);
  
  sleep(10);       // 開始まで待つ
  HWND app = getHandleOfSushida();
  activateWnd(app);
  putSpaceKey();
  double start_t = gettimeofday_sec();
  
  while (gettimeofday_sec() - start_t < PLAY_TIME) {
    GetScreeShot();
    sleep(SCREEN_SHOT_TIME);
    HBITMAP bmp = DispBitMap();
    
    SaveBitmapFile(bmp, TEMP_FILE, 0);
    val = bmpHashValue(TEMP_FILE);
    
    if (hash.count(val)) {
      string text = "";
      
      // 適当なエラーを付加
      for (int i = 0; i < hash[val].length(); i++) {
	if (1.0 * rand() / RAND_MAX < TYPING_ERR_RATE)
	  text += (char)('a' + rand() % 26);
	text += hash[val][i];
      }

      putString(text);
    }
    else {
      char filename[256];
      sprintf(filename, "data/!%I64d.bmp", val);
      rename(TEMP_FILE, filename);
    }  
    
    sleep(KEY_INPUT_TIME);
  }
  remove(TEMP_FILE);
}

/*
 * 寿司打のウィンドウハンドルを取得
 */
HWND getHandleOfSushida() {
  // 寿司打のウィンドウハンドルを取得
  appWndConfig conf;
  conf.hWnd = NULL;
  conf.layerNum = 5;
  strcpy(conf.className[0], "Chrome_WidgetWin_0");
  strcpy(conf.className[1], "Chrome_WidgetWin_0");
  strcpy(conf.className[2], "Chrome_RenderWidgetHostHWND");
  strcpy(conf.className[3], "WrapperNativeWindowClass");
  strcpy(conf.className[4], "NativeWindowClass");
  EnumWindows(EnumWndGetApp, (LPARAM)&conf);
  if (conf.hWnd == NULL)
    printf("寿司打のウィンドウハンドルの取得に失敗しました。\n");
  
  return conf.hWnd;
}

/*
 * ビットマップファイルのハッシュ値を計算
 */
long long int bmpHashValue(string filename) {
  BitMapProcessor bmp;
  
  bmp.loadData(filename);

  long long int ret = 0;
  long long int MOD = 1000000007;
  for (int i = R0; i < R1; i++) {
    for (int j = C0; j < C1; j++) {
      int r = bmp.getColor(i, j).r;
      int g = bmp.getColor(i, j).g;
      int b = bmp.getColor(i, j).b;

      if (r*r + g*g + b*b > 255*255*3*0.8)
	ret = (23 * ret + i * j) % MOD;
    }  
  }

  return ret;
}
