/*
 *  myenv.c : env コマンドのクローン
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __linux__
#include "myputenv.h"
#endif

extern char **environ;

int main(int argc, char *argv[]) {
  int i;

  //putenvが失敗した時点でそれ以降はコマンドとその引数とみなす
  for (i = 1; argv[i] != NULL; i++) {
    if (putenv(argv[i]) != 0) {
      break; 
    }
  }

  //コマンドが指定されている場合→それを実行
  if (argv[i] != NULL) {
    execvp(argv[i], &argv[i]);
    perror(argv[i]);
    return 1;
  }

  //コマンドが省略された場合は環境変数一覧を表示
  for (int j = 0; environ[j] != NULL; j++) {
    printf("%s\n", environ[j]);
  }
 
  return 0;
}
/* 実行例
コンパイル結果：
% make
cc -D_GNU_SOURCE -Wall -std=c99 -o myenv myenv.c myputenv.c

コンパイルエラー無し→コンパイル成功

% ./myenv | head -5
COMMAND_MODE=unix2003
HOME=/Users/hibikit510
LOGNAME=hibikit510
MACH_PORT_RENDEZVOUS_PEER_VALDATION=0
MallocNanoZone=0

引数なしの時は環境変数一覧が表示された

% ./myenv TZ=Cuba date
2026年 6月29日 月曜日 21時42分37秒 CDT

環境変数を1つ変更してdateを実行した。→変更した環境変数に合わせた結果となった

% ./myenv LC_TIME=ja_JP.UTF-8 TZ=Cuba date
2026年 6月29日 月曜日 21時43分40秒 CDT

複数の環境変数を変更してdateを実行した。→先ほど同様、変更点に合わせて結果も変わった

% ./myenv LC_TIME=ja_JP.UTF-8 ls -l
total 384
-rw-r--r--@ 1 hibikit510  staff     132  6 30 10:01 Makefile
-rw-r--r--@ 1 hibikit510  staff    1268  6 30 10:01 README.md
-rw-r--r--@ 1 hibikit510  staff  139098  6 30 10:01 README.pdf
-rwxr-xr-x@ 1 hibikit510  staff   33736  6 30 10:40 myenv
-rw-r--r--@ 1 hibikit510  staff     566  6 30 10:40 myenv.c
-rwxr-xr-x@ 1 hibikit510  staff     216  6 30 10:01 myputenv.c
-rw-r--r--@ 1 hibikit510  staff      93  6 30 10:01 myputenv.h

引数付きコマンドを実行した。
putenv("LC_TIME=ja_JP.UTF-8")が成功した後、ls がputenvでエラーが出ることで、
ls -lがexecvpに渡され、実行されることで-lオプション付きで実行できた。
lsはロケール依存でないためLC TIMEは見た目には出ないがコマンド自体は正常に実行できた。

% ./myenv NO_SUCH_COMMAND_XYZ
NO_SUCH_COMMAND_XYZ: No such file or directory

存在しないコマンドを実行した。それによりエラーメッセージがでた。

 % ./myenv LC_TIME=ja_JP.UTF-8 TZ=Cuba ls -l
total 384
-rw-r--r--@ 1 hibikit510  staff     132  6 29 21:01 Makefile
-rw-r--r--@ 1 hibikit510  staff    1268  6 29 21:01 README.md
-rw-r--r--@ 1 hibikit510  staff  139098  6 29 21:01 README.pdf
-rwxr-xr-x@ 1 hibikit510  staff   33736  6 29 21:40 myenv
-rw-r--r--@ 1 hibikit510  staff     566  6 29 21:40 myenv.c
-rwxr-xr-x@ 1 hibikit510  staff     216  6 29 21:01 myputenv.c
-rw-r--r--@ 1 hibikit510  staff      93  6 29 21:01 myputenv.h

スライドに載っている状況のコマンドを実行した。
先ほどの% ./myenv LC_TIME=ja_JP.UTF-8 ls -lの時と同様の挙動を示し、
putenv(argv[2])までが実行され、argv[3]のls以降はexecvpにより実行された。
*/
