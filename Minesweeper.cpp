#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"

int board[30][24];		//地雷面板
int open[30][24];		//格子是否開過
						//0=未開 1=剛開(強制顯示) 2=開過(不強制顯示)
int mark[30][24];		//格子是否標記
						//-1=剛取消(強制顯示) 0=取消(不強制顯示) 1=剛標記(強制顯示) 2=顯示(不強制顯示)
int w;					//寬
int h;					//高
int b;					//炸彈數
int count;				//計數器
int mx;					//滑鼠X座標
int my;					//滑鼠y座標
int marknum;			//目前標記數
int gameover;			//是否Gameover	1=輸	2=贏
char ch;				//取得按鍵

void resetzero(){				//變數歸0
	ch=0;
	gameover=0;
	count=0;
	mx=0;
	my=0;
	marknum=0;
	int i,j;
	for(j=0;j<24;j++){
		for(i=0;i<30;i++){
			board[i][j]=0;
			open[i][j]=0;
			mark[i][j]=0;
		}
	}
}

int glevel(){				//主選單
	printf("***************Minesweeper v1.2 Made by World***************\n");
	ch='0';
	printf("Choose Game Level (1)Easy (2)Normal (3)Hard (4)Other (5)Quit:");
	while(!(ch=='1' || ch=='2' || ch=='3' || ch=='4' || ch=='5'))
		ch = getch();
	printf("%c\n", ch);
	switch(ch){
	case '1':
		return 1;break;
	case '2':
		return 2;break;
	case '3':
		return 3;break;
	case '4':
		return 4;break;
	default:
		return 5;break;
	}
}

void otlevel(){					//自訂大小&給定初始值
	printf("Width(9~30):");
	scanf("%d", &w);
	if(w<9)
		w=9;
	else if(w>30)
		w=30;
	printf("Height(9~24):");
	scanf("%d", &h);
	if(h<9)
		h=9;
	else if(h>24)
		h=24;
	printf("Number of Mines:(10~%d)", (w*h)-(w+h-1));
	scanf("%d", &b);
	if(b<10)
		b = 10;
	else if(b > (w*h))
		b = (w*h)-(w+h-1);			//炸彈max=場地大小-(長+寬-1)
}

void initial(int level){			//給定初始值(預設難度)
	switch(level){
	case 1:
		w=9;h=9;b=10;break;
	case 2:
		w=16;h=16;b=40;break;
	case 3:
		w=30;h=16;b=99;break;
	}
}

char* display(int i,int j){			//要顯示的字元
	if(gameover==1 && (mark[i][j]==1||mark[i][j]==2) && board[i][j] != -1)	//Gameover，顯示標錯者
			return "╳";
	switch(board[i][j]){
	case -1:
		if(gameover != 0 && (mark[i][j]==1||mark[i][j]==2))			//遊戲結束時(輸or贏)有標記正確
			return "★";
		else if(gameover == 2)										//遊戲結束時(贏)還沒標記者自動標記
			return "★";
		else
			return "◎";											//遊戲結束時(輸)未標記的地雷
	case 0:
		return "　";break;
	case 1:
		return "１";break;
	case 2:
		return "２";break;
	case 3:
		return "３";break;
	case 4:
		return "４";break;
	case 5:
		return "５";break;
	case 6:
		return "６";break;
	case 7:
		return "７";break;
	case 8:
		return "８";break;
	default:
		return "　";break;
	}
}

void ptboard(){			//印出面板
	int i,j;
	if(gameover==0){
		for(j=0;j<h;j++){
			for(i=0;i<w;i++){
				if(i==mx && j==my && open[i][j] !=1 && (mark[i][j] ==0||mark[i][j] ==2))						//游標or已開or已標記
					printf("□");
				else if(i==mx && j==my && mark[i][j]==-1)									//剛取消標記
					printf("■");
				else if(mark[i][j] !=0)														//剛標記
					printf("★");
				else if(open[i][j]==0)
					printf("■");
				else
					printf("%s",display(i,j));
			}
			printf("\n");
		}
		printf("Mines:%3d		(－＿－)",b-marknum);
	}else{
		for(j=0;j<h;j++){
			for(i=0;i<w;i++){
				printf("%s",display(i,j));
			}
			printf("\n");
		}
		if(gameover==1){
			printf("Mines:%3d		(￣□￣)	***YOU LOSE!!***\n",b-marknum);
		}
		if(gameover==2){
			printf("Mines:%3d		(￣︶￣)	***YOU WIN!!***\n",0);
		}
		printf("Press Enter button to continue...");
		while(true){
			ch=getch();
			if(ch==13)
				break;
		}
	}
}

int getkey(){			//取得按鍵
	while(true){
		ch = getch();
		if (ch == '\xe0'|| ch == '\0')
		{
			ch = getch(); 
			switch(ch){
			case 72:	//↑
				return 0;break;
			case 80:	//↓
				return 1;break;
			case 75:	//←
				return 2;break;
			case 77:	//→
				return 3;break;
			}
		}
		else{
			switch(ch){
			case 32:	//Space
				return 4;break;
			case 'x': case 'X':
				return 5;break;
			case 'z': case 'Z':
				return 6;break;
			}
		}
	}
	return 0;
}

void move(int key){			//移動游標
	if(open[mx][my]==1){								//移動後，剛開的格子不再強制顯示
		open[mx][my]++;
	}													//移動後，剛開的格子不再強制顯示
	if(mark[mx][my]==1 || mark[mx][my]==-1){			//移動後，剛標記的格子不再強制顯示
		mark[mx][my]++;
	}													//移動後，剛標記的格子不再強制顯示
	switch(key){
	case 0:	//↑
		if(my==0) {my=(h-1);break;}
		else {my--;break;}
	case 1:	//↓
		if(my==(h-1)) {my=0;break;}
		else {my++;break;}
	case 2:	//←
		if(mx==0) {mx=(w-1);break;}
		else {mx--;break;}
	case 3:	//→
		if(mx==(w-1)) {mx=0;break;}
		else {mx++;break;}
	}
}

void test(){			//測試用，先印出答案
	clrscr();
	int i,j;
	for(j=0;j<h;j++){
		for(i=0;i<w;i++){
			printf("%2d",board[i][j]);
		}
		printf("\n");
	}
	ch=getch();
}

void markb(int nx,int ny){		//標記
	if(open[nx][ny] == 0 && (mark[nx][ny] ==-1||mark[nx][ny] ==0)){
		marknum++;
		mark[nx][ny]=1;		//標記
	}
	else if(open[nx][ny] == 0 && mark[nx][ny] != 0){
		marknum--;
		mark[nx][ny]=-1;		//取消標記
	}
}

void random(){				//設定炸彈位置
	int c1=0,i,j,k,l;
	while(c1<b){
		i = rand()%w;
		j = rand()%h;
		if(board[i][j] != -1 && !(mx == i && my == j)){
			board[i][j] = -1;
			c1++;
		}
	}
	//算出其他格顯示的數字
	c1=0;
	for(j=0;j<h;j++){
		for(i=0;i<w;i++){							//每個陣列元素都要算到
			for(l=j-1;l<=j+1;l++){					//1
				for(k=i-1;k<=i+1;k++){
					if(k>=0 && k<w && l>=0 && l<h && !(k==i && l==j)){
						if(board[k][l] == -1)
							c1++;
					}
				}
			}
		if(board[i][j] != -1)		//非地雷的格子才做改變
		board[i][j]=c1;				//計算結果存到board中
		c1=0;
		}											//1
	}
	count++;						//不再執行此函式
	//TEST
	//test();
	//
	
}

void click2(int nx,int ny){			//直接開週圍八格
	int k,l;
	for(l=ny-1;l<=ny+1;l++){
		for(k=nx-1;k<=nx+1;k++){
			if(k>=0 && k<w && l>=0 && l<h && !(k==nx && l==ny) && open[k][l]==0 && (mark[k][l] ==-1||mark[k][l]==0)){
				open[k][l]=2;
			if(board[k][l]==0)
				click2(k,l);

			}
		}
	}
}

void click(int nx,int ny,int c2){			//單鍵開啟
	if(mark[mx][my]==-1){								//移動後，剛標記的格子不再強制顯示
		mark[mx][my]++;
	}													//移動後，剛標記的格子不再強制顯示
	if(open[nx][ny] == 0 && (mark[nx][ny] ==-1||mark[nx][ny] ==0)){
		if(count==0)
			random();
		if(c2==0)				//直接開啟某格
			open[nx][ny]=1;		//剛開
		else					//掃雷開啟
			open[nx][ny]=2;		//已開
	}
	if(board[nx][ny]==0)			//自動展開周圍八格
		click2(nx,ny);
}

void extend(int nx,int ny){			//掃雷
	int k,l,c2=0;
	if(open[nx][ny]>0 && board[nx][ny] !=0){							//在已開、非0的格上才能用掃雷
		for(l=ny-1;l<=ny+1;l++){
			for(k=nx-1;k<=nx+1;k++){
				if(k>=0 && k<w && l>=0 && l<h && !(k==nx && l==ny) && (mark[k][l] ==2||mark[k][l]==3))
					c2++;
			}
		}
		if(c2==board[nx][ny]){											//標記數目=格子的數才能掃雷
			for(l=ny-1;l<=ny+1;l++){
				for(k=nx-1;k<=nx+1;k++){
					if(k>=0 && k<w && l>=0 && l<h && !(k==nx && l==ny) && open[k][l]==0 && (mark[k][l] ==-1||mark[k][l]==0))
						click(k,l,c2);
				}
			}
		}
	}
}

void gamecheck(){						//判斷是否Gameover
										//失敗條件：踩到地雷
										//勝利條件：剩下的格子都是地雷
	int i,j,c3=0,c4=0;
	for(j=0;j<h;j++){
		for(i=0;i<w;i++){
			if(board[i][j]==-1 && open[i][j]==0)
				c3++;
			if(open[i][j]>0)
				c4++;
		}
	}
	if(c3==b && c4==(w*h)-b)
		gameover=2;

	for(j=0;j<h;j++){
		for(i=0;i<w;i++){
			if(board[i][j]==-1 && open[i][j]>0)
				gameover=1;
		}
	}
	
}



void main(){
	srand((unsigned int)time(NULL));
	int level,key;
	while(true){
		clrscr();
		resetzero();
		level = glevel();
		if(level==5)
			break;
		if(level==4)
			otlevel();
		else
			initial(level);
		clrscr();
		while(true){
			gotoxy(0, 0);
			ptboard();
			if(gameover==0){
				key = getkey();
				switch(key){
				case 0: case 1: case 2: case 3:
					move(key);break;
				case 4:
					click(mx,my,0);break;
				case 5:
					markb(mx,my);break;
				case 6:
					extend(mx,my);break;
				}
				gamecheck();
			}else
				break;
		}
	}
}