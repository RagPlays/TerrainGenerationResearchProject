
//External includes
#include <cassert>
#include <iostream>
#include <sstream>
#include "SDL.h"
#include "SDL_surface.h"

//Framework includes
#include "Renderer.h"
#include "MathHelpers.h"
#include "ColorRGB.h"
#include "Timer.h"

// Other includes
#include "TerrainMap.h"
#include "MapLayerManager.h"

Renderer::Renderer(SDL_Window* pWindow, int width, int height) 
	: m_pWindow{ pWindow }
	, m_Width{ width }
	, m_Height{ height }
	, m_NrOfPixels{ width * height }
	, m_ShowOnlyOne{ true }
{
	//Create Buffers
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);

	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);

	// set every pixel to black
	std::fill_n(static_cast<uint32_t*>(m_pBackBuffer->pixels), m_NrOfPixels, uint32_t{ 0 });

	// Init everything
	InitMapLayers();
	InitMaps();
}

Renderer::~Renderer()
{
	DestroyMaps();
}

void Renderer::Update(float elapsedSec)
{
	// set every pixel to black
	std::fill_n(static_cast<uint32_t*>(m_pBackBuffer->pixels), m_NrOfPixels, uint32_t{0});

	if (m_RenderMode == RenderMode::layerShadowMap)
	{
		// Update Mouse
		int mouseX;
		int mouseY;

		const Uint32 mouseState{ SDL_GetMouseState(&mouseX, &mouseY) };

		if (mouseState)
		{
			// Update Maps
			for (const auto& terrainMap : m_pTerrainMaps)
			{
				terrainMap->UpdateSunPos(mouseX, mouseY);
			}
		}
	}
}

void Renderer::Render() const
{
	//Lock BackBuffer
	SDL_LockSurface(m_pBackBuffer);

	// actual rendering
	for (const auto& terrainMap : m_pTerrainMaps)
	{
		terrainMap->Render(m_RenderMode);
	}

	//Update SDL Surface
	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

// change maps
void Renderer::ScrollWheelInput(float value)
{
	// change all map zooms
	for (const auto& terrainMap : m_pTerrainMaps)
	{
		terrainMap->ChangeZoom(value * 3.f);
	}
}

void Renderer::NewMaps()
{
	for (const auto& terrainMap : m_pTerrainMaps)
	{
		terrainMap->SetSeed(rand());
	}
}

void Renderer::ToggleRenderModes()
{
	std::stringstream ss;
	ss << "CurrentRenderMode: ";

	switch (m_RenderMode)
	{
	case RenderMode::noiseMap:
		m_RenderMode = RenderMode::layermap;
		ss << "layermap.\n";
		break;

	case RenderMode::layermap:
		m_RenderMode = RenderMode::layerShadowMap;
		ss << "layerShadowMap.\n";
		break;

	case RenderMode::layerShadowMap:
		m_RenderMode = RenderMode::noiseMap;
		ss << "noiseMap.\n";
		break;

	default:
		ss << "UNKOWN!!\n";
		break;
	}

	std::cout << ss.str();
}

void Renderer::ShowOnlyOneToggle()
{
	// toggle
	m_ShowOnlyOne = !m_ShowOnlyOne;

	// delete maps
	DestroyMaps();

	// Init new map(s)
	InitMaps();
}

void Renderer::ChangeMapSeed()
{
	std::cout << "Enter Following: \"map number\" \"new seed number\" \n";

	int mapNr{ -1 };
	int newSeed{ -1 };

	// get input from collum
	std::cin >> mapNr >> newSeed;

	if (mapNr <= 0 || mapNr > m_pTerrainMaps.size())
	{
		std::cout << "---mapNr Not Valid!---\n";
		return;
	}

	if (newSeed != -1)
	{
		m_pTerrainMaps[mapNr - 1]->SetSeed(newSeed);
	}
	else
	{
		std::cout << "---Not enoughData to change seed!---\n";
	}
}

void Renderer::AddLayer()
{
	std::cout << "Enter Following: \"layer name\" \"layer height\" \"r\" \"g\" \"b\" \n";

	std::string name{};
	float height{};
	int r{ - 1 };
	int g{ - 1 };
	int b{ - 1 };

	// get input from collum
	std::cin >> name >> height >> r >> g >> b;

	if (name != std::string{}
		&& height != 0.f
		&& r != -1
		&& g != -1
		&& b != -1)
	{
		m_pMapLayerManager->AddLayer(name, height, ColorRGB{ r, g, b });
		std::cout << "TerrainLayer Added!\n";
	}
	else
	{
		std::cout << "---Not enoughData to add layer!---\n";
	}
}

// screen
bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "screen.bmp");
}

// private member functions
void Renderer::InitMaps()
{
	constexpr int mapOffset{ 1 };

	NoiseValues noiseValues;
	Recti terrainRect;

	if (m_ShowOnlyOne)
	{
		m_pTerrainMaps.reserve(1);

		noiseValues.seed = rand();
		noiseValues.zoom = 100;
		noiseValues.octaves = 10;
		noiseValues.lacunarity = 1.7f;
		noiseValues.gain = 0.7f;
		noiseValues.weightedStrength = 0.f;
		noiseValues.fractalBounding = 0.57f;

		terrainRect.left = 0;
		terrainRect.top = 0;
		terrainRect.width = m_Width;
		terrainRect.height = m_Height;

		m_pTerrainMaps.emplace_back(new TerrainMap{ terrainRect, noiseValues, m_pMapLayerManager, m_pBackBuffer });
	}
	else
	{
		m_pTerrainMaps.reserve(4);

		///////////
		// map 1 //
		///////////

		noiseValues.seed = rand();
		noiseValues.zoom = 100;
		noiseValues.octaves = 10;
		noiseValues.lacunarity = 1.7f;
		noiseValues.gain = 0.7f;
		noiseValues.weightedStrength = 0.f;
		noiseValues.fractalBounding = 0.57f;

		terrainRect.left = 0;
		terrainRect.top = 0;
		terrainRect.width = m_Width / 2 - mapOffset;
		terrainRect.height = m_Height / 2 - mapOffset;
		m_pTerrainMaps.emplace_back(new TerrainMap{ terrainRect, noiseValues, m_pMapLayerManager, m_pBackBuffer });

		///////////
		// map 2 //
		///////////
		noiseValues.seed = rand();
		noiseValues.zoom = 100;
		noiseValues.octaves = 10;
		noiseValues.lacunarity = 1.7f;
		noiseValues.gain = 0.7f;
		noiseValues.weightedStrength = 0.f;
		noiseValues.fractalBounding = 0.57f;

		terrainRect.left = m_Width / 2 + mapOffset;
		terrainRect.top = 0;
		terrainRect.width = m_Width / 2 - mapOffset;
		terrainRect.height = m_Height / 2 - mapOffset;
		m_pTerrainMaps.emplace_back(new TerrainMap{ terrainRect, noiseValues, m_pMapLayerManager, m_pBackBuffer });

		/////////////
		//// map 3 //
		/////////////

		noiseValues.seed = rand();
		noiseValues.zoom = 100;
		noiseValues.octaves = 10;
		noiseValues.lacunarity = 1.7f;
		noiseValues.gain = 0.7f;
		noiseValues.weightedStrength = 0.f;
		noiseValues.fractalBounding = 0.57f;

		terrainRect.left = 0;
		terrainRect.top = m_Height / 2 + mapOffset;
		terrainRect.width = m_Width / 2 - mapOffset;
		terrainRect.height = m_Height / 2 - mapOffset;
		m_pTerrainMaps.emplace_back(new TerrainMap{ terrainRect, noiseValues, m_pMapLayerManager, m_pBackBuffer });

		/////////////
		//// map 4 //
		/////////////

		noiseValues.seed = rand();
		noiseValues.zoom = 100;
		noiseValues.octaves = 10;
		noiseValues.lacunarity = 1.7f;
		noiseValues.gain = 0.7f;
		noiseValues.weightedStrength = 0.f;
		noiseValues.fractalBounding = 0.57f;

		terrainRect.left = m_Width / 2 + mapOffset;
		terrainRect.top = m_Height / 2 + mapOffset;
		terrainRect.width = m_Width / 2 - mapOffset;
		terrainRect.height = m_Height / 2 - mapOffset;
		m_pTerrainMaps.emplace_back(new TerrainMap{ terrainRect, noiseValues, m_pMapLayerManager, m_pBackBuffer });
	}
}

void Renderer::InitMapLayers()
{
	// Create Manager
	m_pMapLayerManager = std::make_shared< MapLayerManager>();

	// add Layers
	m_pMapLayerManager->AddLayer("water", 0.4f, ColorRGB{ 30, 176, 251 });
	m_pMapLayerManager->AddLayer("sand", 0.5f, ColorRGB{ 255, 246, 193 });
	m_pMapLayerManager->AddLayer("grass", 0.7f, ColorRGB{ 118, 239, 124 });
	m_pMapLayerManager->AddLayer("mountainGrass", 1.1f, ColorRGB{ 22, 181, 141 });
}

void Renderer::DestroyMaps()
{
	// destroy maps
	for (size_t idx{}; idx < m_pTerrainMaps.size(); ++idx)
	{
		delete m_pTerrainMaps[idx];
	}
	m_pTerrainMaps.clear();
}