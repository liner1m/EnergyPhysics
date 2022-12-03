#include <SDL.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

//

const unsigned delay_ms = 5;

//
constexpr int SCREEN_WIDTH = 1005;
constexpr int SCREEN_HEIGHT = SCREEN_WIDTH;

constexpr int GRID_SIZE = 67;
constexpr int GRID_SHIFT = SCREEN_WIDTH / GRID_SIZE;

//
const unsigned MAX_LIGHT_STRENGTH = 20000;

const int map_side_size = GRID_SIZE;

unsigned light_map[map_side_size][map_side_size] = {};
unsigned light_map_buffer[map_side_size][map_side_size] = {};

bool solid_map[map_side_size][map_side_size] = {};

//
float RENDER_BRIGHTNESS_FACTOR = 500;

//
unsigned summ_start = 0;

int clamp(int value, const int min, const int max) {
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}

void draw_cells(SDL_Renderer* renderer) {
	SDL_Rect rect;
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			if (light_map[i][j] || solid_map[i][j]) {
				if (light_map[i][j]) {
					unsigned light_to_rgb = clamp(float(light_map[i][j]) / MAX_LIGHT_STRENGTH * 255 * RENDER_BRIGHTNESS_FACTOR, 0, 255);
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
		return map_side_size / 2;;
	}
	if (i >= map_side_size) {
		//return 0;
		return map_side_size / 2;;
	}
	return i;
}
int get_j_cycle_array(int j) {
	if (j < 0) {
		//return map_side_size - 1;
		return map_side_size / 2;
	}
	if (j >= map_side_size) {
		//return 0;
		return map_side_size / 2;
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
	//Energy lost
	cout << "Energy lost: " << summ_start - summ_finish << endl;
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
		unsigned light_to_rgb = clamp(255 * RENDER_BRIGHTNESS_FACTOR, 0, 255);
		SDL_SetRenderDrawColor(renderer, light_to_rgb, light_to_rgb, light_to_rgb, SDL_ALPHA_OPAQUE);
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
	light_map[map_side_size / 2][map_side_size / 2] = MAX_LIGHT_STRENGTH;
	//light_map[24][25] = MAX_LIGHT_STRENGTH;
	//light_map[25][24] = MAX_LIGHT_STRENGTH;
	//light_map[60][56] = MAX_LIGHT_STRENGTH / 5;
	//light_map[70][80] = MAX_LIGHT_STRENGTH / 5;

	//Set Solid on solid light_map
	//solid_map[20][50] = true;
	//solid_map[21][50] = true;
	//solid_map[22][50] = true;
	//solid_map[23][50] = true;
	//solid_map[24][50] = true;
	//solid_map[25][50] = true;
	//solid_map[26][50] = true;
	//solid_map[27][50] = true;
	//solid_map[28][50] = true;
	//solid_map[29][50] = true;
	//solid_map[30][50] = true;
	////solid_map[31][50] = true;
	//solid_map[32][50] = true;
	//solid_map[33][50] = true;
	//solid_map[34][50] = true;
	//solid_map[35][50] = true;
	//solid_map[36][50] = true;
	//solid_map[37][50] = true;
	//solid_map[38][50] = true;
	//solid_map[39][50] = true;
	//solid_map[40][50] = true;
	//solid_map[41][50] = true;
	//solid_map[42][50] = true;
	//solid_map[43][50] = true;

	for (unsigned int i = 0; i < GRID_SIZE; ++i)
	{
		solid_map[i][50] = true;
	}
	solid_map[33][50] = false;

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
			else
			{
				if (event.type == SDL_KEYDOWN) {
					switch (event.key.keysym.sym)
					{
					case SDLK_b:
						cout << "Enter brightness: ";
						cin >> RENDER_BRIGHTNESS_FACTOR;
						cout << endl;
						break;
					default:
						break;
					}
				}
				if (event.type == SDL_MOUSEWHEEL) {
					if (event.wheel.y > 0) {
						RENDER_BRIGHTNESS_FACTOR += 100;
						cout << "Brightness Factor: " << RENDER_BRIGHTNESS_FACTOR << endl;
					}
					if (event.wheel.y < 0) {
						RENDER_BRIGHTNESS_FACTOR -= 100;
						cout << "Brightness Factor: " << RENDER_BRIGHTNESS_FACTOR << endl;
					}
				}
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

		/// Test
		
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
