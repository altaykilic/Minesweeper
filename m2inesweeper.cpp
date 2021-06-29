//Altay Kılıç, 17/04/2020
#include <iostream>
#include <vector>
#include <conio.h>	//getch()
#include <ctime>	//rand()
#include <cassert>	//assert()
using namespace std;
#define nl '\n'
#define pb push_back
#define clear system("cls")

bool gameexit=0, lose, win;
int cursorx, cursory;
int gridl, gridw;
int tiles, mines, flags;
double prog, progIncrement;
short firstMoveType;
vector<vector<int> > grid;
vector<vector<int> > board;

void game();
void setGrid();
void dfs(int,int);
void display(short,bool);
void firstMove();
void menu();
bool random(int,int);

int main(){
	srand(time(0));
	// system("start https://www.youtube.com/watch?v=8QT7ITv9Ecs");
	
	mainmenu:
	system("color 07");
	menu();
	
	clear;
	
	if(gameexit)
		return 0;
	
	gridsize:
	cout << "dimensions of the grid: ";
	cin >> gridw >> gridl;
	if(gridw<2 || gridw>50 || gridl<2 || gridl>30 || gridw*gridl<=10){
		cout << "too large or too small" << nl;
		goto gridsize;
	}
	grid.resize(gridl+2);
	fill(grid.begin(), grid.end(), vector<int>(gridw+2,0));
	board.resize(gridl+2);
	fill(board.begin(), board.end(), vector<int>(gridw+2,0));
	tiles=gridl*gridw;
	
	clear;
	
	nofmines:
	cout << "number of mines: ";
	cin >> mines;
	if(mines>tiles-10 || mines<1){
		cout << "too many or too few" << nl;
		goto nofmines;
	}
	
	prog=0;
	progIncrement=100.0f/(tiles-mines);
	lose=win=0;
	
	cursorx=cursory=1;
	flags=0;
	firstMoveType=0;
	
	//The mines' locations are determined after the first move.
	firstMove();
	if(cursory==1 || cursory==gridl) firstMoveType++;
	if(cursorx==1 || cursorx==gridw) firstMoveType++;
	//firstMoveType = {0 -> middle, 1 -> edge, 2 -> corner}
	setGrid();	
	dfs(cursory, cursorx);
	
	game();
	goto mainmenu;
}

void game(){
	clear;
	display(0,1);
	char hamle='c';
	while(!win && !lose){
		invalidmove:
		hamle=getch();
		switch(hamle){
			case 'u': assert(0); //assert(condition) -> Forces runtime error if "condition" is false.
			case 'l': lose=1; break;
			case 'W': case 'w': if(cursory>1) cursory--; clear; display(0,1); break;
			case 'A': case 'a': if(cursorx>1) cursorx--; clear; display(0,1); break;
			case 'S': case 's': if(cursory<gridl) cursory++; clear; display(0,1); break;
			case 'D': case 'd': if(cursorx<gridw) cursorx++; clear; display(0,1); break;
			case 'J': case 'j': 
			if(board[cursory][cursorx]==0 || board[cursory][cursorx]==-1){
				dfs(cursory, cursorx);
				clear;
				display(0,0);
				break;
			}
			else if(board[cursory][cursorx]==1){
				int cnt=0;
				if(board[cursory-1][cursorx-1]==2) cnt++;
				if(board[cursory-1][cursorx]==2) cnt++;
				if(board[cursory-1][cursorx+1]==2) cnt++;
				if(board[cursory][cursorx-1]==2) cnt++;
				if(board[cursory][cursorx+1]==2) cnt++;
				if(board[cursory+1][cursorx-1]==2) cnt++;
				if(board[cursory+1][cursorx]==2) cnt++;
				if(board[cursory+1][cursorx+1]==2) cnt++;
				if(cnt==grid[cursory][cursorx]){
					dfs(cursory-1, cursorx-1);
					dfs(cursory-1, cursorx);
					dfs(cursory-1, cursorx+1);
					dfs(cursory, cursorx-1);
					dfs(cursory, cursorx+1);
					dfs(cursory+1, cursorx-1);
					dfs(cursory+1, cursorx);
					dfs(cursory+1, cursorx+1);
				}
				clear;
				display(0,1);
			}
			break;
			
			case 'K': case 'k': 
			if(board[cursory][cursorx]==2){
				if(grid[cursory][cursorx]<0)
					board[cursory][cursorx]=-1;
				else
					board[cursory][cursorx]=0;
				flags--;
			}
			else if(board[cursory][cursorx]!=1){
				board[cursory][cursorx]=2;
				flags++;
			}
			else
				break;
			clear;
			display(0,0);
			break;
			
			default:
			goto invalidmove;
		}
	}
	if(lose){
		clear;
		system("color c0");
		display(3,0);
		while(1){
			hamle=getch();
			if(hamle=='1'){
				clear;
				display(3,0);
			}
			else if(hamle=='2'){
				clear;
				display(0,0);
			}
			else if(hamle=='0'){
				return;
			}
		}
	}
	if(win){
		clear;
		system("color a0");
		display(0,0);
		cout << "0 -> Menu";
		while(hamle!='0')
			hamle=getch();
		return;
	}
}

void setGrid(){
	int minesRemaining=mines, tilesRemaining=tiles;
	if(firstMoveType==0) tilesRemaining-=9;
	else if(firstMoveType==1) tilesRemaining-=6;
	else tilesRemaining-=4;
	int i, j;
	for(i=1; i<gridl+1; i++){
		for(j=1; j<gridw+1; j++){
			if(i<cursory+2 && i>cursory-2 && j<cursorx+2 && j>cursorx-2){
				continue;
			}
			if(random(tilesRemaining,minesRemaining)){
				board[i][j]=-1;
				grid[i][j]=-9;
				minesRemaining--;
				grid[i-1][j-1]++;
				grid[i-1][j]++;
				grid[i-1][j+1]++;
				grid[i][j-1]++;
				grid[i][j+1]++;
				grid[i+1][j-1]++;
				grid[i+1][j]++;
				grid[i+1][j+1]++;
			}
			tilesRemaining--;
		}
	}
}

void dfs(int y, int x){
	if(y<1 || y>gridl || x<1 || x>gridw)
		return;
	if(board[y][x]==2 || board[y][x]==1)
		return;
	if(board[y][x]==-1){
		lose=1;
		return;
	}
	board[y][x]=1;
	prog+=progIncrement;
	if(prog>=99.9){ //if(prog==100) çalışmıyor???
		win=1;
		return;
	}
	if(grid[y][x]==0){
		if(board[y-1][x-1]==0) dfs(y-1, x-1);
		if(board[y-1][x]==0) dfs(y-1, x);
		if(board[y-1][x+1]==0) dfs(y-1, x+1);
		if(board[y][x-1]==0) dfs(y, x-1);
		if(board[y][x+1]==0) dfs(y, x+1);
		if(board[y+1][x-1]==0) dfs(y+1, x-1);
		if(board[y+1][x]==0) dfs(y+1, x);
		if(board[y+1][x+1]==0) dfs(y+1, x+1);
	}
}

//displays board
void display(short mode, bool showCursor){
	printf("progress: %.2lf / 100\n", prog);
	printf("mines remaining: %02d\n\n", mines-flags);
	int i, j;
	// cout << " X    ";
	// for(i=1; i<gridw+1; i++){
	// 	printf("%02d  ",i);
	// }
	// cout << nl << 'Y';
	cout << nl << nl;
	for(i=1; i<gridl+1; i++){
		// printf("%02d    ",i);
		printf("    ");
		for(j=1; j<gridw+1; j++){
			if(mode==0){		//game & lose
				if(i == cursory && j == cursorx && showCursor){
					printf("%c%c  ", 176, 176);
				}
				else if(board[i][j]==1){
					if(grid[i][j])
						printf("%2d  ", grid[i][j]);
					else
						printf("    ");
				}
				else if(board[i][j]==2){
					printf("XX  ");
				}
				else{
					printf("%c%c  ", 219, 219);
				}
			}
			else if(mode==1){	//debug
				if(i == cursory && j == cursorx){
					printf("%c%c  ", 176, 176);
				}
				else if(board[i][j]==2){
					printf("XX  ");
				}
				else if(board[i][j]==-1){
					printf("++  ");
				}
				else{
					printf("%2d  ", grid[i][j]);
				}
			}
			else if(mode==2){	//debug
				printf("%2d  ", board[i][j]);
			}
			else if(mode==3){	//lose
				if(grid[i][j]<0){
					printf("++  ");
				}
				else if(board[i][j]==1){
					if(grid[i][j])
						printf("%2d  ", grid[i][j]);
					else
						printf("    ");
				}
				else{
					printf("%c%c  ", 219, 219);
				}
			}
		}
		cout << nl << nl;
	}
	if(lose){
		cout << "0 -> Menu\n1 -> Mayinlar\n2 -> Bayraklar";
	}
}

void firstMove(){
	char hamle='?';
	while(1){
		clear;
		display(0,1);
		invalidmove:
		hamle=getch();
		switch(hamle){
			case 'u': assert(0);
			case 'W': case 'w': if(cursory>1) cursory--; break;
			case 'A': case 'a': if(cursorx>1) cursorx--; break;
			case 'S': case 's': if(cursory<gridl) cursory++; break;
			case 'D': case 'd': if(cursorx<gridw) cursorx++; break;
			case 'J': case 'j': return;
			default: goto invalidmove;
		}
		if(cursorx<1) cursorx=1;
		else if(cursorx>gridw) cursorx=gridw;
		else if(cursory<1) cursory=1;
		else if(cursory>gridl) cursory=gridl;
	}
}

void menu(){
	char choice;
	menu0:
	clear;
	cout << "1 -> Oyna\n2 -> Kurallar\n3 -> Kontroller\n4 -> Exit";
	choice=getch();
	if(choice=='1'){
		lose=0;
		win=0;
		return;
	}
	else if(choice=='2'){
		menu1:
		clear;
		cout << "Acilmis karelerdeki sayilar, o karenin komsularindan ";
		cout << "kacinin mayin oldugunu gosterir.\n";
		cout << "Mayin olan karelere bayrak koy, diger kareleri ac.\n";
		cout << "Ilk hamlende bayrak koyamazsin.\n" << nl << nl << "0 -> Geri";
		choice=getch();
		while(choice!='0')
			choice=getch();
		goto menu0;
	}
	else if(choice=='3'){
		menu2:
		clear;
		cout << "Hareket etme: W A S D\n";
		cout << "Bayrak koyma/kaldirma: K\n";
		cout << "Kutu acma: J\n" << nl;
		cout << "Lose: L\n";
		cout << "Crash: U\n" << nl << nl << "0 -> Geri";
		choice=getch();
		while(choice!='0')
			choice=getch();
		goto menu0;
		//kontrolleri değiştirme seçeneği eklemeyi düşündüm ancak bunun için
		//bütün switch-case'leri if'lerle değiştirmem gerekiyordu.
		//ona da üşendim açıkçası
	}
	else if(choice=='4'){
		gameexit=1;
		return;
	}
	else
		goto menu0;
}

bool random(int x, int y){ //Returns 1 with a random chance of y/x.
	if(rand()%x<y) return 1;
	return 0;
} 
