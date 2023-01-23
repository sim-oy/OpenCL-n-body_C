#include "Main.h"

#define N_PAR 1
#define N2 16000
#define rounding 256
#define N (N2 % rounding == 0 ? N2 : (N2 - N2 % rounding) + rounding)

int main() {
	printf("start\n");

    srand(0);

    printf("N = %d\n", N);

    const float G = 0.000000001f;
    const float smthing = 0.0000000001f;
    
    //static float particles[N * 5];
    static particle particles[N];

    GenerateParticles(particles, N);
    //ParticlesPreset8(particles, N);

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

    clock_t oa_tim_strt = 0, oa_tim_end = 0;

    int frames = 0;
    double times[FRAMES_PER_PRINT];

    FILE* file = NULL;
    if (fopen_s(&file, "data.txt", "w") != 0)
    {
        printf("Error opening to the file");
        return -1;
    }
    CLInit(particles, N * 5, G, smthing);

    while (sfRenderWindow_isOpen(window))
    {
        oa_tim_strt = clock();

        while (sfRenderWindow_pollEvent(window, &event)) 
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
        sfRenderWindow_clear(window, sfBlack);

        if (WRITE_TO_FILE) {
            for (int i = 0; i < N * 5; i += 5) {
                fprintf(file, "%f%f\n", 
                particles[i],
                particles[i + 1]);
            }
        }
        
        CLRun(particles, N * 5);
        //CalculateSingleArray(particles, N, G, smthing);

        memset(windowBuffer, 0, sizeof(windowBuffer));
        DrawParticles(particles, windowBuffer);
        //DrawParticlesSerial(particles, windowBuffer);
        
        sfTexture_updateFromPixels(Texture, windowBuffer, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
        sfSprite_setTexture(sprite, Texture, sfFalse);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        DrawTrackingCircle(window, particles);

        sfRenderWindow_display(window);

        oa_tim_end = clock();

        double elapsedTime_s = ((double)(oa_tim_end - oa_tim_strt)) / CLOCKS_PER_SEC;
        times[frames] = elapsedTime_s;
        if (frames >= FRAMES_PER_PRINT - 1) {
            double avg_elapsedTime_s = DoubleArraySum(times, FRAMES_PER_PRINT) / (double)FRAMES_PER_PRINT;
            printf("time: ms %d\t fps: %.1lf\n", (int)(avg_elapsedTime_s * 1000), 1.0 / avg_elapsedTime_s);
            frames = 0;
        }
        else {
            frames++;
        }

        //sleep(1000);
    }
    fclose(file);
    sfRenderWindow_destroy(window);

    printf("end\n");
    return 0;
}

void DrawParticles2(float particles[], char windowBuffer[]) {
    int sum = 0;
    for (int i = 0; i < N; i++) {
        if (particles[i] < 0 || particles[i] >= 1.0 || particles[i + N] < 0 || particles[i + N] >= 1.0) {
            continue;
        }

        int x = (int)(particles[i] * WINDOW_WIDTH);
        int y = (int)(particles[i + N] * WINDOW_HEIGHT);

        int index = (y * WINDOW_WIDTH + x) * 4;

        windowBuffer[index] = 255;
        windowBuffer[index + 1] = 255;
        windowBuffer[index + 2] = 255;
        windowBuffer[index + 3] = 255;
    }
}

void DrawParticles(particle particles[], char windowBuffer[]) {
    int sum = 0;
    for (int i = 0; i < N; i++) {
        if (particles[i].x < 0 || particles[i].x >= 1.0 || particles[i].y < 0 || particles[i].y >= 1.0) {
            continue;
        }

        int x = (int)(particles[i].x * WINDOW_WIDTH);
        int y = (int)(particles[i].y * WINDOW_HEIGHT);

        int index = (y * WINDOW_WIDTH + x) * 4;

        windowBuffer[index] = 255;
        windowBuffer[index + 1] = 255;
        windowBuffer[index + 2] = 255;
        windowBuffer[index + 3] = 255;
    }
}


double DoubleArraySum(double array[], int len) {
    double sum = 0;
    for (int i = 0; i < len; i++) {
        sum += array[i];
    }
    return sum;
}


void DrawTrackingCircle(sfRenderWindow* window, particle particles[]) {
    
    int x = (int)(particles[0].x * WINDOW_WIDTH);
    //int y = (int)(particles[1] * WINDOW_HEIGHT);
    int y = (int)(particles[0].y * WINDOW_HEIGHT);

    sfCircleShape* circle = sfCircleShape_create();
    sfCircleShape_setRadius(circle, 2.0f);
    sfCircleShape_setPosition(circle, (sfVector2f) { x, y });
    sfCircleShape_setFillColor(circle, sfRed);
    sfRenderWindow_drawCircleShape(window, circle, NULL);
}