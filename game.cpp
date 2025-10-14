#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <array>
#include <random>
#include "windows.h"

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
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	Grid grid = { 0 }; //grid[row][column]

	bool gridToUpdate;
	char input = ' ';
	

	void Game::Init()
	{
		gridToUpdate = true;
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
		if (gridToUpdate) {
			generateNewCell(&grid);
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

		//if (!generateNewCell(&grid) && gridToUpdate) {
		//	printf("addio");
		//	//exit(0);
		//}
		// clear the graphics window
		screen->Clear(0);
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
			if (count > SIZE * SIZE) return false;
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