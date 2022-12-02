#include <SDL.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

//
constexpr int SCREEN_WIDTH = 700;
constexpr int SCREEN_HEIGHT = SCREEN_WIDTH;

constexpr int GRID_SIZE = 51;
constexpr int GRID_SHIFT = SCREEN_WIDTH / GRID_SIZE;

//
const unsigned MAX_LIGHT_STRENGTH = 10000;

const int map_side_size = GRID_SIZE;

unsigned light_map[map_side_size][map_side_size] = {};
unsigned light_map_buffer[map_side_size][map_side_size] = {};

bool solid_map[map_side_size][map_side_size] = {};

//
float RENDER_BRIGHTNESS_FACTOR = 600;

//
unsigned summ_start = 0;

void draw_cells(SDL_Renderer* renderer) {
	SDL_Rect rect;
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			if (light_map[i][j] || solid_map[i][j]) {
				if (light_map[i][j]) {
					unsigned light_to_rgb = float(light_map[i][j]) / MAX_LIGHT_STRENGTH * 255 * RENDER_BRIGHTNESS_FACTOR;
					SDL_SetRenderDrawColor(renderer, light_to_rgb, light_to_rgb, 0, SDL_ALPHA_OPAQUE);
				}
				if (solid_map[i][j]) {
					SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
				}

				rect.x = i * GRID_SHIFT;
				rect.y = j * GRID_SHIFT;
				rect.w = GRID_SHIFT;
				rect.h = GRID_SHIFT;

				SDL_RenderFillRect(renderer, &rect);
			}			
		}
	}	
}

void lightPhysics() {
	//Settings
	const unsigned ITERATION_AMOUNT = 1;

	//light_map declar
	/*const int map_side_size = 16;

	unsigned light_map[map_side_size][map_side_size] = {};
	unsigned light_map_buffer[map_side_size][map_side_size] = {};

	bool solid_map[map_side_size][map_side_size] = {};*/

	////Set light on light_map
	////light_map[8][6] = 256;
	////light_map[7][10] = 256;
	//light_map[50][50] = 5;
	////light_map[70][50] = 100;

	////Set Solid on solid light_map
	//solid_map[7][8] = true;
	//solid_map[6][8] = true;
	//solid_map[6][9] = true;
	//solid_map[6][10] = true;
	//solid_map[6][11] = true;
	//solid_map[6][12] = true;
	//solid_map[7][11] = true;
	//solid_map[8][11] = true;

	////Summ of all cells value
	//unsigned summ_start = 0;
	//for (const auto& row : light_map)
	//{
	//	for (const auto& col : row)
	//	{
	//		summ_start += col;
	//	}
	//}

	// Main
	for (unsigned iter = 0; iter < ITERATION_AMOUNT; ++iter)
	{
		////Draw
		//for (auto i = 0; i < map_side_size; ++i)
		//{
		//	for (auto j = 0; j < map_side_size; ++j)
		//	{
		//		if (!solid_map[i][j]) {
		//			if (light_map[i][j] == 0)
		//			{
		//				cout << "___";
		//			}
		//			else
		//			{

		//				if (light_map[i][j] < 100)
		//				{
		//					cout << "0";
		//				}
		//				if (light_map[i][j] < 10)
		//				{
		//					cout << "0";
		//				}
		//				cout << light_map[i][j];

		//			}
		//		}
		//		else
		//		{
		//			cout << "###";
		//		}
		//		cout << " ";
		//	}
		//	cout << endl << endl;
		//}
		//cout << endl << endl << endl;

		//Light wave one iter
		for (auto i = 0; i < map_side_size; ++i)
		{
			for (auto j = 0; j < map_side_size; ++j)
			{
				if (i > 0 && j > 0 && i < map_side_size - 1 && j < map_side_size - 1)
				{
					//if (!solid_map[i][j])
					{
						//if (light_map[i][j] > 0)
						{
							//if (light_map[i][j] / 5 > 0)
							{
								unsigned divided_value = light_map[i][j] / 5;

								if (!solid_map[i - 1][j])
								{
									light_map_buffer[i - 1][j] += divided_value;
								}
								else
								{
									light_map_buffer[i][j] += divided_value;
								}

								if (!solid_map[i + 1][j])
								{
									light_map_buffer[i + 1][j] += divided_value;
								}
								else
								{
									light_map_buffer[i][j] += divided_value;
								}

								if (!solid_map[i][j - 1])
								{
									light_map_buffer[i][j - 1] += divided_value;
								}
								else
								{
									light_map_buffer[i][j] += divided_value;
								}

								if (!solid_map[i][j + 1])
								{
									light_map_buffer[i][j + 1] += divided_value;
								}
								else
								{
									light_map_buffer[i][j] += divided_value;
								}

								//
								light_map_buffer[i][j] += divided_value;

								//
								light_map_buffer[i][j] += light_map[i][j] % 5;
							}
							// else
							// {
							//     light_map_buffer[i][j] += light_map[i][j];
							// }
						}
					}
				}
			}
		}

		// Copy light_map_buffer to light_map and clear
		for (auto i = 0; i < map_side_size; ++i)
		{
			for (auto j = 0; j < map_side_size; ++j)
			{
				if (i > 0 && j > 0 && i < map_side_size - 1 && j < map_side_size - 1)
				{
					light_map[i][j] = light_map_buffer[i][j];
					light_map_buffer[i][j] = 0;
				}
			}
		}
	}

	////// Test (Lost Value To Center)
	unsigned summ_finish = 0;
	for (const auto& row : light_map)
	{
		for (const auto& col : row)
		{
			summ_finish += col;
		}
	}	
	light_map[25][25] += summ_start - summ_finish;
	////////////

	//Summ of all cells value
	summ_finish = 0;
	for (const auto& row : light_map)
	{
		for (const auto& col : row)
		{
			summ_finish += col;
		}
	}

	if (summ_start - summ_finish != 0) {
		//Print summ
		//cout << "Finish summ of all cells value: " << summ_start << endl;
		//cout << "Finish summ of all cells value: " << summ_finish << endl;
	}
	//Value lost
	cout << "Value lost: " << summ_start - summ_finish << endl;


	
}

int main(int argc, char* argv[]) {
	//Set light on light_map
	//light_map[8][6] = 256;
	//light_map[7][10] = 256;
	light_map[25][25] = MAX_LIGHT_STRENGTH;
	//light_map[60][56] = MAX_LIGHT_STRENGTH / 5;
	//light_map[70][80] = MAX_LIGHT_STRENGTH / 5;

	//Set Solid on solid light_map
	/*solid_map[50][60] = true;
	solid_map[51][60] = true;
	solid_map[52][60] = true;
	solid_map[53][60] = true;
	solid_map[54][60] = true;
	solid_map[55][60] = true;
	solid_map[56][60] = true;
	solid_map[57][60] = true;
	solid_map[58][60] = true;
	solid_map[59][60] = true;
	solid_map[60][60] = true;
	solid_map[61][60] = true;
	solid_map[62][60] = true;
	solid_map[63][60] = true;
	solid_map[64][60] = true;
	solid_map[65][60] = true;
	solid_map[66][60] = true;
	solid_map[67][60] = true;
	solid_map[68][60] = true;
	solid_map[69][60] = true;
	solid_map[70][60] = true;
	solid_map[71][60] = true;
	solid_map[72][60] = true;
	solid_map[73][60] = true;*/

	//////////////////////////

	//Summ of all cells value
	for (const auto& row : light_map)
	{
		for (const auto& col : row)
		{
			summ_start += col;
		}
	}

	/////////////////////////////////////////////////////////

	// Create a window data type
	// This pointer will point to the 
	// window that is allocated from SDL_CreateWindow
	SDL_Window* window = nullptr;

	// Initialize the video subsystem.
	// iF it returns less than 1, then an
	// error code will be received.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not be initialized: " <<
			SDL_GetError();
	}
	else {
		std::cout << "SDL video system is ready to go\n";
	}
	// Request a window to be created for our platform
	// The parameters are for the title, x and y position,
	// and the width and height of the window.
	window = SDL_CreateWindow("Energy Physics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = nullptr;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Infinite loop for our application
	bool gameIsRunning = true;
	// Main application loop
	while (gameIsRunning) {
		SDL_Event event;

		// (1) Handle Input
		// Start our event loop
		while (SDL_PollEvent(&event)) {
			// Handle each specific event
			if (event.type == SDL_QUIT) {
				gameIsRunning = false;
			}
		}
		// (2) Handle Updates

		// (3) Clear and Draw the Screen
		// Gives us a clear "canvas"
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//////////////////////////////////

		//Physics
		lightPhysics();

		//Draw
		draw_cells(renderer);


		//Draw Cells
		/*SDL_SetRenderDrawColor(renderer, 150, 150, 150, SDL_ALPHA_OPAQUE);
		SDL_Rect cells[GRID_SIZE][GRID_SIZE];
		for (int i = 0; i < SCREEN_WIDTH; i += GRID_SHIFT) {
			for (int j = 0; j < SCREEN_HEIGHT; j += GRID_SHIFT) {
				SDL_Rect rect{ i, j, i + GRID_SHIFT, j + GRID_SHIFT };
				SDL_RenderFillRect(renderer, &rect);
			}
		}*/

		// Draw Rect
		//SDL_SetRenderDrawColor(renderer, 150, 150, 150, SDL_ALPHA_OPAQUE);
		//SDL_Rect rect{0, 0, GRID_SHIFT, GRID_SHIFT};
		//SDL_RenderFillRect(renderer, &rect);

		// Draw Grid
		/*SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		for (int w = -1; w <= SCREEN_WIDTH; w += GRID_SHIFT) {
			SDL_RenderDrawLine(renderer, w, 0, w, SCREEN_HEIGHT);
			SDL_RenderDrawLine(renderer, w + 1, 0, w + 1, SCREEN_HEIGHT);
		}
		for (int h = -1; h <= SCREEN_HEIGHT; h += GRID_SHIFT) {
			SDL_RenderDrawLine(renderer, 0, h, SCREEN_WIDTH, h);
			SDL_RenderDrawLine(renderer, 0, h + 1, SCREEN_WIDTH, h + 1);
		}*/

		//////////////////////////////////
				
		// Finally show what we've drawn
		SDL_RenderPresent(renderer);

		SDL_Delay(50);
	}

	// We destroy our window. We are passing in the pointer
	// that points to the memory allocated by the 
	// 'SDL_CreateWindow' function. Remember, this is
	// a 'C-style' API, we don't have destructors.
	SDL_DestroyWindow(window);

	// our program.
	SDL_Quit();
	return 0;
}
