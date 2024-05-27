//Standard includes
#include <iostream>

//External includes
#ifdef _DEBUG
#include "vld.h"
#endif // DEBUG

// SDL includes
#include <memory>
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Project includes
#include "Timer.h"
#include "Renderer.h"

// framework design from Graphics programming lessons //
int main()
{
	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	constexpr uint32_t width{ 720 };
	constexpr uint32_t height{ 480 };

	SDL_Window* pWindow
	{
		SDL_CreateWindow
		(
			"GPP_Research - **Maurice Vandenheede** - 2DAE18N",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, height, 0
		)
	};

	if (!pWindow) return 1;

	//Initialize "framework"
	std::unique_ptr<Timer> pTimer{ std::make_unique<Timer>() };
	std::unique_ptr<Renderer> pRenderer{ std::make_unique<Renderer>(pWindow, width, height) };

	//Start loop
	pTimer->Start();

	float printTimer{};
	bool showFPS{ true };
	bool isLooping{ true };
	bool takeScreenshot{ false };

	SDL_Event e;

	while (isLooping)
	{
		//--------- Get input events ---------//
		while (SDL_PollEvent(&e)) // go over all events
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_MOUSEWHEEL:
				pRenderer->ScrollWheelInput(e.wheel.preciseY);
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					isLooping = false;
					break;

				case SDL_SCANCODE_N:
					pRenderer->NewMaps();
					break;

				case SDL_SCANCODE_S:
					pRenderer->ChangeMapSeed();
					break;

				case SDL_SCANCODE_R:
					pRenderer->ToggleRenderModes();
					break;

				case SDL_SCANCODE_Z:
					pRenderer->ShowOnlyOneToggle();
					break;

				case SDL_SCANCODE_Q:
					pRenderer->AddLayer();
					break;

				case SDL_SCANCODE_X:
					takeScreenshot = true;
					break;

				case SDL_SCANCODE_F:
					showFPS = !showFPS;
					break;
				}
				break;
			}
		}

		//--------- Update ---------
		pRenderer->Update(pTimer->GetElapsed());

		//--------- Render ---------
		pRenderer->Render();

		//--------- Timer ---------
		pTimer->Update();

		// fps
		if (showFPS)
		{
			printTimer += pTimer->GetElapsed();
			if (printTimer >= 1.f)
			{
				printTimer = 0.f;
				std::cout << "dFPS: " << pTimer->GetdFPS() << "\n";
			}
		}

		//Save screenshot
		if (takeScreenshot)
		{
			takeScreenshot = false;
			if (pRenderer->SaveBufferToImage())
			{
				std::cout << "Something went wrong. Screenshot not saved!" << "\n";
			}
			else
			{
				std::cout << "Screenshot saved!" << "\n";
			}
		}
	}

	pTimer->Stop();

	SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return 0;
}