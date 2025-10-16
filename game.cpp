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

	void printGrid(Grid grid);
	bool generateNewCell(Grid* grid);
	bool moveGrid(Grid* grid, char c);
	Line moveGridToLeft(Line line);
	Line compactLine(Line line);
	Line sumLine(Line line);
	Line invertLine(Line line);

	void drawGrid(const Grid& grid, Surface* screen);
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	Grid grid = { 0 }; //grid[row][column]

	bool gridToUpdate;
	char input = ' ';
	bool winCondition;
	
	std::unordered_map<int, Sprite*> tileSprites;

	void Game::Init()
	{
		tileSprites[0] = new Sprite(new Surface("assets/voidTile.png"), 1); // o "voidTile.png"
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
		//generateNewCell(&grid);
		//Line l = { 0,2,4,0 };
		//moveGridToLeft(l);
		//invertLine(l);
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
			if (!generateNewCell(&grid))
				exit(0);
			printGrid(grid);
			gridToUpdate = false;
		}
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
				Sprite* tile = tileSprites.count(value) ? tileSprites[value] : tileSprites[0];
				if (value == 64) {
					winCondition = true;
				}

				tile->Draw(screen, tileSize * j, tileSize * i);
			}
		}
	}


	bool moveGrid(Grid* grid, char c) {
		bool moved = false;
		switch (c)
		{
		case 'a':
			{
				for (int i = 0; i < SIZE; i++) {
					Line newLine = moveGridToLeft((*grid)[i]);
					if ((*grid)[i] != newLine) moved = true;
					(*grid)[i] = newLine;
				}
			}
			break;
		case 'd':
			{
				for (int i = 0; i < SIZE; i++) {
					//Line revLine = invertLine((*grid)[i]);
					//Line newRevLine = moveGridToLeft(revLine);
					//Line newLine = invertLine(newRevLine);
					Line newLine = invertLine(moveGridToLeft(invertLine((*grid)[i])));
					if ((*grid)[i] != newLine) moved = true;
					(*grid)[i] = newLine;
				}
			}
			break;
		case 'w':
			{
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
				for (int i = 0; i < SIZE; i++) {
					Line line = { (*grid)[0][i], (*grid)[1][i], (*grid)[2][i], (*grid)[3][i] };
					//Line revLine = invertLine(line);
					//Line newRevLine = moveGridToLeft(revLine);
					//Line newLine = invertLine(newRevLine);
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

	Line invertLine(Line line) {
		Line lineR = { 0 };
		for (int i = 0; i<SIZE; i++) {
			lineR[SIZE -1 - i] = line[i];
		}
		return lineR;
	}
	
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

	bool generateNewCell(Grid* grid) {
		int row = 0, col = 0, number = -1, count = 0;
		double r;
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
		system("cls");
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