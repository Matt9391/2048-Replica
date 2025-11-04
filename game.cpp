#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <array>
#include <random>
#include "windows.h"
#include <unordered_map>

#define SIZE 4

namespace Tmpl8
{
	std::random_device rd;
	std::mt19937 gen(rd());
	
	
	using Grid = std::array<std::array<int, SIZE>, SIZE>;
	using Line = std::array<int, SIZE>;

	//method that prints the grid on the console
	void printGrid(Grid grid);
	//function that add a cell/number over the grid
	bool generateNewCell(Grid* grid);
	//function that handle the movement of the grid based on the user input
	bool moveGrid(Grid* grid, char c);
	//default function used to move each line to the left
	Line moveGridToLeft(Line line);
	//function that remove all the '0' from the line and adds them at the end
	Line compactLine(Line line);
	//function that sums the series of same numbers in the same line
	Line sumLine(Line line);
	//function that revert the given line
	Line invertLine(Line line);

	//method that print the grid on the screen with graphics
	void drawGrid(const Grid& grid, Surface* screen);
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	Grid grid = { 0 }; //grid[row][column]

	//variable that control if the grid has to be refreshed or not
	bool gridToUpdate;
	//user input
	char input = ' ';
	bool winCondition;
	
	std::unordered_map<int, Sprite*> tileSprites; //it's like a dictionary
	//in this scenario it is really useful since grid values are number, and with std::undordered_map
	//is possible to get values not based on index but on "keys" that in this case are the number of the nubmers of the grid

	void Game::Init()
	{
		tileSprites[0] = new Sprite(new Surface("assets/voidTile.png"), 1); //this is the void tile
		tileSprites[2] = new Sprite(new Surface("assets/2 Tile.png"), 1);
		tileSprites[4] = new Sprite(new Surface("assets/4 Tile.png"), 1);
		tileSprites[8] = new Sprite(new Surface("assets/8 Tile.png"), 1);
		tileSprites[16] = new Sprite(new Surface("assets/16 Tile.png"), 1);
		tileSprites[32] = new Sprite(new Surface("assets/32 Tile.png"), 1);
		tileSprites[64] = new Sprite(new Surface("assets/64 Tile.png"), 1);
		tileSprites[128] = new Sprite(new Surface("assets/128 Tile.png"), 1);
		tileSprites[256] = new Sprite(new Surface("assets/256 Tile.png"), 1);
		tileSprites[512] = new Sprite(new Surface("assets/512 Tile.png"), 1);
		tileSprites[1024] = new Sprite(new Surface("assets/1024 Tile.png"), 1);
		tileSprites[2048] = new Sprite(new Surface("assets/2048 Tile.png"), 1);
		tileSprites[4096] = new Sprite(new Surface("assets/4096 Tile.png"), 1);
		tileSprites[8192] = new Sprite(new Surface("assets/8192 Tile.png"), 1);
		tileSprites[16384] = new Sprite(new Surface("assets/16384 Tile.png"), 1);
		tileSprites[32768] = new Sprite(new Surface("assets/32768 Tile.png"), 1);
		tileSprites[65536] = new Sprite(new Surface("assets/65536 Tile.png"), 1);
		gridToUpdate = true;
		winCondition = false;
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}
	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		if (winCondition) {
			printf("YOU WON!!!\n");
			grid = { 0 };
			winCondition = false;
			gridToUpdate = true;
		}

		if (gridToUpdate) {
			if (!generateNewCell(&grid)) //if there's no space to generate a new cell it means you're out of moves, so you lost
				exit(0);
			printGrid(grid);
			gridToUpdate = false;
		}

		//last input is used in order to prevent user from holding key issues
		char lastInput = input;
		input = ' ';
		if (GetAsyncKeyState('A')) input = 'a';
		if (GetAsyncKeyState('D')) input = 'd';
		if (GetAsyncKeyState('W')) input = 'w';
		if (GetAsyncKeyState('S')) input = 's';

		if (lastInput != input) {
			gridToUpdate = moveGrid(&grid, input);
		}

		screen->Clear(0);
		drawGrid(grid, screen);
	}

	void drawGrid(const Grid& grid, Surface* screen)
	{
		const int tileSize = 128; // regola in base alle immagini

		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				int value = grid[i][j];
				//takes the right sprite based on the value
				Sprite* tile = tileSprites[value];
				if (value == 2048) {
					winCondition = true;
				}

				tile->Draw(screen, tileSize * j, tileSize * i);
			}
		}
	}


	bool moveGrid(Grid* grid, char c) {
		bool moved = false;
		//it just move the grid based on the input using the before declared functions adapted to each case
		switch (c)
		{
		case 'a':
			{
				//a means left so you just need to move it to the left then sum
				for (int i = 0; i < SIZE; i++) {
					Line newLine = moveGridToLeft((*grid)[i]);
					if ((*grid)[i] != newLine) moved = true;
					(*grid)[i] = newLine;
				}
			}
			break;
		case 'd':
			{
				//d means right so it's the same as left but you have to first invert the line
				//then move it to the left and sum and finally re-invert the line
				for (int i = 0; i < SIZE; i++) {
					Line newLine = invertLine(moveGridToLeft(invertLine((*grid)[i])));
					if ((*grid)[i] != newLine) moved = true;
					(*grid)[i] = newLine;
				}
			}
			break;
		case 'w':
			{
				//w means up so it is just needed to take the column and treat it like a line
				//and move to the left and sum and finally insert in the grid as a column
				//that's the reason for the second for loop
				for (int i = 0; i < SIZE; i++) {
					Line line = { (*grid)[0][i], (*grid)[1][i], (*grid)[2][i], (*grid)[3][i] };
					Line newLine = moveGridToLeft(line);
					for (int j = 0; j < SIZE; j++) {
						if ((*grid)[j][i] != newLine[j]) moved = true;
						(*grid)[j][i] = newLine[j];
					}
				}
			}
			break;
		case 's':
			{
				//s means down so it is just needed to take the column and treat it like a line
				//pretending to move it to the right, so the logic it's the same:
				//invert, move to the left and sum, re-invert
				//finally insert in the grid as a column
				//that's the reason for the second for loop
				for (int i = 0; i < SIZE; i++) {
					Line line = { (*grid)[0][i], (*grid)[1][i], (*grid)[2][i], (*grid)[3][i] };
					Line newLine = invertLine(moveGridToLeft(invertLine(line)));
					for (int j = 0; j < SIZE; j++) { 
						if ((*grid)[j][i] != newLine[j]) moved = true;
						(*grid)[j][i] = newLine[j];
					}
				}
			}
			break;
		default:
			break;
		}

		return moved;
	}

	//simply invert the line
	Line invertLine(Line line) {
		Line lineR = { 0 };
		for (int i = 0; i<SIZE; i++) {
			lineR[SIZE -1 - i] = line[i];
		}
		return lineR;
	}
	
	//it compact the line to the "left"/start and leaves 0 at the end
	Line compactLine(Line line) {
		Line l = { 0 };
		int j = 0;
		for (int i = 0; i < SIZE; i++) {
			if (line[i] != 0) {
				l[j] = line[i];
				j++;
			}
		}
		return l;
	}

	//it sum adiacent cells with the same value
	Line sumLine(Line line) {
		for (int i = 0; i < SIZE-1; i++) {
			if (line[i] == line[i + 1]) {
				line[i] += line[i + 1];
				line[i + 1] = 0;
			}
		}
		return line;
	}

	Line moveGridToLeft(Line line){
		Line orgLine = line;
		line = compactLine(line);
		line = sumLine(line);
		line = compactLine(line);

		return line;
	}
	
	//function that generate a new cell/value in a free slot
	bool generateNewCell(Grid* grid) {
		int row = 0, col = 0, number = -1, count = 0;
		double r;
		//the generation it's actually wrong because there's a chance to lose even
		//if you don't actually lose because the code doesn't check for availavable slot
		//it just return false if it doens't find it in less than some iterations
		//so i know it's wrong but since it's just a project to start using matrix i won't implement it
		//even though implementing it is actually easy
		std::uniform_int_distribution<> distIndex(0, 3);
		do {
			row = distIndex(gen);
			col = distIndex(gen);
			count++;
			if (count > SIZE * SIZE * SIZE * SIZE) return false;
		} while ((*grid)[row][col] != 0);

		std::uniform_real_distribution<> distValue(0.0, 1.0);
		r = distValue(gen);
		
		if (r < 0.9) number = 2;
		else number = 4;

		(*grid)[row][col] = number;

		gridToUpdate = true;

		return true;


	}

	void printGrid(Grid grid) {
		//system("cls");
		printf("----2048----\n");
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				printf("%d ", grid[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}
};