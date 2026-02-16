/*
  Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <glad/glad.h>
#include <iostream>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_GLContext maincontext;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// Request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Create the window */
    window = SDL_CreateWindow("Grey Heavens", 800, 600, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    maincontext = SDL_GL_CreateContext(window);
    if (!maincontext) {
        SDL_Log("Couldn't create context: %s", SDL_GetError());
		return SDL_APP_FAILURE;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
        SDL_Log("Couldn't get ProcAddres: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

    // Sync to monitors refresh rate, idk
    SDL_GL_SetSwapInterval(1);

    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_GL_SwapWindow(window);

    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

