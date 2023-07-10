//
// sdl/imgui
#include <SDL.h>
#include <SDL_image.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <string>

#include "GameState.h"
#include "InputManager.h"
#include "Renderer.h"
#include "Player.h"
#include "Direction.h"

//const int DEFAULT_COUNT = 500;
const int DEFAULT_COUNT = 1500;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
//const int WINDOW_WIDTH = 720;
//const int WINDOW_HEIGHT = 480;
Uint32 countedFrames = 0;
int fpsCap = 60;
Uint32 frameTimeToComplete = -1;

ImGuiIO io; // idk what this is for rn, but imgui needs it

extern Player player; // from gamestate

int setup();
void gameLoop();
void showImGui();

void gameLoop() {

	int quit = false;
	SDL_Event event;
	std::vector<SDL_Event> events;
	Uint32 lastPhysicsUpdate = 0;
	//float dt = 0;

	Uint32 frameStart = 0;

	while (!quit) {
		frameStart = SDL_GetTicks();

		// capture user inputs
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			//inputManager.handleEvent(event);
			handleEvent(&event, &player);
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		// Simulate entire gamestate
		simulateWorld();

		// Render current gamestate
		renderGameState(&player);
		showImGui();
		showBackbufferClear();

		// Finished rendering, cap framerate.
		// If frame is finished early, wait remaining time.
		frameTimeToComplete = SDL_GetTicks() - frameStart;
		if (1000 / fpsCap > frameTimeToComplete) {
			SDL_Delay((1000 / fpsCap) - frameTimeToComplete);
		}
		//assert(1000 / fpsCap > frameTimeToComplete);
		if (!(1000 / fpsCap > frameTimeToComplete)) {
			printf("DID NOT FINISH IN TIME\n");
		}

	}
}

// Main code
int main(int, char**)
{
	// Sets up SDL and imgui
	int result = setup();
	if (result != 0) {
		printf("%i", result);
		return result;
	}

	// Loads assets to be ready for rendering
	//renderManager->setup();

	gameLoop();

	// Cleanup imgui
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// TODO: check that this is working.
	//renderManager->~Renderer();

	return 0;
}

// Parameters are all the stuff ImGui needs to display not in scope.
void showImGui() {
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//bool show = true;
	//ImGui::ShowDemoWindow(&show);
	float secondsSinceStart = (SDL_GetTicks() / 1000.f);
	float fps = countedFrames / secondsSinceStart;

	ImGui::Begin("Info");
	ImGui::Text("Counted frames");
	ImGui::Text(std::to_string(countedFrames).c_str());
	if (countedFrames > 0) {
		ImGui::Text("avg fps");
		ImGui::Text(std::to_string(fps).c_str());
	}
	ImGui::Text("Enemy count:");
	ImGui::Text(std::to_string(getEnemies().size()).c_str());
	ImGui::Text("Rendered enemies this frame:");
	ImGui::Text(std::to_string(renderedEnemiesThisFrame).c_str());
	ImGui::SliderInt("fps cap:", &fpsCap, 5, 120);
	ImGui::Text("Ticks to complete frame:");
	ImGui::Text(std::to_string(frameTimeToComplete).c_str());
	ImGui::Text("How long it can take w/ current framerate cap:");
	ImGui::Text(std::to_string(1000.f / fpsCap).c_str());
	ImGui::End();

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

/* Setup required for sdl and imgui.
 * Mostly just ripped from the example imgui provides.
 */
int setup() {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("QuadTree Collision Detection demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_Log("Error creating SDL_Renderer!");
		return 1;
	}
	//SDL_RendererInfo info;
	//SDL_GetRendererInfo(renderer, &info);
	//SDL_Log("Current SDL_Renderer: %s", info.name);

	// SDL_Image
	int imgFlags = IMG_INIT_PNG || IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image Error: %s\n", IMG_GetError());
		return 2;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);

	// Render manager will hold pointers to these. It will handle
	// cleaning itself up.
	//renderManager = new Renderer(window, renderer);
	setupGameState();
	setupRenderer(renderer, window);

	return 0;
}



