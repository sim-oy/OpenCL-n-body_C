#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <stdio.h>
#include <Cl/cl.h>
#include "Main.h"

struct Color
{
    char R;
    char G;
    char B;
    char A;
};

int main() {
	printf("start");

    float particles[PARTICLEAMOUNT * 5];

    srand(0);
    for (int i = 0; i < PARTICLEAMOUNT; i += 5) {
        particles[i] = randf() * 0.2f + 0.4f;
        particles[i + 1] = randf() * 0.2f + 0.4f;
        particles[i + 2] = 0;
        particles[i + 3] = 0;
        particles[i + 4] = randf();
    }
    
    char windowBuffer[WINDOW_WIDTH * WINDOW_HEIGHT * 4];
    
	sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
	
    sfRenderWindow* window;
    sfTexture* buffer;
	sfEvent event;

	window = sfRenderWindow_create(mode, "My Window", sfClose, NULL);
    if (!window)
        return -1;
    buffer = sfTexture_create(sfRenderWindow_getSize(window).x, sfRenderWindow_getSize(window).y);

    sfSprite* sprite;
    sprite = sfSprite_create();
    sfSprite_setTexture(sprite, buffer, sfTrue);

    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, sfBlack);

        DrawParticles(particles);

        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);

    printf("end");
    return 0;
}

void DrawParticles(float* particles) {
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
}