#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Utils/primitives.hpp>
#include <Utils/shader.hpp>
#include <Utils/camera.hpp>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_GLContext main_context;

int width;
int height;
double delta = 0.0f;
Uint64 tick_last = 0;

std::string base_path = SDL_GetBasePath();

Shader base_shader;
unsigned int texture_container;
unsigned int texture_reimu;

Shader skybox_shader;
unsigned int skybox_texture;

Camera main_camera;

/* Forward Declaration. Cringe, remove later */
void BasicScene();
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
unsigned int loadCubemap(std::vector<std::string> faces);

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO); // Required so RenderDoc can work

    SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// Request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Create the window */
    window = SDL_CreateWindow("Grey Heavens", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
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

    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    main_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Starting pos

    if (!SDL_SetWindowRelativeMouseMode(window, true)) {
        SDL_Log("Something fcked up g, can't set WindowRelative: %s", SDL_GetError());
    }

    BasicScene();

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if ((event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE)
        ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
		SDL_GetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
    }

    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        main_camera.ProcessMouseMovement(event->motion.xrel, -event->motion.yrel);
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_GL_SwapWindow(window);

    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // @TODO:
    // WASD Movement
    // Move to it's own function soon
    // Has to be called per Iteration, because AppEvent don't automatically repeat on it's own
    const bool *key_states = SDL_GetKeyboardState(NULL);

    if (key_states[SDL_SCANCODE_W]) {
        main_camera.ProcessKeyboard(FORWARD, delta);
    } 

    if (key_states[SDL_SCANCODE_S]) {
        main_camera.ProcessKeyboard(BACKWARD, delta);
    }

    if (key_states[SDL_SCANCODE_A]) {
        main_camera.ProcessKeyboard(LEFT, delta);
    }

    if (key_states[SDL_SCANCODE_D]) {
        main_camera.ProcessKeyboard(RIGHT, delta);
    }

    // Transformation matrixes
    base_shader.use();
    glm::mat4 model = glm::mat4(1);

    Uint64 tick_current = SDL_GetTicks();
    delta = (double)((tick_current - tick_last) * 0.001f); // Ugly casting. Very cringe tbh
    tick_last = tick_current;

    model = glm::rotate(model, glm::radians(45.0f + SDL_GetTicks()) * 0.01f, glm::vec3(0.5f, 1.0f, 0.0f));

    glm::mat4 view = main_camera.GetViewMatrix();

    glm::mat4 projection = glm::perspective(45.0f, (float) width / (float) height, 0.01f, 1000.0f);

    base_shader.setMat4("model", model);
    base_shader.setMat4("view", view);
    base_shader.setMat4("projection", projection);

    // Bind and draw
    Primitives::UseVAOCube();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_container);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_reimu);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Skybox
    Primitives::UseVAOSkybox();
    glDepthFunc(GL_LEQUAL);
    skybox_shader.use();

    view = glm::mat4(glm::mat3(main_camera.GetViewMatrix())); // To stop translation
    skybox_shader.setMat4("view", view);
    skybox_shader.setMat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}

void BasicScene() 
{
    std::string container_image_path = "assets/textures/container2.png";
    texture_container = TextureFromFile(container_image_path.c_str(), base_path);

    std::string reimu_image_path = "assets/textures/reimu_timbersaw.png";
    texture_reimu = TextureFromFile(reimu_image_path.c_str(), base_path);

    std::string shader_vert_path = base_path + "assets/shaders/basic/cube.vert";
    std::string shader_frag_path = base_path + "assets/shaders/basic/cube.frag";
    base_shader = Shader(shader_vert_path.c_str(), shader_frag_path.c_str());

    base_shader.use();
    base_shader.setInt("texture1", 0);
    base_shader.setInt("texture2", 1);

    /// Skybox
    ///
    /// faces contains file names
    std::vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
    skybox_texture = loadCubemap(faces);  
    std::string skybox_vert_path = base_path + "assets/shaders/basic/skybox.vert";
    std::string skybox_frag_path = base_path + "assets/shaders/basic/skybox.frag";
    skybox_shader = Shader(skybox_vert_path.c_str(), skybox_frag_path.c_str());
    skybox_shader.use();
    skybox_shader.setInt("skybox", 0);
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

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string face_path = base_path + "assets/textures/cubemap/" + faces[i];

        stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char *data = stbi_load(face_path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}  
