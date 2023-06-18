//
// sdl/imgui
#include <SDL.h>
#include <SDL_image.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <string>

#include "InputManager.h"
#include "Renderer.h"
#include "Player.h"
#include "Direction.h"

//const int DEFAULT_COUNT = 500;
const int DEFAULT_COUNT = 1500;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Globals because this just a demo project to implement a QuadTree.
//SDL_Renderer* renderer;
//SDL_Window* window;
ImGuiIO io; // idk what this is for rn, but imgui needs it

Player player = Player(SDL_FRect{250, 250, 50, 50});
InputManager inputManager = InputManager(&player);
Renderer* renderManager;

int setup();
void gameLoop();
void showImGui();

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
    renderManager->setup();

    gameLoop();

    // Cleanup imgui
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // TODO: check that this is working.
    renderManager->~Renderer();

    return 0;
}

// @refactor: put rendering into separate rendering module, START ORGANIZING YOUR SHIT
void gameLoop() {

    int quit = false;
    SDL_Event event;
    std::vector<SDL_Event> events;
    Uint32 lastPhysicsUpdate = 0;
    float dt = 0;

    while (!quit) {
        // capture inputs
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            inputManager.handleEvent(event);
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // simulate gamestate
        dt = (SDL_GetTicks() - lastPhysicsUpdate) / 1000.f;
        player.simulate(dt);
        lastPhysicsUpdate = SDL_GetTicks();
        // NOTE: when it gets more complicated than a player object, then it should 
        // go into a GameState manager thing instead.

        // Rendering
        renderManager->renderPlayer(&player);
        showImGui();
        renderManager->renderPresent();
    }
}

// Parameters are all the stuff ImGui needs to display not in scope.
void showImGui() {
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //bool show = true;
    //ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Info");
    ImGui::Text("Test text");
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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 1;
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);

    // SDL_Image
    int imgFlags = IMG_INIT_PNG;
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
    renderManager = new Renderer(window, renderer);

    return 0;
}



