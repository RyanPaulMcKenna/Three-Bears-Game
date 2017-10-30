//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//For resizing console window
#define _WIN32_WINNT 0x0500
#include <windows.h>
//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <ctime>
#include <locale>
#include <cctype>
using namespace std;

//include our own libraries
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int  SIZEX(25);    	//horizontal dimension
const int  SIZEY(25);		//vertical dimension
//defining symbols used for display of the grid and content
const char BEAR('@');   	//bear
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char BOMB('0');		//Bomb
const char EXIT('x');		//Exit
const char DETONATOR('T');	//Detonator
//defining the command letters to move the bear on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
const char C('C');
//defining the other command letters
const char QUIT('Q');		//to open menu
const char INFO('F');
const int MAXBEARS(3);
const int DEC(10);

//Variables for functions
struct Item {
	int x, y;
	char symbol;
};

struct vars {
	char name[10];
	int bearEscaped = 0;
	int movesSoFar = 0;
	int endGame = 0;
	int cheat = 0;
};
vars myvars;
//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------
int main()
{


	//function declarations (prototypes)
	void entryScreen(vars& myvars);
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], vector<Item>& bears, vector<Item>& bombs);
	void paintGame(const char g[][SIZEX], string mess, vars myvars);
	bool wantsToQuit(const int key);
	bool infoKey(const int key);
	void displayRules();
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], vector<Item>& bears, vector<Item>& bombs, const int key, string& mess, vars& myvars);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const vector<Item> bears, const vector<Item> bombs);
	void endProgram(vars& myvars);
	void menu(vars myvars);
	void restart(char g[][SIZEX], char m[][SIZEX], vector<Item>& bears, vector<Item>& bombs, vars& myvars);


	//local variable declarations 
	char grid[SIZEY][SIZEX];	//grid for display
	char maze[SIZEY][SIZEX];	//structure of the maze
	vector<Item> bears = { { 0, 0, BEAR }, { 0, 0, BEAR }, { 0, 0, BEAR } }; 			//bear's position and symbol
	vector<Item> bombs = { { 0, 0, BOMB }, { 0, 0, BOMB }, { 0, 0, BOMB }, { 0, 0, BOMB }, { 0, 0, BOMB } };
	

	string message("LET'S START...");		//current message to player
	entryScreen(myvars);	// Entry Screen
	menu(myvars);		//opens menu

	if (myvars.endGame == 0)
	{
		initialiseGame(grid, maze, bears, bombs);						//initialise grid (incl. walls & bear)
		paintGame(grid, message, myvars);		//display game info, modified grid & messages
	}
		

		int key(getKeyPress()); 			//read in  selected key: arrow or letter command

		while (myvars.endGame == 0)			//while user does not want to quit
		{
			if (isArrowKey(key))
			{
				updateGameData(grid, bears, bombs, key, message, myvars);		//move bear in that direction
				updateGrid(grid, maze, bears, bombs);			//update grid information
			}
			else
			{
				message = "INVALID KEY!";	//set 'Invalid key' message
			}
	
				paintGame(grid, message, myvars);		//display game info, modified grid & messages
				key = getKeyPress(); 			//display menu & read in next option
				
				if (wantsToQuit(key)){ //if the player presses 'q', open the menu and restart the game
					system("CLS");
					menu(myvars);
					restart(grid, maze, bears, bombs, myvars);
				}

				if (infoKey(key)){ // if the player presses 'F'
					displayRules();
					system("CLS");
				}
		}


	endProgram(myvars);						//display final message
	return 0;

}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

//Displays the rules of the game when 'F' is pressed
void displayRules(){
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, string cheatMess);
	showMessage(clBlack, clWhite, 40, 13, "GAME LEVEL 1 RULES:                ");
	showMessage(clBlack, clWhite, 40, 14, "Rescue all the bears '@' through   ");
	showMessage(clBlack, clWhite, 40, 15, "exit 'X' avoiding bombs '0'        ");
	showMessage(clBlack, clWhite, 40, 16, "To disable bombs use detonator 'T' ");
	showMessage(clBlack, clWhite, 40, 17, "                                   ");
	showMessage(clBlack, clWhite, 40, 18, "TO MOVE USE KEYBOARD ARROWS        ");
	showMessage(clBlack, clWhite, 40, 19, "TO RETURN TO MENU PRESS 'Q'        ");
	Gotoxy(0, 23);
	cout << "GAME PAUSED -   ";
	system("pause");
}

// DISPLAYS THE MENU
void menu(vars myvars) {


	void rules(vars myvars);
	void quit(vars& myvars);
	void score(vars myvars);
	void highScores(vars myvars);
	
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, string cheatMess);
	int option;
	//displays the menu info
	showMessage(clBlack, clBlue, 22, 4, "--------------------------------------------------------------------");
	showMessage(clBlack, clBlue, 22, 14, "--------------------------------------------------------------------");
	showMessage(clBlack, clWhite, 15, 5, "		GAME MENU		 \n");
	showMessage(clBlack, clWhite, 15, 7, "		QUIT:			0");
	showMessage(clBlack, clWhite, 15, 8, "		PLAY GAME:		1");
	showMessage(clBlack, clWhite, 15, 9, "		SHOW RULES:		2");
	showMessage(clBlack, clWhite, 15, 10, "		SHOW SCORE		3");
	showMessage(clBlack, clWhite, 15, 11, "		HIGH SCORE		4\n");
	showMessage(clBlack, clWhite, 15, 13, "		Select Option:		");

	cin >> option;

	//Case statement for each option of the menu
	switch (option) {
	case 0: Clrscr();
		quit(myvars);
		break;
	case 1:
		Clrscr();
		break;
	case 2:
		Clrscr();
		rules(myvars);
		break;
	case 3: Clrscr();
		score(myvars);
		break;
	case 4:
		Clrscr();
		highScores(myvars);
		break;
	default: showMessage(clBlack, clWhite, 15, 13, "		ENTER A VALID NUMBER		");
		break;

	}
}

//RESTARTS THE GAME
void restart(char g[][SIZEX], char m[][SIZEX], vector<Item>& bears, vector<Item>& bombs, vars& myvars){
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], vector<Item>& bears, vector<Item>& bombs);

	// Reinitialises the game, and restores values
	initialiseGame(g, m, bears, bombs);			//initialise grid (incl. walls & bear)
	myvars.movesSoFar = 0;
	myvars.bearEscaped = 0;
	

}

//DISPLAYS THE RULES 
void rules(vars myvars) {
	void menu(vars myvars);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, string cheatMess);
	string back;

	showMessage(clBlack, clBlue, 22, 4, "--------------------------------------------------------------------");
	showMessage(clBlack, clBlue, 22, 15, "--------------------------------------------------------------------");
	showMessage(clBlack, clWhite, 55, 5, "RULES");
	showMessage(clBlack, clWhite, 15, 7, "		1: Move the bears using the arrow keys.		 ");
	showMessage(clBlack, clWhite, 15, 8, "		2: If one of the bears touches a bomb(0) its game over.		 ");
	showMessage(clBlack, clWhite, 15, 9, "		3: Touch the detonator(T) to remove all the bombs.		 ");
	showMessage(clBlack, clWhite, 15, 10, "		4: All bears must reach the exit point(X) to finish the level.		 \n\n");
	showMessage(clBlack, clWhite, 15, 11, "		5: If the timer reaches 0, you lose.		 \n\n");
	showMessage(clBlack, clWhite, 15, 13, "		Enter Any Key To Return:  ");
	cin >> back;
	Clrscr();
	menu(myvars);
}

//QUIT OPTION
void quit(vars& myvars) {
	void menu(vars myvars);
	char option;
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, string cheatMess);
	showMessage(clBlack, clBlue, 22, 4, "--------------------------------------------------------------------");
	showMessage(clBlack, clBlue, 22, 8, "--------------------------------------------------------------------");
	showMessage(clBlack, clWhite, 15, 5, "		END GAME?		 ");
	showMessage(clBlack, clWhite, 15, 7, "		Y/N:	 ");
	cin >> option;

	//Switch statement to allow the user to quit or go back to menu
	switch (option)
	{
	case 'Y':
	case 'y':
		myvars.endGame = 1;
		Clrscr();
		break;
	case 'n':
	case 'N':
		Clrscr();
		menu(myvars);
		break;
	default: showMessage(clBlack, clWhite, 15, 8, "ENTER A VALID LETTER");
		break;
	}
}

//Displays the score menu
void score(vars myvars) {
	void menu(vars myvars);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, string cheatMess);
	string back;
	string name;
	string fileName;
	int score = 0;
	showMessage(clBlack, clBlue, 22, 4, "--------------------------------------------------------------------");
	showMessage(clBlack, clBlue, 22, 11, "--------------------------------------------------------------------");
	showMessage(clBlack, clWhite, 15, 5, "		Who's score do you want to know?		 ");
	showMessage(clBlack, clWhite, 15, 6, "		Player's Name:  ");
	cin >> name;
	ifstream fin;
	fileName = ".\\scores\\" + name + ".txt";
	fin.open(fileName.c_str(), ios::in);
	if (fin.fail())
		showMessage(clBlack, clWhite, 15, 7, "		Player not found.  ");

	else {
		fin >> score;
		showMessage(clBlack, clWhite, 24, 7, name + "'s previous score is: ");
		cout << score << "\n\n";
	}

	showMessage(clBlack, clWhite, 15, 9, "		Enter Any Key To Return:  ");
	cin >> back;
	Clrscr();
	menu(myvars);
}

void highScores(vars myvars) {

	void menu(vars myvars);
	cout << "The High Scores Table!" << endl;
	string output;
	string score;
	string currentName;
	struct playersAndScore
	{
		string name;
		string score;
	};
	playersAndScore myPlayers[4];
	ifstream fin;
	string fileName;
	int index(0);
	fin.open(".\\scores\\players.txt", ios::in);
	if (fin.fail()) {
		//Do nothing
		cout << "Sorry there was an error opening the score records" << endl;
	}
	else 
	{
		//Read Player names in to string
		while (fin) {
			//read
			fin >> output;
					}
		fin.close();
	}
		// Extract each name from string
		//--------------------------------------
	for (size_t i(0); i < output.size()-1; i++) 
	{
			
		if (isupper(output.at(i)))
		{
				
			currentName = output.at(i);
			++i;
			while(islower(output.at(i)))
			{
				currentName += output.at(i);
				++i;
			}
			
			fileName = ".\\scores\\" + currentName + ".txt";
			fin.open(fileName.c_str(), ios::in);
			if(fin.fail()) { cout << "\n"; }
			else 
			{
				fin >> score;
				myPlayers[index].name = currentName;
				myPlayers[index].score = score;
				index++;
			}
		}	
	}
	for (size_t j = (0); j < 4; j++)
	{
		cout << myPlayers[j].name + "\t" + myPlayers[j].score + "\n";
	}
	//--------------------------------------
	system("pause");
	Clrscr();
	menu(myvars);
	

}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------
void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], vector<Item>& bears, vector<Item>& bombs)
{ //initialise grid & place bear in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setInitialDataFromMaze(char maze[][SIZEX], vector<Item>& bears, vector<Item>& bombs);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], vector<Item> bears, vector<Item> bombs);

	setInitialMazeStructure(maze);			//initialise maze
	setInitialDataFromMaze(maze, bears, bombs);	//initialise bear's position
	updateGrid(grid, maze, bears, bombs);			//prepare grid
}

void setInitialMazeStructure(char maze[][SIZEX])
{ //set the position of the walls in the maze
	//initialise maze configuration
	int initialMaze[SIZEY][SIZEX] 	//local array to store the maze structure
		= { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 2, 3, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 4, 0, 0, 0, 3, 0, 1, 0, 1, 0, 1 },
		{ 1, 2, 1, 0, 1, 3, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1 },
		{ 1, 2, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };
	// with 1 for wall, 0 for tunnel, etc. 
	//copy into maze structure
	for (int row(0); row < SIZEY; ++row)
	for (int col(0); col < SIZEX; ++col)
		switch (initialMaze[row][col])
	{
		case 0: maze[row][col] = TUNNEL; break;
		case 1: maze[row][col] = WALL; break;
		case 2: maze[row][col] = BEAR; break;
		case 3: maze[row][col] = BOMB; break;
		case 4: maze[row][col] = EXIT; break;
		case 5: maze[row][col] = DETONATOR; break;
	}
}

void setInitialDataFromMaze(char maze[][SIZEX], vector<Item>& bears, vector<Item>& bombs)
{ //extract bear's coordinates from initial maze info
	int bombNo = 0;
	int bearNo = 0;
	for (int row(0); row < SIZEY; ++row)
	for (int col(0); col < SIZEX; ++col)
		switch (maze[row][col])
	{
		case BEAR:
		{
					 bears.at(bearNo).x = col;
					 bears.at(bearNo).y = row;
					 bearNo++;
					 maze[row][col] = TUNNEL;

		}
			break;
		case BOMB:
		{
					 bombs.at(bombNo).x = col;
					 bombs.at(bombNo).y = row;
					 bombNo++;
					 maze[row][col] = TUNNEL;

		}
			break;

			//will work for other items too
	}
}
//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const vector<Item> bears, const vector<Item> bombs)
{ //update grid configuration after each move
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeBear(char g[][SIZEX], const vector<Item> bears, const vector<Item> bombs);

	setMaze(grid, maze);	//reset the empty maze configuration into grid
	placeBear(grid, bears, bombs);	//set bear in grid
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
	for (int col(0); col < SIZEX; ++col)
		grid[row][col] = maze[row][col];
}

void placeBear(char g[][SIZEX], const vector<Item> bears, const vector<Item> bombs)
{ //place bear at its new position in grid
	for (int bombNo = 0; bombNo < 5; bombNo++){
		for (int bearNo = 0; bearNo < MAXBEARS; bearNo++)
			g[bears.at(bearNo).y][bears.at(bearNo).x] = bears.at(bearNo).symbol;
		g[bombs.at(bombNo).y][bombs.at(bombNo).x] = bombs.at(bombNo).symbol;
	}
}
//---------------------------------------------------------------------------
//----- move the bear
//---------------------------------------------------------------------------
void updateGameData(const char g[][SIZEX], vector<Item>& bears, vector<Item>& bombs, const int key, string& mess, vars& myvars)
{ //move bear in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy, vars& myvars);
	void endProgram(vars& myvars);
	assert(isArrowKey(key));
	
	//reset message to blank
	mess = "                                         ";		//reset message to blank

	//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy, myvars);
	//check new target position in grid and update game data (incl. bear coordinates) if move is possible

	for (int bearNo = 0; bearNo < MAXBEARS; ++bearNo)
		switch (g[bears.at(bearNo).y + dy][bears.at(bearNo).x + dx])
	{			//...depending on what's on the target position in grid...
		case TUNNEL:		//can move
			bears.at(bearNo).y += dy;	//go in that Y direction
			bears.at(bearNo).x += dx;	//go in that X direction
			break;
		case WALL:  		//hit a wall and stay there
			//cout << '\a';	//beep the alarm
			mess = "CANNOT GO THERE !!";
			break;
		case BOMB:
			if (myvars.cheat == 0)
			{
				bears.at(bearNo).y = TUNNEL; //Make bears invisible 
				bears.at(bearNo).x = TUNNEL;
				SelectTextColour(clBlue);
				myvars.endGame = 2;
				endProgram(myvars);
				mess = "     BEAR DIES!";
			}
			else
			{
				bears.at(bearNo).y += dy;	//go in that Y direction
				bears.at(bearNo).x += dx;	//go in that X direction	
			}
			break;
		case EXIT:
			bears.at(bearNo).y = TUNNEL;
			bears.at(bearNo).x = TUNNEL;
			myvars.bearEscaped++;
			if (myvars.bearEscaped == 3){
				myvars.endGame = 1;
				endProgram(myvars);
			}
			break;
		case DETONATOR:
			if (myvars.cheat == 0)
			{
				bombs.at(0).y = TUNNEL;
				bombs.at(0).x = TUNNEL;
				bombs.at(1).y = TUNNEL;
				bombs.at(1).x = TUNNEL;
				bombs.at(2).y = TUNNEL;
				bombs.at(2).x = TUNNEL;
				bombs.at(3).y = TUNNEL;
				bombs.at(3).x = TUNNEL;
				bombs.at(4).y = TUNNEL;
				bombs.at(4).x = TUNNEL;
				bears.at(bearNo).y += dy;	//go in that Y direction
				bears.at(bearNo).x += dx;	//go in that X direction	
				mess = "Deactivated Bomb!";
			}
			break;
		case BEAR:	//Checks bears positions relative to eachother and the walls of the grid
			if ((g[bears.at(bearNo).y + (2 * dy)][bears.at(bearNo).x + (2 * dx)]) == BEAR)
			{
				if ((g[bears.at(bearNo).y + (3 * dy)][bears.at(bearNo).x + (3 * dx)]) != WALL)
				{
					//move 3 bears
					bears.at(bearNo).y = bears.at(bearNo).y + dy;
					bears.at(bearNo).x = bears.at(bearNo).x + dx;
				}

			}
			else {
				if ((g[bears.at(bearNo).y + (2 * dy)][bears.at(bearNo).x + (2 * dx)]) != WALL)
				{
					//move 2 bears
					bears.at(bearNo).y = bears.at(bearNo).y + dy;
					bears.at(bearNo).x = bears.at(bearNo).x + dx;
				}
			}

			break;
	}
}
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy, vars& myvars)
{ //calculate direction indicated by key
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	void cheatEnabled(vars& myvars);
	void cheatDisabled(vars& myvars);
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		myvars.movesSoFar++;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		myvars.movesSoFar++;
		break;
	case UP: 		//when UP arrow pressed...
		dx = 0;
		dy = -1;	//decrease the Y coordinate
		myvars.movesSoFar++;
		break;
	case DOWN: 		//when DOWN arrow pressed...
		dx = 0;
		dy = +1;	//increase the Y coordinate
		myvars.movesSoFar++;
		break;
	case C:
		if (myvars.cheat == 0)
		{
			cheatEnabled(myvars);
		}
		else
		{
			cheatDisabled(myvars);
		}
		break;
	}
}

void cheatEnabled(vars& myvars){
	//Enables cheat mode
	string cheatMess;
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, string cheatMess);
	showMessage(clBlack, clWhite, 40, 4, "CHEAT MODE ENABLED");
	showMessage(clBlack, clWhite, 40, 5, "BOMBS DISABLED");
	myvars.movesSoFar = 500;
	myvars.cheat = 1;
	for (int i = 0; i < 3; i++)
	{
		Beep(500, 100);
	}
}

void cheatDisabled(vars& myvars) {
	//Disabled cheat mode.
	for (int i = 0; i < 3; i++)
	{
		Beep(500, 100);
	}
	system("CLS");
	myvars.cheat = 0;
}

int getKeyPress()
{ //get key or command (in uppercase) selected by user
	//KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return toupper(keyPressed);		//return it in uppercase 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN) || (key == C);
}

bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')
	return toupper(key) == QUIT;
}

bool infoKey(const int key)
{
	return toupper(key) == INFO;
}
//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(char x) {
	std::ostringstream os;
	os << x;
	return os.str();
}

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message)
{
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}

void entryScreen(vars& myvars)
{
	showMessage(clBlack, clBlue, 22, 4, "--------------------------------------------------------------------");
	showMessage(clBlack, clBlue, 22, 12, "--------------------------------------------------------------------");
	// Displays the entry screen where the user enters their name.
	showMessage(clBlack, clWhite, 45, 5, "Three Bears Game");
	showMessage(clBlack, clWhite, 22, 7, "Ryan Mckenna");
	showMessage(clBlack, clWhite, 22, 8, "SE1T - FoP - 2016-2017                     ");
	showMessage(clBlack, clWhite, 22, 10, "Enter Your Name:  ");
	cin >> myvars.name;
	myvars.name[0] = toupper(myvars.name[0]);
	Clrscr();
}

void paintGame(const char g[][SIZEX], string mess, vars myvars)
{ //display game title, messages, maze, bear and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void paintGrid(const char g[][SIZEX]);
	
	//display game title
	paintGrid(g);
	showMessage(clYellow, clBlack, 0, 0, "THREE BEARS GAME");
	showMessage(clGrey, clYellow, 0, 2, "Rescued:      ");
	if (myvars.bearEscaped == 1)
	{
		showMessage(clGrey, clYellow, 0, 2, "Rescued: @    ");
	}
	if (myvars.bearEscaped == 2)
	{
		showMessage(clGrey, clYellow, 0, 2, "Rescued: @ @  ");
	}
	if (myvars.bearEscaped == 3)
	{
		showMessage(clGrey, clYellow, 0, 2, "Rescued: @ @ @");
	}

	// display grid contents
	showMessage(clGrey, clWhite, 0, 16, "BEAR:      ");
	showMessage(clGrey, clGreen, 11, 16, "@     ");
	showMessage(clGrey, clWhite, 0, 17, "BOMB:      ");
	showMessage(clGrey, clRed, 11, 17, "0     ");
	showMessage(clGrey, clWhite, 0, 18, "DETONATOR: ");
	showMessage(clGrey, clYellow, 11, 18, "T     ");
	showMessage(clGrey, clWhite, 0, 19, "EXIT:      ");
	showMessage(clGrey, clBlack, 11, 19, "X     ");

	//Player info
	string strName;

	for (int i(0); i <= DEC; i++) {

		if (!isspace(myvars.name[i]) && isalpha(myvars.name[i])) {
			strName += (tostring(myvars.name[i]));
			
		}

	}
	int spaces = 23 - strName.size();
	for (int i(0); i < spaces; i++)
	{
		strName += " ";
	}

	showMessage(clGrey, clYellow, 40, 0, "Current Player: ");
	showMessage(clGrey, clYellow, 56, 0, strName);
	showMessage(clGrey, clYellow, 40, 1, "Previous Score:                        ");
	showMessage(clGrey, clYellow, 40, 2, "Date and Time: ");
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	cout << (now->tm_year + 1900) << '-'
		<< (now->tm_mon + 1) << '-'
		<< now->tm_mday
		<< "  "
		<< now->tm_hour
		<< ":"
		<< now->tm_min
		<< ":"
		<< now->tm_sec
		<< "     "
		<< endl;
	

	showMessage(clBlack, clWhite, 40, 6, "Moves so Far:"); // Variables needed on end
	showMessage(clBlack, clWhite, 55, 6, to_string(myvars.movesSoFar));
	showMessage(clBlack, clWhite, 40, 7, "Bears Out:      "); // variables needed on end
	showMessage(clBlack, clWhite, 55, 7, to_string(myvars.bearEscaped));
	//display menu options available
	showMessage(clBlack, clWhite, 40, 13, "PRESS F TO DISPLAY RULES");

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 9, mess);	//display current message
}

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 4);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col) {
			switch (g[row][col]) {
			case BEAR: SelectTextColour(clGreen); cout << g[row][col]; break;		//output cell content
			case WALL: SelectTextColour(clWhite); cout << g[row][col]; break;
			case BOMB: SelectTextColour(clRed); cout << g[row][col]; break;
			case EXIT: SelectBackColour(clDarkGrey); SelectTextColour(clBlack); cout << g[row][col]; SelectBackColour(clBlack); break;
			case DETONATOR: SelectTextColour(clYellow); cout << g[row][col]; break;
			case TUNNEL: cout << g[row][col]; break;
			}
		}
		cout << endl;
	}
}

void endProgram(vars& myvars)
{
	//Score Algorithm
	//-------------------------- 
	int calcScore = 0;
	for (int i = 0; i < myvars.movesSoFar; i++) {
		if (myvars.movesSoFar < 69) {
			calcScore++;
		}
		if (myvars.movesSoFar > 69) {
			calcScore--;
		}

	}
	int score = 500 - calcScore;
	if (score > 500) { score = 500; }
	//---------------------------
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	switch (myvars.endGame)
	{
	case 1:	showMessage(clRed, clYellow, 40, 21, "END OF GAME: BEARS RESCUED");  break;
	case 2: showMessage(clRed, clYellow, 40, 21, "END OF GAME: GAME OVER"); score = 500; break;
	}
	// Output score to named file
	//---------------
	string strName;

	for (int i(0); i <= DEC; i++) {
		if (!isspace(myvars.name[i]) && isalpha(myvars.name[i])) {
			strName += (tostring(myvars.name[i]));}
							}


		ofstream fout;
		string fileName;
		fileName = ".\\scores\\" + strName + ".txt";
		fout.open(fileName.c_str(), ios::out);
		if (fout.fail()) {
			cout << "Error creating player score file" << endl;
		}
		else
		{
			fout << score;
			fout.close();
			remove(".\\scores\\.txt");
		}
		//----------------
		//Reference of all players for high score table..
		fout.open(".\\scores\\players.txt", ios::app);
		fout << strName;
		fout.close();

		//----------------
		showMessage(clBlack, clWhite, 40, 22, "");
	

}