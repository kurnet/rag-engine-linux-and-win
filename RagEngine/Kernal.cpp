#include "stdafx.h"
#include "Kernal.h"

#include <iostream>
#include <memory>
#ifdef __linux__
#include <libgen.h>
#endif
// #include<stdlib.h>

#include "Settings.h"
#include "Page.h"
#include "Page_Default.h"
#include "MouseEvent.h"
// #include "sdl-widgets.h"

Kernal s_kernal;

Kernal::Kernal():
	_renderer(nullptr),
	window_(nullptr),
	font_(nullptr),
	_dt(0),
	_updated(false),
	_quit(false),
	currentPage_(nullptr),
	nextPage_(nullptr),
	_bgRed(255),
	_bgGreen(255),
	_bgBlue(255)
{
	for (int i=0; i<GLYPH_SIZE; ++i)
	{
		glyphs_[i] = nullptr;
	}
}

Kernal::~Kernal()
{
	if (nextPage_ != nullptr)
	{
		delete nextPage_;
		nextPage_ = nullptr;
	}
	
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(window_);
	window_ = nullptr;
	_renderer = nullptr;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void Kernal::showSDLError()
{
	std::cout << SDL_GetError() << std::endl;
}

bool Kernal::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "sdl init failed: " << std::endl;
		showSDLError();
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "sdl hint render scale quality set failed" << std::endl;
		showSDLError();
		return false;
	}
	
	// start SDL_ttf
	if(TTF_Init()==-1)
	{
		printf("TTF_Init: %s\n", TTF_GetError());
		return 2;
	}
	// atexit(free_font); /* remember to free any loaded font and glyph cache */

// create window
	window_ = SDL_CreateWindow(settings_.winName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, settings_.screenWidth, settings_.screenHeight, SDL_WINDOW_SHOWN);

	if (window_ == nullptr)
	{
		std::cout << "window count not be created" << std::endl;
		showSDLError();
		return false;
	}

	_renderer = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr)
	{
		std::cout << "renderer could not be created!" << std::endl;
		showSDLError();
		return false;
	}
	
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << "SDL_image could not initialize" << std::endl;
		showSDLError();
		return false;
	}

	return true;
}

bool Kernal::exec(int argc, char* argv[], const Settings& settings, Page* startPage)
{
#ifdef WIN32
	{
		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		// Note: _makepath is deprecated; consider using _makepath_s instead
		_splitpath_s(argv[0], drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT); // C4996


		// Note: _splitpath is deprecated; consider using _splitpath_s instead
//		printf("Path extracted with _splitpath:\n");
//		printf("  Drive: %s\n", drive);
//		printf("  Dir: %s\n", dir);
//		printf("  Filename: %s\n", fname);
//		printf("  Ext: %s\n", ext);
		baseDir_ = dir;
	}
#elif __linux__
	baseDir_ = dirname(argv[0]);
#endif
	std::cout << "base dir: " << baseDir_ << std::endl;
	
	settings_ = settings;
	setBackgroundColor(settings_.bgRed, settings_.bgGreen, settings_.bgBlue);		

	if (!initSDL())
	{
		return false;
	}

	if (startPage == nullptr)
	{
		startPage = new Page_Default();
	}

	nextPage_ = startPage;
	return loop();
}

bool Kernal::loop()
{
	SDL_Event e;

	const double timePerFrame = 1.0 / settings_.frameRate;
	double timeSinceLastUpdate = 0;

	create();

	while (!_quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				_quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				// key code reference : https://wiki.libsdl.org/SDL_Keycode

				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					_quit = true;
					break;
				// case SDL_MOUSEMOTION:
				// 	std::cout << "mouse motion"p << std::endl;
				// 	break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				std::unique_ptr<MouseEvent> ptr = std::unique_ptr<MouseEvent>(new MouseEvent(MouseEvent::MOUSE_MOVE, mouseX, mouseY));
				stage_->dispatchEvent(*ptr.get());
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				std::unique_ptr<MouseEvent> ptr = std::unique_ptr<MouseEvent>(new MouseEvent(MouseEvent::MOUSE_DOWN, mouseX, mouseY));
				stage_->dispatchEvent(*ptr.get());
			}
			else if (e.type == SDL_MOUSEBUTTONUP)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				std::unique_ptr<MouseEvent> ptr = std::unique_ptr<MouseEvent>(new MouseEvent(MouseEvent::MOUSE_UP, mouseX, mouseY));
				stage_->dispatchEvent(*ptr.get());
			}
		}

		std::chrono::time_point<std::chrono::system_clock> now;
		now = std::chrono::system_clock::now();
		if (!_updated)
		{
			now = std::chrono::system_clock::now();
			_lastClock = now;
			_updated = true;
		}
		std::chrono::duration<double> elapsed_seconds = now - _lastClock;
		_lastClock = now;
		timeSinceLastUpdate += elapsed_seconds.count();

//		std::cout << "accum: " << accum << std::endl;
//		std::cout << "dt: " << _dt << std::endl;
//		std::cout << "timeperframe: " << timePerFrame << std::endl;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
//			std::cout << "update" << std::endl;
			update(timePerFrame);
		}
		
//		std::cout << "render" << std::endl;		
		render();
		
//		update();
//		render();
	}

	return true;
}

void Kernal::create()
{
	stage_ = std::unique_ptr<Stage>(new Stage());
}

void Kernal::free_font()
{
	if (font_ != nullptr)
		TTF_CloseFont(font_);
	font_= nullptr;
	free_glyphs();
}

void Kernal::free_glyphs()
{
	int i;

	for(i=0; i<128; i++)
	{
		if(glyphs_[i])
			SDL_FreeSurface(glyphs_[i]);
		glyphs_[i]=0;
	}
}

void Kernal::update(const double dt)
{
	_dt = dt;
	if (currentPage_ != nullptr)
	{
		// std::cout << "update current page"<< std::endl;
		currentPage_->update(_dt);
	}
	
	// change to next page if there is one
	if (nextPage_ != nullptr)
	{
		Page* targetPage = nextPage_;
		nextPage_ = nullptr;
		setPage(targetPage);
	}
}

void Kernal::render()
{
	SDL_SetRenderDrawColor(kernal.getRenderer(), 0xFF, _bgRed, _bgGreen, _bgBlue);
	SDL_RenderClear(kernal.getRenderer());
	
	if (currentPage_ != nullptr)
	{
		currentPage_->render();
	}
	
	SDL_RenderPresent(kernal.getRenderer());
}

void Kernal::changePage(Page* page)
{
	nextPage_ = page;
}

void Kernal::setPage(Page* page)
{
	if (currentPage_ != nullptr)
	{
		stage_->removeChild(currentPage_);
		currentPage_->dispose();
		delete currentPage_;
		currentPage_ = nullptr;
	}
	
	currentPage_ = page;
	stage_->addChild(currentPage_);
	currentPage_->enter();
}

SDL_Renderer* Kernal::getRenderer()
{
	return _renderer;
}

SDL_Window* Kernal::getWindow()
{
	return window_;
}

std::string Kernal::getBaseDir() const
{
	return baseDir_;
}

void Kernal::setBackgroundColor(const Uint8 red, const Uint8 green, const Uint8 blue)
{
	_bgRed = red;
	_bgGreen = green;
	_bgBlue = blue;
}

Kernal& Kernal::getInstance()
{
	return s_kernal;
}

Stage& Kernal::getStage()
{
	return *stage_.get();
}

int Kernal::getScreenWidth() const
{
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(window_, &w, &h);
	return w;
}

int Kernal::getScreenHeight() const
{
	return settings_.screenHeight;
}

bool Kernal::isQuit()
{
	return _quit;
}

void Kernal::quit()
{
	_quit = true;
}
