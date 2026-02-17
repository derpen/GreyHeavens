#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <glad/glad.h>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Utils/primitives.hpp>
#include <Utils/shader.hpp>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_GLContext main_context;

std::string base_path = SDL_GetBasePath();
unsigned int texture_container;
unsigned int texture_reimu;

/* Forward Declaration. Cringe, remove later */
void BasicScene();
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

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

    main_context = SDL_GL_CreateContext(window);
    if (!main_context) {
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
    BasicScene();

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if ((event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE)
        ||
        event->type == SDL_EVENT_QUIT) 
    {
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

    // Bind and draw
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_container);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_reimu);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

void BasicScene() 
{
    Primitives::UseVAOPlane();
    std::string container_image_path = "assets/textures/container2.png";
    texture_container = TextureFromFile(container_image_path.c_str(), base_path);

    std::string reimu_image_path = "assets/textures/reimu_timbersaw.png";
    texture_reimu = TextureFromFile(reimu_image_path.c_str(), base_path);

    std::string shader_vert_path = base_path + "assets/shaders/basic/plane.vert";
    std::string shader_frag_path = base_path + "assets/shaders/basic/plane.frag";
    Shader base_shader(shader_vert_path.c_str(), shader_frag_path.c_str());

    base_shader.use();
    base_shader.setInt("texture1", 0);
    base_shader.setInt("texture2", 1);
}

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    std::string filename = std::string(path);
    filename = directory + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_NONE;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
