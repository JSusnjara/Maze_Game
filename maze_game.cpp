#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

int mazeX, mazeY;
int** board;
int** auxBoard;
int x = 0, y;
int steps = 0;

void sp(int rgbColor){ //set text color
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rgbColor);
}

void s(){ //set color back to normal
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void clearScreen(){
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void printPlayer(){
	steps++;
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	sp(711);
	cout<<" ";
	s();
}

void printBestRoute(){
	COORD position;
	for(int i = 0; i < mazeY; i++){
		for(int j = 0; j < mazeX; j++){
			if(auxBoard[i][j] == 2){
				position.X = j;
				position.Y = i;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
				sp(160);
				cout<<" ";
				s();
			}
		}
	}
}

void deletePlayer(int x, int y){
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	sp(250);
	cout<<" ";
	s();
}

void printSpace(int color){
	s();
	cout<<" ";
	sp(color);
}

void generateMaze(){
	int start = rand() % (mazeX - 2) + 1;
	x = start;
	for(int i = 0; i < mazeY; i++){
		for(int j = 0; j < mazeX; j++){
			if(i == 0){
				board[0][j] = (j == start) ? 0 : 1;
			}
			else{
				if(j == 0 || j == mazeX - 1){
					board[i][j] = 1;
				}
				else{
					int nearby = 0;
					nearby += board[i-1][j] ? 0 : 1;
					nearby += board[i][j-1] ? 0 : 1;
					if(i == 1 && board[i-1][j] == 0)
						board[i][j] == 0;
					else
						board[i][j] = (rand() % 9 + nearby * 2) > 5 ? 0 : 1;
				}
			}
		}
	}
	for(int i = 0; i < mazeY; i++){
		for(int j = 0; j < mazeX; j++){
			auxBoard[i][j] = board[i][j];
		}
	}
}

void printMaze(){
	steps = -1;
	
	for(int i = 0; i < mazeY; i++){
		for(int j = 0; j < mazeX; j++){
			if(board[i][j] == 1){
				sp(750);
				cout<<" ";
				s();
			}
			else{
				cout<<" ";
				//cout<<(auxBoard[i][j] * (-1)) % 10;
			}
		}
		cout<<endl;
	}
}

bool checkIfSolvable(){
	for(int i = 1; i < mazeX; i++){
		if(auxBoard[0][i] == 0)
			auxBoard[0][i] = -1;
	}
	bool changed;
	do{
		changed = false;
		for(int i = 0; i < mazeY; i++){
			for(int j = 0; j < mazeX; j++){
				if(i < mazeY - 1 && auxBoard[i][j] <= -1 && auxBoard[i+1][j] == 0){
					auxBoard[i+1][j] = auxBoard[i][j] - 1;
					changed = true;
				}
				if(j > 1 && auxBoard[i][j] <= -1 && auxBoard[i][j-1] == 0){
					auxBoard[i][j-1] = auxBoard[i][j] - 1;
					changed = true;
				}
				if(j < mazeX - 2 && auxBoard[i][j] <= -1 && auxBoard[i][j+1] == 0){
					auxBoard[i][j+1] = auxBoard[i][j] - 1;
					changed = true;
				}
				if(i > 1 && auxBoard[i][j] == -1 && auxBoard[i-1][j] <= 0){
					auxBoard[i-1][j] = auxBoard[i][j] - 1;
					changed = true;
				}
			}
		}
		
	}while(changed);
	
	for(int i = 1; i < mazeX; i++){
		if(auxBoard[mazeY - 1][i] <= -1)
			return true;
	}
	return false;
}

void checkIfVictory(){
	int stepsNecessary = -1000000;
	for(int i = 1; i < mazeX; i++){
		if(auxBoard[mazeY-1][i] < 0 && auxBoard[mazeY-1][i] > stepsNecessary)
			stepsNecessary = auxBoard[mazeY-1][i];
	}
	
	int number = stepsNecessary;
	stepsNecessary *= -1;
	stepsNecessary--;
	int x, y;
	for(int i = 1; i < mazeX; i++){
		if(auxBoard[mazeY-1][i] == number){
			auxBoard[mazeY-1][i] = 2;
			x = i;
			y = mazeY - 1;
			while(1){
				if(number == -1){
					break;
				}
				
				if(auxBoard[y-1][x] == number + 1){
					auxBoard[y-1][x] = 2;
					number++;
					y--;
				}
				else if(x > 1 && auxBoard[y][x-1] == number + 1){
					auxBoard[y][x-1] = 2;
					number++;
					x--;
				}
				else if(x < mazeX - 2 && auxBoard[y][x+1] == number + 1){
					auxBoard[y][x+1] = 2;
					number++;
					x++;
				}
				else if(auxBoard[y+1][x] == number + 1){
					auxBoard[y+1][x] = 2;
					number++;
					y++;
				}
			}
		}
	}
	
	cout<<endl<<endl<<endl;
	cout<<"Your steps: "<<steps<<endl;
	cout<<"Best route: "<<stepsNecessary<<endl;
	
	if(steps > stepsNecessary){
		printBestRoute();
		COORD position;
		position.X = 0;
		position.Y = mazeY + 3;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	}
	system("pause");
}

int main(){
	
	srand(time(0));
	int row = 4000000;
	char levels[4][25] = {"          Easy          ", "         Medium         ", "          Hard          ", "         Insane         "};
	sp(175);
	cout<<levels[0]<<endl;
	sp(236);
	cout<<levels[1]<<endl;
	cout<<levels[2]<<endl;
	cout<<levels[3]<<endl;
	s();
	cout<<endl<<"  Controls:"<<endl<<"    w - up"<<endl<<"    s - down"<<endl<<"    a - left"<<endl<<"    d - right"<<endl;
	bool menu = true;
	while(menu){
		if(_kbhit()){
			bool change = false;
			int previous = row;
			COORD position;
			position.X = 0;
			char key = _getch();
			switch(key){
				case 'w':{
					row--;
					change = true;
					break;
				}
				case 's':{
					row++;
					change = true;
					break;
				}
				case 13:{
					switch(row % 4){
						case 0:{
							mazeX = 25;
							mazeY = 25;					
							break;
						}
						case 1:{
							mazeX = 35;
							mazeY = 35;	
							break;
						}
						case 2:{
							mazeX = 45;
							mazeY = 45;	
							break;
						}
						case 3:{
							mazeX = 60;
							mazeY = 60;	
							break;
						}
					}
					board = new int*[mazeY];
					auxBoard = new int*[mazeY];
					for(int i = 0; i < mazeY; i++){
						board[i] = new int[mazeX];
						auxBoard[i] = new int[mazeX];
					}
					menu = false;
					break;
				}
			}
			
			if(change){
				position.Y = previous % 4;
				sp(236);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
				cout<<levels[previous % 4];
				position.Y = row % 4;
				sp(175);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
				cout<<levels[row % 4];
				s();
			}
		}
	}
	
	system("cls");
	do{
		generateMaze();
	}
	while(!checkIfSolvable());
	printMaze();
	printPlayer();
	
	while(1){
		if(_kbhit()){
			char key = _getch();
			switch(key){
				case 'w':{
					if(y != 0 && board[y-1][x] != 1){
						y -= 1;
						printPlayer();
						deletePlayer(x, y+1);
					}
					break;
				}
				case 's':{
					if(y != mazeY - 1 && board[y+1][x] != 1){
						y += 1;
						printPlayer();
						deletePlayer(x, y-1);
					}
					break;
				}
				case 'a':{
					if(x != 1 && board[y][x-1] != 1){
						x -= 1;
						printPlayer();
						deletePlayer(x+1, y);
					}
					break;
				}
				case 'd':{
					if(x != mazeX - 2 && board[y][x+1] != 1){
						x += 1;
						printPlayer();
						deletePlayer(x-1, y);
					}
					break;
				}
			}
		}
		if(y == mazeY - 1){
			checkIfVictory();
			system("cls");
			do{
				generateMaze();
			}
			while(!checkIfSolvable());
			clearScreen();
			y = 0;
			printMaze();
			printPlayer();
		}
	}
	
	return 0;
}
