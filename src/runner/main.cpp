#include <SDL3/SDL.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#ifdef WIN32
#include <spdlog/sinks/msvc_sink.h>	
#endif

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "imgui_internal.h"

#include "MainApp.h"

void LogInit(const char *fileName)
{	
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(fileName, true));

#ifdef WIN32
	sinks.push_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());
#endif		
	auto combined_logger = std::make_shared<spdlog::logger>("litecpu_runner", begin(sinks), end(sinks));

	//register it if you need to access it globally
	spdlog::register_logger(combined_logger);

	combined_logger->set_level(spdlog::level::trace); // Set specific logger's log level		

	// Customize msg format for all loggers
	spdlog::set_pattern("[%T] [%^-%L-%$] [T %t] %v");

	combined_logger->flush_on(spdlog::level::err);

	combined_logger->info("Log started");

#ifdef WIN32
	// can be set globally or per logger(logger->set_error_handler(..))
	spdlog::set_error_handler([](const std::string &msg) { spdlog::get("console")->error("*** LOGGER ERROR ***: {}", msg); });
#endif

	spdlog::set_default_logger(combined_logger);
}

static bool g_fExitRequested = false;

int main()
{
	LogInit("LiteCPURunner.log");

	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMEPAD) != 0)
	{
		spdlog::critical("[Runner] Error: SDL_Init(): %s\n", SDL_GetError());
		return -1;
	}

	// Enable native IME.
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
	SDL_Window *window = SDL_CreateWindow("LitePanelSDL", 1280, 720, window_flags);
	if (window == nullptr)
	{
		spdlog::critical("[Runner] Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		spdlog::critical("[Runner] Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
		return -1;
	}

	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = nullptr;

	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	MainApp app;

	while (!g_fExitRequested)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{			
			ImGui_ImplSDL3_ProcessEvent(&event);

			if (event.type == SDL_EVENT_QUIT)
				g_fExitRequested = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				g_fExitRequested = true;

			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				app.HandleEvent(event.key);
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		
		if (!app.Display())
			g_fExitRequested = true;

		app.Run();

		// Rendering
		ImGui::Render();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		SDL_RenderClear(renderer);

		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
