//
// sdl/imgui
#include <SDL.h>
#include <SDL_image.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <string>

//const int DEFAULT_COUNT = 500;
const int DEFAULT_COUNT = 1500;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Globals because this just a demo project to implement a QuadTree.
SDL_Renderer* renderer;
SDL_Window* window;
ImGuiIO io; // idk what this is for rn, but imgui needs it

SDL_Texture* playerTexture;

int setup();
void gameLoop();
void showImGui();

// Main code
int main(int, char**)
{
    // Sets up SDL and imgui
    int result = setup();
    if (result == 0) {
        printf("All setup! :)");
    }
    else {
        printf("Oh noooo");
    }

    playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    if (playerTexture == NULL) {
        printf("Couldn't load player texture. %s", IMG_GetError());
    }

    gameLoop();

    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// @refactor: put rendering into separate rendering module, START ORGANIZING YOUR SHIT
void gameLoop() {

    int quit = false;
    SDL_Event event;

    while (!quit) {
        // capture inputs
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
                quit = true; // idk why this is here but it was in the imgui example so will leave it
            }
        }

        // simulate gamestate

        // render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect rect = { 250, 250, 150, 150 };
        SDL_RenderCopyEx(renderer, playerTexture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);

        showImGui();

        SDL_RenderPresent(renderer);
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
    window = SDL_CreateWindow("QuadTree Collision Detection demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
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

    return 0;
}



