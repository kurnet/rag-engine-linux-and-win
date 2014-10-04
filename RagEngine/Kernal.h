#ifndef __KERNAL_H__
#define __KERNAL_H__

#include <ctime>
#include <chrono>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Settings.h"
#include "Stage.h"

#define GLYPH_SIZE 128

class Page;

class Kernal
{
public:
	Kernal();
	~Kernal();

public:
	bool exec(int argc, char* argv[], const Settings& settings, Page* startPage = nullptr);
	void showSDLError();

	void changePage(Page* page);
	
	bool isQuit();
	void quit();
	void setBackgroundColor(const Uint8 red, const Uint8 green, const Uint8 blue);

	// getter and setter
	SDL_Renderer* getRenderer();
	SDL_Window* getWindow();
	Stage& getStage();
	int getScreenWidth() const;
	int getScreenHeight() const;
	std::string getBaseDir() const;

private:
	bool initSDL();
	bool loop();
	void update(const double dt);
	void render();
	void setPage(Page* page);
	void create();
	void free_font();
	void free_glyphs();
	
private:
	SDL_Renderer* _renderer;
	SDL_Window* window_;
	TTF_Font* font_;
	SDL_Surface* glyphs_[GLYPH_SIZE];
	std::chrono::time_point<std::chrono::system_clock> _lastClock;
	double _dt;
	bool _updated;
	bool _quit;
	Page* currentPage_;
	Page* nextPage_;
	Settings settings_;
	Uint8 _bgRed;
	Uint8 _bgGreen;
	Uint8 _bgBlue;
	std::string baseDir_;
	std::unique_ptr<Stage> stage_;
	
public:
	static Kernal& getInstance();
};

#define kernal Kernal::getInstance()
#endif
