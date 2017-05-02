#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "utilities.h"


#if (defined _WIN32) || (defined _WIN64)
#pragma warning(disable:4996) 
#endif

char           gMediaPath[256] = "../Media";
SDL_Rect       gMainWinRect = { 100, 100, 640, 480 };
SDL_Window *   gMainWindow = NULL;
SDL_Renderer * gMainRenderer = NULL;
SDL_Color      gBackgroundColor = { 0, 0, 0, 255 };

static void cleanAll()
{
	if( gMainRenderer ) SDL_DestroyRenderer(gMainRenderer), gMainRenderer = NULL;
	if( gMainWindow ) SDL_DestroyWindow(gMainWindow), gMainWindow = NULL;
}


void initApp(int argc, char *argv[])
{
	SDL_SetHint(SDL_HINT_IME_INTERNAL_EDITING, "1" );
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS );
	// 创建窗口
	if( ! (gMainWindow = SDL_CreateWindow(argv[0], gMainWinRect.x, gMainWinRect.y, gMainWinRect.w, gMainWinRect.h, 0) ) ) {
		printf("Error create SDL window\n");
		cleanAll();	exit(-1);
	}
	// 创建绘制环境
	if( ! (gMainRenderer = SDL_CreateRenderer(gMainWindow, -1, SDL_RENDERER_ACCELERATED)) ) {
		printf("Error create SDL Renderer\n");
		cleanAll();	exit(-1);
	}
	// 初始化 true type font使用
	if ( TTF_Init() < 0 ) {
		printf("Couldn't initialize TTF: %s\n",SDL_GetError());
		cleanAll();	exit(-1);
	}
}

void endApp()
{
	TTF_Quit();
	SDL_DestroyRenderer(gMainRenderer);
	SDL_DestroyWindow(gMainWindow);
	SDL_Quit();
}


void display()
{
	SDL_SetRenderDrawColor(gMainRenderer, gBackgroundColor.r, gBackgroundColor.g,
		gBackgroundColor.b, gBackgroundColor.a);
	SDL_RenderClear(gMainRenderer);
	{
		SDL_Color textcolor = {255,255,0,255};
		SDL_Texture * texture = loadTextureFromBMP(FullPath("/default/helloSDL.bmp"));
		SDL_RenderCopy(gMainRenderer, texture, NULL, NULL);
		SDL_DestroyTexture(texture);
		// 绘制文字
		drawstring("Hi, there!", gMainWinRect.w/8,20, FullPath("/default/FreeSerif.ttf"), 64, textcolor);
		textcolor.r = 0; // 红色
		drawstring("Can you see me?", gMainWinRect.w/8,gMainWinRect.h-100, FullPath("/default/FreeSerif.ttf"), 64, textcolor);
	}
	// present the result
	SDL_RenderPresent(gMainRenderer);
}

void handleEvent(SDL_Event* e)
{
	switch (e->type) {
	case SDL_MOUSEMOTION:
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_KEYDOWN:
		break;
	case SDL_KEYUP:
		switch (e->key.keysym.sym) {
		case SDLK_ESCAPE: 
			{
				SDL_Event ev;
				ev.type = SDL_QUIT; 
				SDL_PushEvent( &ev );
			}
		}
		break;
	default: 
		break;
	}
}


int main(int argc, char *argv[]) 
{
	if( argc>1 ) strcpy(gMediaPath, argv[1]);
	initApp(argc,argv);
	display();
	// Enter event processing loop 
	while ( 1 ) 
	{
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break; // 终止应用程序
			}
			handleEvent( &e );
		}
		// don't take all the cpu time
		SDL_Delay(10); 
	}
	endApp();

	return 0;
}

