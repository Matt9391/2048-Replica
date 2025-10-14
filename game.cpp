#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <array>
#include <random>

#define SIZE 4

namespace Tmpl8
{
	std::random_device rd;
	std::mt19937 gen(rd());
	

	using Grid = std::array<std::array<int, SIZE>, SIZE>;

	void printGrid(Grid grid);
	bool generateNewCell(Grid* grid);
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	Grid grid = { 0 }; //grid[row][column]

	bool gridToUpdate;

	

	void Game::Init()
	{
		gridToUpdate = true;
		generateNewCell(&grid);
		generateNewCell(&grid);
		generateNewCell(&grid);
		generateNewCell(&grid);
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
			printGrid(grid);
			gridToUpdate = false;
		}

		//if (!generateNewCell(&grid) && gridToUpdate) {
		//	printf("addio");
		//	//exit(0);
		//}
		// clear the graphics window
		screen->Clear(0);
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
		//system("cls");
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				printf("%d ", grid[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}
};