#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"

int board[30][24];		//�a�p���O
int open[30][24];		//��l�O�_�}�L
						//0=���} 1=��}(�j�����) 2=�}�L(���j�����)
int mark[30][24];		//��l�O�_�аO
						//-1=�����(�j�����) 0=����(���j�����) 1=��аO(�j�����) 2=���(���j�����)
int w;					//�e
int h;					//��
int b;					//���u��
int count;				//�p�ƾ�
int mx;					//�ƹ�X�y��
int my;					//�ƹ�y�y��
int marknum;			//�ثe�аO��
int gameover;			//�O�_Gameover	1=��	2=Ĺ
char ch;				//���o����

void resetzero(){				//�ܼ��k0
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

int glevel(){				//�D���
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

void otlevel(){					//�ۭq�j�p&���w��l��
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
		b = (w*h)-(w+h-1);			//���umax=���a�j�p-(��+�e-1)
}

void initial(int level){			//���w��l��(�w�]����)
	switch(level){
	case 1:
		w=9;h=9;b=10;break;
	case 2:
		w=16;h=16;b=40;break;
	case 3:
		w=30;h=16;b=99;break;
	}
}

char* display(int i,int j){			//�n��ܪ��r��
	if(gameover==1 && (mark[i][j]==1||mark[i][j]==2) && board[i][j] != -1)	//Gameover�A��ܼп���
			return "��";
	switch(board[i][j]){
	case -1:
		if(gameover != 0 && (mark[i][j]==1||mark[i][j]==2))			//�C��������(��orĹ)���аO���T
			return "��";
		else if(gameover == 2)										//�C��������(Ĺ)�٨S�аO�̦۰ʼаO
			return "��";
		else
			return "��";											//�C��������(��)���аO���a�p
	case 0:
		return "�@";break;
	case 1:
		return "��";break;
	case 2:
		return "��";break;
	case 3:
		return "��";break;
	case 4:
		return "��";break;
	case 5:
		return "��";break;
	case 6:
		return "��";break;
	case 7:
		return "��";break;
	case 8:
		return "��";break;
	default:
		return "�@";break;
	}
}

void ptboard(){			//�L�X���O
	int i,j;
	if(gameover==0){
		for(j=0;j<h;j++){
			for(i=0;i<w;i++){
				if(i==mx && j==my && open[i][j] !=1 && (mark[i][j] ==0||mark[i][j] ==2))						//���or�w�}or�w�аO
					printf("��");
				else if(i==mx && j==my && mark[i][j]==-1)									//������аO
					printf("��");
				else if(mark[i][j] !=0)														//��аO
					printf("��");
				else if(open[i][j]==0)
					printf("��");
				else
					printf("%s",display(i,j));
			}
			printf("\n");
		}
		printf("Mines:%3d		(�Сġ�)",b-marknum);
	}else{
		for(j=0;j<h;j++){
			for(i=0;i<w;i++){
				printf("%s",display(i,j));
			}
			printf("\n");
		}
		if(gameover==1){
			printf("Mines:%3d		(�á���)	***YOU LOSE!!***\n",b-marknum);
		}
		if(gameover==2){
			printf("Mines:%3d		(�á`��)	***YOU WIN!!***\n",0);
		}
		printf("Press Enter button to continue...");
		while(true){
			ch=getch();
			if(ch==13)
				break;
		}
	}
}

int getkey(){			//���o����
	while(true){
		ch = getch();
		if (ch == '\xe0'|| ch == '\0')
		{
			ch = getch(); 
			switch(ch){
			case 72:	//��
				return 0;break;
			case 80:	//��
				return 1;break;
			case 75:	//��
				return 2;break;
			case 77:	//��
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

void move(int key){			//���ʴ��
	if(open[mx][my]==1){								//���ʫ�A��}����l���A�j�����
		open[mx][my]++;
	}													//���ʫ�A��}����l���A�j�����
	if(mark[mx][my]==1 || mark[mx][my]==-1){			//���ʫ�A��аO����l���A�j�����
		mark[mx][my]++;
	}													//���ʫ�A��аO����l���A�j�����
	switch(key){
	case 0:	//��
		if(my==0) {my=(h-1);break;}
		else {my--;break;}
	case 1:	//��
		if(my==(h-1)) {my=0;break;}
		else {my++;break;}
	case 2:	//��
		if(mx==0) {mx=(w-1);break;}
		else {mx--;break;}
	case 3:	//��
		if(mx==(w-1)) {mx=0;break;}
		else {mx++;break;}
	}
}

void test(){			//���եΡA���L�X����
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

void markb(int nx,int ny){		//�аO
	if(open[nx][ny] == 0 && (mark[nx][ny] ==-1||mark[nx][ny] ==0)){
		marknum++;
		mark[nx][ny]=1;		//�аO
	}
	else if(open[nx][ny] == 0 && mark[nx][ny] != 0){
		marknum--;
		mark[nx][ny]=-1;		//�����аO
	}
}

void random(){				//�]�w���u��m
	int c1=0,i,j,k,l;
	while(c1<b){
		i = rand()%w;
		j = rand()%h;
		if(board[i][j] != -1 && !(mx == i && my == j)){
			board[i][j] = -1;
			c1++;
		}
	}
	//��X��L����ܪ��Ʀr
	c1=0;
	for(j=0;j<h;j++){
		for(i=0;i<w;i++){							//�C�Ӱ}�C�������n���
			for(l=j-1;l<=j+1;l++){					//1
				for(k=i-1;k<=i+1;k++){
					if(k>=0 && k<w && l>=0 && l<h && !(k==i && l==j)){
						if(board[k][l] == -1)
							c1++;
					}
				}
			}
		if(board[i][j] != -1)		//�D�a�p����l�~������
		board[i][j]=c1;				//�p�⵲�G�s��board��
		c1=0;
		}											//1
	}
	count++;						//���A���榹�禡
	//TEST
	//test();
	//
	
}

void click2(int nx,int ny){			//�����}�g��K��
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

void click(int nx,int ny,int c2){			//����}��
	if(mark[mx][my]==-1){								//���ʫ�A��аO����l���A�j�����
		mark[mx][my]++;
	}													//���ʫ�A��аO����l���A�j�����
	if(open[nx][ny] == 0 && (mark[nx][ny] ==-1||mark[nx][ny] ==0)){
		if(count==0)
			random();
		if(c2==0)				//�����}�ҬY��
			open[nx][ny]=1;		//��}
		else					//���p�}��
			open[nx][ny]=2;		//�w�}
	}
	if(board[nx][ny]==0)			//�۰ʮi�}�P��K��
		click2(nx,ny);
}

void extend(int nx,int ny){			//���p
	int k,l,c2=0;
	if(open[nx][ny]>0 && board[nx][ny] !=0){							//�b�w�}�B�D0����W�~��α��p
		for(l=ny-1;l<=ny+1;l++){
			for(k=nx-1;k<=nx+1;k++){
				if(k>=0 && k<w && l>=0 && l<h && !(k==nx && l==ny) && (mark[k][l] ==2||mark[k][l]==3))
					c2++;
			}
		}
		if(c2==board[nx][ny]){											//�аO�ƥ�=��l���Ƥ~�౽�p
			for(l=ny-1;l<=ny+1;l++){
				for(k=nx-1;k<=nx+1;k++){
					if(k>=0 && k<w && l>=0 && l<h && !(k==nx && l==ny) && open[k][l]==0 && (mark[k][l] ==-1||mark[k][l]==0))
						click(k,l,c2);
				}
			}
		}
	}
}

void gamecheck(){						//�P�_�O�_Gameover
										//���ѱ���G���a�p
										//�ӧQ����G�ѤU����l���O�a�p
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