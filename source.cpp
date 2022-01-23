#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

#define LENGTH 18
#define WIDTH 18
#define BOMBS 10

int user_board[LENGTH][WIDTH];
int board[LENGTH][WIDTH];
int foundedBombs = 0;
int finish = 0;
int firstMove = 0;

#define w 32
#define Cliked 0
#define Bomb 9
#define None 10
#define Flag 11

void initUserBoard(){
	for (int i = 1; i < LENGTH - 1; i++){
		for (int j = 1; j < WIDTH - 1; j++){
			user_board[i][j] = None;
		}
	}
}

bool isMine(int x, int y){
	if (board[x][y] == Bomb)
		return true;
	return false;
}

void initBombs(int x, int y){
	int cnt = BOMBS;

	while (cnt-- > 0){
		int a = rand() % (LENGTH-2) +1;
		int b = rand()%(WIDTH-2)+1;

		if (isMine(x, y) || (x == a && y == b)) cnt++;
		else board[a][b] = Bomb;
	}
}

void getMineNumber(){
	for (int a = 1; a < LENGTH - 1; a++){
		for (int b = 1; b < WIDTH - 1; b++){
			int count = 0;

			if (isMine(a, b)) continue;
			if (isMine(a - 1, b - 1)) count++;
			if (isMine(a - 1, b )) count++;
			if (isMine(a - 1, b + 1)) count++;
			if (isMine(a , b - 1)) count++;
			if (isMine(a, b + 1)) count++;
			if (isMine(a + 1, b - 1)) count++;
			if (isMine(a + 1, b)) count++;
			if (isMine(a + 1, b + 1)) count++;

			board[a][b] = count;
		}
	}

	for (int a = 1; a < LENGTH - 1; a++){
		for (int b = 1; b < WIDTH - 1; b++){
			cout << board[b][a] << " ";
		}
		cout << endl;
	}
}

void openAround(int a, int b){
	int cnt = 0;
	for (int i = -1; i <= 1; i++){
		for (int j = -1; j <= 1; j++){
			if (user_board[a + i][b + j] != None) continue;
			
			user_board[a + i][b + j] = board[a + i][b + j];

			if (board[a + i][b + j] == 0) 
				openAround(a + i, b + j);
		}
	}
}


int main(){
	RenderWindow app(VideoMode(600, 600), "Minesweeper");

	Texture t;
	t.loadFromFile("images/tiles.jpg");
	Sprite  s(t);

	Texture winsource;
	winsource.loadFromFile("images/win.jpg");
	Sprite win(winsource);

	srand((unsigned int)time(NULL));

	initUserBoard();

	while (app.isOpen()){
		Vector2i pos = Mouse::getPosition(app);

		int x = pos.x / w;
		int y = pos.y / w;

		Event e;

		while (app.pollEvent(e)){
			if (e.type == Event::Closed)
				app.close();

			if (e.type == Event::MouseButtonPressed){
				if (e.key.code == Mouse::Left){
					if (firstMove == 0){
						initBombs(x, y);
						getMineNumber();
						user_board[x][y] = board[x][y];
						firstMove = 1;
					}
					else if (board[x][y] == Bomb) finish = 1;
					else user_board[x][y] = board[x][y];

					if (board[x][y] == 0)
						openAround(x, y);
				}
				else if (e.key.code == Mouse::Right){
					if (user_board[x][y] == Flag){
						user_board[x][y] = None;
						if (board[x][y] == Bomb)
							foundedBombs--;

					}
					else if (user_board[x][y] == None){
						user_board[x][y] = Flag;
						if (board[x][y] == Bomb)
							foundedBombs++;
					}
				}
			}
		}

		app.clear(Color::White);

		if (foundedBombs == BOMBS){
			win.setPosition(120, 110);
			app.draw(win);
			app.display();
		}
		else{
			for (int i = 1; i < LENGTH - 1; i++){
				for (int j = 1; j<WIDTH - 1; j++){
					if (finish == 1){
						user_board[i][j] = board[i][j];
					}
					s.setTextureRect(IntRect(user_board[i][j] * w, 0, w, w));
					s.setPosition(i * w, j * w);
					app.draw(s);
				}
			}
			app.display();
		}
	}
	return 0;
}
