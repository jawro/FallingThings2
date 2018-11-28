#include <iostream>
#include <SDL.h>
#include <time.h> 
#include<stdlib.h>

using namespace std;
const int window_height = 1000;
const int window_width = 1200;
class moveable
{
public:
	SDL_Rect shape;
	int xpos, ypos;

	//virtual void update() = 0;
	//virtual void render() = 0;
};
/*
struct Color
{
	char data[4];

	Color(int first, int second, int third, int forth)
	{
		data[0] = first;
		data[1] = second;
		data[2] = third;
		data[3] = forth;
	}
};

enum squareColor
{
	red = Color(255,0,0,255)
	green,
	blue,
	yellow,
	magenta,
	pink,
	exit
};
*/
class falling_thing : public moveable
{
public:
	int color[4] = { 255,100,100,255 };

	falling_thing()
	{
		srand(time(NULL));
		xpos = 10 + rand() % (window_width - 10);
		ypos = window_height;
	}
	~falling_thing()
	{}

	void fall()
	{
		ypos+=4;
		if (ypos > 1000)
		{
			ypos = 0;
			xpos = 10 + rand() % (window_width - 10);
		}
	}

	void update() 
	{
		shape.h = 40; 
		shape.w = 40;
		shape.x = xpos;
		shape.y = ypos;
	}

	void render(SDL_Renderer* rend) 
	{
		SDL_SetRenderDrawColor(rend, color[0], color[1], color[2], color[3]);
		SDL_RenderFillRect(rend, &shape);
	}


};

class picker : public moveable
{
public:
	int height, width, health;
	int color[4] = { 255,200,100,255 };
	picker()
	{}
	~picker()
	{}

	void update(SDL_Event ev)
	{
		shape.h = height = 60;
		shape.w = width = 200;
		shape.x = xpos = ev.button.x-width/2;
		shape.y = ypos = window_height -height;
	}
	void render(SDL_Renderer* rend) 
	{
		SDL_SetRenderDrawColor(rend, color[0], color[1], color[2], color[3]);
		SDL_RenderFillRect(rend, &shape);
	}

};

class Game
{
public:
	SDL_Window *window;
	SDL_Renderer *renderer;
	picker *player = new picker;
	falling_thing *square = new falling_thing;
	bool run = 0;

	Game() {};
	~Game() {};

	void start(const char * gamename, int xposition, int yposition, int width, int height)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)	//SDL initialization
		{
			cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
			run = 0;
		}
		else
		{
			cout << "SDL initialization succeeded!";
			window = SDL_CreateWindow(gamename, xposition, yposition, width, height, SDL_WINDOW_RESIZABLE); //creating visible  window
			if (window)
			{
				cout << "Window OK" << endl;
			}

			renderer = SDL_CreateRenderer(window, -1, 0);	//initializing renderer
			if (renderer)
			{
				cout << "Renderer OK" << endl;
			}

			run = 1;
		}
	}
	void update()
	{
		square->fall();
		square->update();
	}
	void Events()
	{
		SDL_Event evnt; //"event" is a keyword in std so i used "evnt" :)
		SDL_PollEvent(&evnt);

		switch (evnt.type) 
		{

		case SDL_QUIT:
			run = 0;   //switch off the game
			break;

		case SDL_MOUSEMOTION:
			player->update(evnt);		//player follows mouse position
			break;
		}
	}
	void render()
	{
		SDL_RenderClear(renderer);
		//sth to show
		square->render(renderer);
		player->render(renderer);
		SDL_SetRenderDrawColor(renderer, 30, 25, 42,255);
		SDL_RenderPresent(renderer);


	}
	void clean_up( )//destructor
	{
		delete player;
		delete square;
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		cout << "Game shut down." << endl;
	}
};

int main(int argc, char * argv[])
{
	Game game;
	game.start("gra", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height);


	while (game.run)
	{
		game.Events();
		game.update();
		game.render();
	}
	
	game.clean_up();
	cin.get();
	return 0;
}