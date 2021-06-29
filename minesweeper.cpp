#include <iostream>
#include <vector>
#include <ctime>
#include <conio.h>
using namespace std;
#define nl '\n'
#define pb push_back
#define clear system("cls")

int gridw, gridl;
int mines, tilerem2;
vector<vector<pair<int,int> > > grid;
pair<int,int> ham;
int firstmove=0;
bool patlamadin=1, kazanmadin=1;
int bayrakno=0;

void gridset();
bool random(int,int);
void display(int); //0 normal, 1 & 2 debug, 3 lose
void oyna();
void dfs(int,int);

/*
gridin boyutu ve mayın sayısını okuduktan sonra ilk hamleyi okuyorum.
ilk hamleye göre mayınları diziyorum çünkü ilk hamlenin olduğu karede
ve onun komşusu olan karelerde mayın bulunmaması gerekiyor
*/

int main(){
	// cout << (char)-37; /*
	srand(time(0));
	clear;
	cokbuyukveyacokkucuk:
	cout << "oyun tahtasi boyutlari: ";
	cin >> gridw >> gridl;
	if(gridw>99 || gridw<2 || gridl>99 || gridl<2){
		goto cokbuyukveyacokkucuk;
	}
	tilerem2=gridw*gridl;
	clear;
	grid.resize(gridl+2, vector<pair<int,int> >(gridw+2,{0, 0}));
	tekrarmayin:
	cout << "mayin sayisi: ";
	cin >> mines;
	if(mines>gridl*gridw-9){
		cout << "cok fazla mayin" << nl;
		goto tekrarmayin;
	}
	tilerem2-=mines;
	clear;
	display(0);
	cout << nl << nl << "acilacak kare: ";
	tekrarhamle:
	cin >> ham.first >> ham.second;
	if(ham.first<1 || ham.first>gridw || ham.second<1 || ham.second>gridl){
		cout << "gridin icinden bi kare soyle" << nl;
		goto tekrarhamle;
	}
	clear;
	ham.second=gridl-ham.second+1;
	if(ham.first==1 || ham.first==gridw) firstmove++;
	if(ham.second==1 || ham.second==gridl) firstmove++;
	gridset();
	dfs(ham.second, ham.first);
	while(patlamadin && kazanmadin){ //asıl döngü
		oyna();
	}
/**/
}

void oyna(){
	tekrarread:
	clear;
	display(0);
	cout << nl << nl << "0 -> bayrak\n1 -> acma" << nl << nl;
	int in1, in2;
	char in3=getch();
	if(in3=='0'){
		clear;
		display(0);
		cout << nl << nl << "bayrak konumu: ";
		cin >> in1 >> in2;
		if(ham.first<1 || ham.first>gridw || ham.second<1 || ham.second>gridl){
			goto tekrarread;
		}
		in2=gridl-in2+1;
		if(grid[in2][in1].second==1)
			goto tekrarread;
		if(grid[in2][in1].second==2){
			if(grid[in2][in1].first<0)
				grid[in2][in1].second=-1;
			else
				grid[in2][in1].second=0;
			bayrakno--;
		}
		else{
			grid[in2][in1].second=2;
			bayrakno++;
		}
	}
	else if(in3=='1'){
		clear;
		display(0);
		cout << nl << nl << "acilacak kare: ";
		cin >> in1 >> in2;
		if(ham.first<1 || ham.first>gridw || ham.second<1 || ham.second>gridl){
			goto tekrarread;
		}
		in2=gridl-in2+1;
		if(grid[in2][in1].second==1){
			int cnt=grid[in2][in1].first;
			if(grid[in2-1][in1-1].second==2) cnt--;
			if(grid[in2-1][in1].second==2) cnt--;
			if(grid[in2-1][in1+1].second==2) cnt--;
			if(grid[in2][in1-1].second==2) cnt--;
			if(grid[in2][in1+1].second==2) cnt--;
			if(grid[in2+1][in1-1].second==2) cnt--;
			if(grid[in2+1][in1].second==2) cnt--;
			if(grid[in2+1][in1+1].second==2) cnt--;
			if(!cnt){
				dfs(in2-1,in1-1);
				dfs(in2-1,in1);
				dfs(in2-1,in1+1);
				dfs(in2,in1-1);
				dfs(in2,in1+1);
				dfs(in2+1,in1-1);
				dfs(in2+1,in1);
				dfs(in2+1,in1+1);
			}
		}
		else if(grid[in2][in1].second==2)
			goto tekrarread;
		else
			dfs(in2,in1);
	}
	else
		goto tekrarread;
}

void dfs(int x, int y){ //depth first search bfs breadth first search
	// cout << "dfs " << x << ' ' << y << ' ';
	if(x==0 || x==gridl+1 || y==0 || y==gridw+1){
		// cout << "returned" << nl;
		return;
	}
	if(grid[x][y].second==2)
		return;
	// cout << "continued" << nl;
	if(grid[x][y].second==-1){
		patlamadin=0;
		clear;
		system("color 40");
		display(3);
		cout << nl << nl << "PATLADIN" << nl << nl;
		system("pause");
		return;
	}
	if(grid[x][y].second==0)
		tilerem2--;
	grid[x][y].second=1;
	if(!tilerem2){
		kazanmadin=0;
		clear;
		display(0);
		cout << nl << nl << "TEBRIKLER" << nl << nl;
		system("pause");
		return;
	}
	if(grid[x][y].first==0){
		if(grid[x-1][y-1].second==0) dfs(x-1, y-1);
		if(grid[x-1][y].second==0) dfs(x-1, y);
		if(grid[x-1][y+1].second==0) dfs(x-1, y+1);
		if(grid[x][y-1].second==0) dfs(x, y-1);
		if(grid[x][y+1].second==0) dfs(x, y+1);
		if(grid[x+1][y-1].second==0) dfs(x+1, y-1);
		if(grid[x+1][y].second==0) dfs(x+1, y);
		if(grid[x+1][y+1].second==0) dfs(x+1, y+1);
	}
}

void gridset(){
	int minerem=mines, tilerem=gridw*gridl;
	if(firstmove==1) tilerem-=6;
	else if(firstmove==2) tilerem-=4;
	else tilerem-=9;
	for(int i=1; i<gridl+1; i++){
		for(int j=1; j<gridw+1; j++){
			if(j>ham.first-2 && j<ham.first+2 && i>ham.second-2 && i<ham.second+2){
				continue;
			}
			if(random(tilerem, minerem)){
				grid[i][j].first=-9;
				grid[i][j].second=-1;
				minerem--;
				grid[i-1][j-1].first++;
				grid[i-1][j].first++;
				grid[i-1][j+1].first++;
				grid[i][j-1].first++;
				grid[i][j+1].first++;
				grid[i+1][j-1].first++;
				grid[i+1][j].first++;
				grid[i+1][j+1].first++;
			}
			tilerem--;
		}
	}
}

bool random(int x, int y){ // 1 dönme olasılığı y/x
	int a=rand()%x;
	if(a<y) return 1;
	return 0;
}

void display(int admin){
	int i, j;
	cout << " X    ";
	for(i=1; i<gridw+1; i++){
		printf("%02d  ", i);
	}
	cout << nl << 'Y' << nl << nl;
	for(i=1; i<gridl+1; i++){
		printf("%02d    ", gridl-i+1);
		for(j=1; j<gridw+1; j++){
			if(admin==2){
				printf("%2d  ", grid[i][j].second);
			}
			else if(admin==1){
				if(grid[i][j].second==-1){
					printf("++  ");
				}
				else if(grid[i][j].second==2){
					printf("XX  ");
				}
				else{
					printf("%2d  ", grid[i][j].first);
				}
			}
			else if(admin==0){
				if(grid[i][j].second==1){
					if(grid[i][j].first)
						printf("%2d  ", grid[i][j].first);
					else
						printf("    ");
				}
				else if(grid[i][j].second==2){
					printf("XX  ");
				}
				else{
					printf("%c%c  ", -37, -37);
				}
			}
			else if(admin==3){
				if(grid[i][j].second==1){
					if(grid[i][j].first)
						printf("%2d  ", grid[i][j].first);
					else
						printf("    ");
				}
				else if(grid[i][j].second==2){
					printf("XX  ");
				}
				else if(grid[i][j].second==-1){
					printf("++  ");
				}
				else if(grid[i][j].second==0){
					printf("%c%c  ", -37, -37);
				}
			}
			
		}
		printf("    %02d", gridl-i+1);
		cout << nl << nl;
	}
	cout << nl << "      ";
	for(i=1; i<gridw+1; i++){
		printf("%02d  ", i);
	}
	if(patlamadin && kazanmadin)
		cout << nl << nl << "kalan mayin: " << mines-bayrakno;
}


/*
//eski display loopu
			if(admin==2){
				printf("%2d  ", grid[i][j].second);
			}
			else if(admin==1 && grid[i][j].second==-1){
				printf("++  ");
			}
			else if(admin==1 && grid[i][j].second==0){
				printf("%2d  ", grid[i][j].first);
			}
			else if(grid[i][j].second==1){
				if(grid[i][j].first) printf("%2d  ", grid[i][j].first);
				else printf("    ");
			}
			else if(grid[i][j].second==2){
				printf("[]    ");
			}
			else{
				printf("%c%c  ", -37, -37);
			}/**/
