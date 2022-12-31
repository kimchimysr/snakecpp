#include<iostream>
#include<ctime> //function: rand(), srand(), time()
#include<conio.h> //function: getch(), kbhit()
#include<windows.h> //funciton: Sleep(), ClearScreen(), ShowConsoleCusor()
using namespace std;

const int WIDTH = 30;
const int HEIGHT = 20;
bool gameOver;
int gameMode;
int score;
int nTail;

enum direction {STOP = 0, LEFT, RIGHT, UP, DOWN}; //constant variables for direction
direction dir;

struct position //to store position of snake's head and food
{
	int x,y;
};
position snakeHead;
position food;

struct tail //to store position of tails
{
	int x[200], y[200];
};
tail snakeTail;

void ClearScreen(); //for clearing screen without flickering
void ShowConsoleCursor(bool showFlag); //for hiding cursor
void menu(); //showing main menu
void setup(); //initialize the variables
void board(); //to draw a board for snake to move within
void input(); //register user's input and move according to the key pressed
void mechanic(); //logic for snake
void message(); //show after game is over

int main()
{
	srand((unsigned) time(0)); //seed random number generator based on current time
	char retry;
	ShowConsoleCursor(false);
	
	mainMenu:
	system("Color 07"); //bgcolor = black, txtcolor = white
	menu();
	restart: //label for restarting the current game mode
	setup();
	while(!gameOver)
	{
		ClearScreen();
		board();
		input();
		mechanic();
		Sleep(50);
	}
	system("Color 04"); //bgcolor = black, txtcolor = red
	message();
	
	cout<<"\n\n\t\t\t\t\tPress R to restart."<<endl
		<<"\t\t\t\t\tPress E to go back to menu."<<endl
		<<"\t\t\t\t\tPress Esc to exit.";
	retry = getch();
	if(retry == 114 || retry== 101)
	{
		fill_n(snakeTail.x, 200, 0);
		fill_n(snakeTail.y, 200, 0);
		if(retry == 114)
			goto restart;
		else goto mainMenu;
	}
	else
	return 0;
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void ClearScreen()
{	
	COORD cursorPosition;	
	cursorPosition.X = 0;	
	cursorPosition.Y = 0;	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void menu()
{
	do
	{
		system("cls");
		cout<<"\n\t\t\t\t --------------------------------------------\n";
		cout<<"\t\t\t\t\t\t :Snake Game:\n";
		cout<<"\t\t\t\t --------------------------------------------\n\n"
			<<"\t\t\t\t\t\t-Instruction-\n"
			<<"\t\t\t\t Your goal is to eat as many food as possible\n"
			<<"\t\t\t\t\t before biting your own tail.\n\n"
			<<"\t\t\t\t   -Easy: (no boundary, +10 points/food)\n"
			<<"\t\t\t\t   -Hard: (has boundary, +20 points/food)\n"
			<<"\t\t\t\t\t\t -Controls-\n"
			<<"\t\t\t\t\t    Use w,a,s,d to move.\n"
			<<"\n\t\t\t\t Please choose difficulty:\n"
			<<"\t\t\t\t 1. Easy\n"
			<<"\t\t\t\t 2. Hard\n";
		gameMode = getch() - '0';
		if(gameMode != 1 && gameMode != 2)
		{
			cout<<"\n\t\t\t\t\tInvalid! Please select given options.";
			cout<<"\n\t\t\t\t\t";
			system("pause");
		}
	}while(gameMode != 1 && gameMode != 2);
}

void setup()
{ 
	system("cls");
	gameOver = false; //default is false
	dir = STOP; //make snake's head still
	snakeHead.x = WIDTH / 2; //make snake's head at x axis center
	snakeHead.y = HEIGHT / 2; //make snake's head at y axis center
	food.x = rand() % WIDTH; //random number between 1 and WIDTH
	food.y = rand() % HEIGHT; //random number between 1 and HEIGHT
	nTail = 0;
	score = 0;
}

void board()
{
	system("Color 02"); //bgcolor = black, txtcolor = green
	cout<<"\t\t\t\t\t";
	for(int i = 0; i < WIDTH + 2; i++)
		cout<<"H"; //for printing upper wall
	cout<<endl;
	
	for(int i = 0; i < HEIGHT; i++)
	{
		cout<<"\t\t\t\t\t";
		for(int j = 0; j < WIDTH; j++)
		{
			if(j == 0) cout<<"H"; //for printing left side wall
			
			if(i == snakeHead.y && j == snakeHead.x) cout<<"O"; //to make snake's head center
			else if(i == food.y && j == food.x) cout<<"*"; //for generate random food within frame
			else
			{
				bool print = false;
				for(int k = 0; k < nTail; k++)
				{
					if(snakeTail.x[k] == j && snakeTail.y[k] == i)
					{
						cout<<"o";
						print = true;
					}
				}
				if(!print) cout<<" ";
			}
			if(j == WIDTH - 1) cout<<"H"; //for printing right side wall
		}
		cout<<endl;
	}
	
	cout<<"\t\t\t\t\t";
	for(int i = 0; i < WIDTH + 2; i++)
		cout<<"H"; //for printing lower wall
	cout<<endl;
	cout<<"\t\t\t\t\tScore: "<<score<<endl; //display score
}

void input()
{
	if(kbhit()) //when user using keyboard
		switch(getch()) //it take character
		{
			case 'w': dir = UP; break;
			case 's': dir = DOWN; break;
			case 'a': dir = LEFT; break;
			case 'd': dir = RIGHT; break;
			case 'x': gameOver = true;
		}
}

void mechanic()
{
	position prev;
	position prev2;
	
	prev.x = snakeTail.x[0];
	prev.y = snakeTail.y[0];
	snakeTail.x[0] = snakeHead.x; //storing first tail value of x axis
	snakeTail.y[0] = snakeHead.y; //storing first tail value of y axis
	
	for(int i = 1; i <= nTail; i++) //for storing previous tail position
	{
		prev2.x = snakeTail.x[i];
		prev2.y = snakeTail.y[i];
		snakeTail.x[i] = prev.x;
		snakeTail.y[i] = prev.y;
		prev.x = prev2.x;
		prev.y = prev2.y;
	}
	
	switch(dir)
	{
		case UP: snakeHead.y--; break; //decrease value of y when w is pressed
		case DOWN: snakeHead.y++; break; //increase value of y when s is pressed
		case LEFT: snakeHead.x--; break; //decrease value of x when a is pressed
		case RIGHT: snakeHead.x++; break; //increase value of x when d is pressed
		default: break;
	}
	
	if(gameMode == 1) //easy mode: when snake's head hit wall reset their position
	{
		if (snakeHead.x >= WIDTH) snakeHead.x = 0; else if (snakeHead.x < 0) snakeHead.x = WIDTH - 1;
    	if (snakeHead.y >= HEIGHT) snakeHead.y = 0; else if (snakeHead.y < 0) snakeHead.y = HEIGHT - 1;
	}
	else //hard mode: when snake's head hit wall, gameOver = true
	{
		if (snakeHead.x > WIDTH || snakeHead.x < 0 || snakeHead.y > HEIGHT || snakeHead.y < 0)
      		gameOver = true;
	}
	
	for (int i = 0; i < nTail; i++) // when snake hit its own tail, gameOver = true
        if (snakeHead.x == snakeTail.x[i] && snakeHead.y == snakeTail.y[i])
            gameOver = true;
	
	if(snakeHead.x == food.x && snakeHead.y == food.y) //if snake eat food, its gain point and new tail + gen random food
	{
		if(gameMode == 1)
			score += 10;
		else score += 20;
		food.x = rand() % WIDTH;
		food.y = rand() % HEIGHT;
		nTail++;
	}
}

void message()
{
	cout<<"\t\t\t\t\tStatus: Game Over!";
}
