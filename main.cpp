#include <SDL.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

//

const unsigned delay_ms = 50;

//
constexpr int SCREEN_WIDTH = 714;
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
float RENDER_BRIGHTNESS_FACTOR = 1000;

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

int get_i_cycle_array(int i) {
	if (i < 0) {
		//return map_side_size - 1;
		return 25;
	}
	if (i >= map_side_size) {
		//return 0;
		return 25;
	}
	return i;
}
int get_j_cycle_array(int j) {
	if (j < 0) {
		//return map_side_size - 1;
		return 25;
	}
	if (j >= map_side_size) {
		//return 0;
		return 25;
	}
	return j;
}

void energy_physics() {
	//Light wave one iter
	for (int i = 0; i < map_side_size; ++i)
	{
		for (int j = 0; j < map_side_size; ++j)
		{
			//if (i > 0 && j > 0 && i < map_side_size - 1 && j < map_side_size - 1)
			{
				unsigned divided_value = light_map[i][j] / 5;

				if (!solid_map[get_i_cycle_array(i - 1)][j])
				{
					light_map_buffer[get_i_cycle_array(i - 1)][j] += divided_value;
				}
				else
				{
					light_map_buffer[i][j] += divided_value;
				}

				if (!solid_map[get_i_cycle_array(i + 1)][j])
				{
					light_map_buffer[get_i_cycle_array(i + 1)][j] += divided_value;
				}
				else
				{
					light_map_buffer[i][j] += divided_value;
				}

				if (!solid_map[i][get_j_cycle_array(j - 1)])
				{
					light_map_buffer[i][get_j_cycle_array(j - 1)] += divided_value;
				}
				else
				{
					light_map_buffer[i][j] += divided_value;
				}

				if (!solid_map[i][get_j_cycle_array(j + 1)])
				{
					light_map_buffer[i][get_j_cycle_array(j + 1)] += divided_value;
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
		}
	}

	// Copy light_map_buffer to light_map and clear
	for (int i = 0; i < map_side_size; ++i)
	{
		for (int j = 0; j < map_side_size; ++j)
		{
			//if (i > 0 && j > 0 && i < map_side_size - 1 && j < map_side_size - 1)
			{
				light_map[i][j] = light_map_buffer[i][j];
				light_map_buffer[i][j] = 0;
			}
		}
	}

	////// Test (Lost Value To Center)
	/*unsigned summ_finish = 0;
	for (const auto& row : light_map)
	{
		for (const auto& col : row)
		{
			summ_finish += col;
		}
	}	
	light_map[25][25] += summ_start - summ_finish;*/
	////////////

	//Summ of all cells value
	unsigned summ_finish = 0;
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

void draw_grid(bool is_on, SDL_Renderer* renderer) {
	if (is_on) {
		SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE);
		for (int w = -1; w <= SCREEN_WIDTH; w += GRID_SHIFT) {
			SDL_RenderDrawLine(renderer, w, 0, w, SCREEN_HEIGHT);
			SDL_RenderDrawLine(renderer, w + 1, 0, w + 1, SCREEN_HEIGHT);
		}
		for (int h = -1; h <= SCREEN_HEIGHT; h += GRID_SHIFT) {
			SDL_RenderDrawLine(renderer, 0, h, SCREEN_WIDTH, h);
			SDL_RenderDrawLine(renderer, 0, h + 1, SCREEN_WIDTH, h + 1);
		}
	}	
}

void draw_test_cells(bool is_on, SDL_Renderer* renderer) {
	if (is_on) {
		SDL_SetRenderDrawColor(renderer, 150, 150, 150, SDL_ALPHA_OPAQUE);
		SDL_Rect cells[GRID_SIZE][GRID_SIZE];
		for (int i = 0; i < GRID_SIZE; ++i) {
			for (int j = 0; j < GRID_SIZE; ++j) {
				if (i == j)
				{
					SDL_Rect rect{ i * GRID_SHIFT, j * GRID_SHIFT, GRID_SHIFT, GRID_SHIFT };
					SDL_RenderFillRect(renderer, &rect);
				}

			}
		}
	}	
}

int main(int argc, char* argv[]) {
	//Set light on light_map
	//light_map[8][6] = 256;
	//light_map[7][10] = 256;
	//light_map[24][24] = MAX_LIGHT_STRENGTH;
	light_map[25][25] = MAX_LIGHT_STRENGTH;
	//light_map[24][25] = MAX_LIGHT_STRENGTH;
	//light_map[25][24] = MAX_LIGHT_STRENGTH;
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
		energy_physics();

		//Draw
		draw_cells(renderer);

		// Draw Test Cells
		draw_test_cells(false, renderer);

		// Draw Grid
		draw_grid(true, renderer);

		//////////////////////////////////
				
		// Finally show what we've drawn
		SDL_RenderPresent(renderer);

		SDL_Delay(delay_ms);
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
