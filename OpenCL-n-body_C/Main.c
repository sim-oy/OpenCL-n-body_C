#include "Main.h"
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <Cl/cl.h>

struct Color {
    char R;
    char G;
    char B;
    char A;
};

int main() {
	printf("start\n");

    //float* particles = (float*) malloc(PARTICLEAMOUNT * 5 * sizeof(float));
    static float particles[PARTICLEAMOUNT * 5];
    
    srand(0);
    for (int i = 0; i < PARTICLEAMOUNT * 5; i += 5) {
        particles[i] = randf() * 0.2f + 0.4f;
        particles[i + 1] = randf() * 0.2f + 0.4f;
        particles[i + 2] = 0;
        particles[i + 3] = 0;
        particles[i + 4] = randf();
    }
    
    //char* windowBuffer = (char*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 4 * sizeof(char));
    char windowBuffer[WINDOW_WIDTH * WINDOW_HEIGHT * 4];
    
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    sfRenderWindow* window;
	sfEvent event;
    sfTexture* Texture = sfTexture_create(WINDOW_WIDTH, WINDOW_HEIGHT);
    sfSprite* sprite;

	window = sfRenderWindow_create(mode, "My Window", sfClose, NULL);
    if (!window)
        return -1;
    sfRenderWindow_setVerticalSyncEnabled(window, sfFalse);
    sprite = sfSprite_create();

    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event)) 
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
        sfRenderWindow_clear(window, sfBlack);
        memset(windowBuffer, 0, sizeof(windowBuffer));

        DrawParticles(particles, windowBuffer);
        
        sfTexture_updateFromPixels(Texture, windowBuffer, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
        sfSprite_setTexture(sprite, Texture, sfFalse);
        sfRenderWindow_drawSprite(window, sprite, NULL);

        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);

    printf("end\n");
    return 0;
}

void DrawParticles(float particles[], char windowBuffer[]) {
    int sum = 0;
    for (int i = 0; i < PARTICLEAMOUNT * 5; i += 5) {
        if (particles[i] < 0 || particles[i] >= 1.0 || particles[i + 1] < 0 || particles[i + 1] >= 1.0) {
            return;
        }
        
        int x = (int)(particles[i] * WINDOW_WIDTH);
        int y = (int)(particles[i + 1] * WINDOW_HEIGHT);

        int index = (y * WINDOW_WIDTH + x) * 4;

        particles[i] += 0.00001;
        particles[i + 1] += 0.00001;

        windowBuffer[index] = 255;
        windowBuffer[index + 1] = 255;
        windowBuffer[index + 2] = 255;
        windowBuffer[index + 3] = 255;
    }
}


/*
// Create a render texture
sfRenderTexture* renderTexture = sfRenderTexture_create(windowWidth, windowHeight, 0);

// Create an array of pixels
int arrayWidth = 10;
int arrayHeight = 10;
sfUint8 pixels[arrayWidth * arrayHeight * 4];

// Fill the array with pixel data
for (int i = 0; i < arrayWidth * arrayHeight * 4; i += 4)
{
    pixels[i] = 255;     // Red channel
    pixels[i + 1] = 0;   // Green channel
    pixels[i + 2] = 0;   // Blue channel
    pixels[i + 3] = 255; // Alpha channel
}

// Update the texture of the render texture with the pixel data from the array
sfTexture_updateFromPixels(sfRenderTexture_getTexture(renderTexture), pixels, arrayWidth, arrayHeight, 0, 0);

// Draw the render texture to the screen
sfRenderWindow_drawRenderTexture(window, renderTexture, NULL);*/


/*
#include <stdio.h>
#include <stdlib.h>

#define randf() (float)rand()/(float)(RAND_MAX)

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define PARTICLEAMOUNT 10000

void DrawParticles(float* particles, char* windowBuffer);

int main()
{
    printf("Hello World\n");

    float* particles = (float*)malloc(PARTICLEAMOUNT * 5 * sizeof(float));

    char* windowBuffer = (char*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 4);

    for (int i = 0; i < PARTICLEAMOUNT; i += 5) {
        particles[i] = randf() * 0.2f + 0.4f;
        particles[i + 1] = randf() * 0.2f + 0.4f;
        particles[i + 2] = 0;
        particles[i + 3] = 0;
        particles[i + 4] = randf();
    }


    DrawParticles(particles, windowBuffer);


    for (int i = 0; i < WINDOW_WIDTH * WINDOW_WIDTH; i += 4) {
        
        printf("%d", windowBuffer[i]);
        printf(" %d", windowBuffer[i + 1]);
        printf(" %d", windowBuffer[i + 2]);
        printf(" %d\n", windowBuffer[i + 3]);

        if (windowBuffer[i] != 0) {
            printf("aaa\n");
        }
    }

    return 0;
}


void DrawParticles(float* particles, char* windowBuffer) {
    for (int i = 0; i < PARTICLEAMOUNT; i += 5) {
        if (particles[i] < 0 || particles[i] >= 1.0 || particles[i + 1] < 0 || particles[i + 1] >= 1.0)
            return;

        int x = (int)(particles[i] * WINDOW_WIDTH);
        int y = (int)(particles[i + 1] * WINDOW_HEIGHT);

        int index = (y * WINDOW_WIDTH + x) * 4;

        windowBuffer[index] = (char)255;
        windowBuffer[index + 1] = (char)255;
        windowBuffer[index + 2] = (char)255;
        windowBuffer[index + 3] = (char)255;

    }
}*/