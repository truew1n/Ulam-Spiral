#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define WIDTH 1000
#define HEIGHT 1000
#define STEP_SIZE 1

typedef struct Color {
    int r;
    int g;
    int b;
} Color;

typedef Color Canvas[HEIGHT][WIDTH];

static Canvas canvas;

void saveAsPPM(const char *filepath)
{
    FILE *file = fopen(filepath, "wb");
    if(!file) exit(1);

    fprintf(file, "P6\n %i %i 255\n", WIDTH, HEIGHT);

    int y; for(y = 0; y < HEIGHT; ++y) {
        int x; for(x = 0; x < WIDTH; ++x) {
            Color pixel = canvas[y][x];
            uint8_t bytes[3] = {
                pixel.r,
                pixel.g,
                pixel.b
            };
            fwrite(bytes, sizeof(bytes), 1, file);
        }
    }
}

void fill_canvas(Color color)
{
    int y; for(y = 0; y < HEIGHT; ++y) {
        int x; for(x = 0; x < WIDTH; ++x) {
            canvas[y][x] = color;
        }
    }
}

bool is_prime(int number)
{
    if(number < 2) return false;
    int p = 2;
    while(number >= p*p){
        if(number % p == 0) return false;
        if(p < 3) p++;
        else p+=2;
    }
    return true;
}

void draw_rectangle(int x0, int y0, int w, int h, Color color)
{
    if(x0 < 0) x0 = 0;
    if(y0 < 0) y0 = 0;
    if(w < 0)  w  = 0;
    if(h < 0)  h  = 0;
    int x1 = x0 + w;
    int y1 = y0 + h;
    int y; for(y = y0; y < y1; ++y){
        int x; for(x = x0; x < x1; ++x){
            canvas[y][x] = color;
        }
    }
}

int stepCount = 0;
int stepVal = 1;

void render_ulam_spiral()
{
    int count = 1;
    int w = WIDTH/2;
    int h = HEIGHT/2;
    int x; for(x = 1; x < ((WIDTH/2+HEIGHT/2)*2)/STEP_SIZE; ++x){
        if(stepCount == 2){
            stepVal++;
            stepCount = 0;
        }
        stepCount++;

        int mod = x % 4;
        int i; for(i = 1; i < stepVal+1; ++i){
            if(is_prime(count)){
                draw_rectangle(w - STEP_SIZE / 2, h - STEP_SIZE / 2, STEP_SIZE, STEP_SIZE, (Color){0, 255, 0});
            }
            switch(mod){
                case 0:
                    h += STEP_SIZE;
                    break;
                case 1:
                    w += STEP_SIZE;
                    break;
                case 2:
                    h -= STEP_SIZE;
                    break;
                case 3:
                    w -= STEP_SIZE;
                    break;
            }
        count++;
        }
    }
}

int main(void)
{
    fill_canvas((Color){18, 18, 18});
    render_ulam_spiral();
    saveAsPPM("canvas.ppm");
    return 0;
}
