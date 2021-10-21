#include <SDL2/SDL.h>  
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <linux/fs.h>
#include <fcntl.h>

int main()
{
    bool quit = false;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Mouse Events", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    char buffer[10];
	int fp = open("/dev/mbdriver", O_RDWR);
    SDL_Event event;

    while(!quit)
    {

        while (SDL_PollEvent(&event)) 
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.button)
                    {
                        case SDL_BUTTON_LEFT:

                            printf("Left button was pressed!");
                            SDL_ShowSimpleMessageBox(0, "Click", "Left button was pressed!", window);
                            break;
                        case SDL_BUTTON_RIGHT:
                            printf("Right button was pressed!");
                            SDL_ShowSimpleMessageBox(0, "Click", "Right button was pressed!", window);
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if(event.wheel.y == -1)
                    {
                        printf("You scored down!\n");
                        buffer[0] = '1';
                        write(fp,buffer,1);
                        SDL_ShowSimpleMessageBox(0, "Wheel Event", "Scroll down!", window);
                    } else if (event.wheel.y == 1)
                    {
                        printf("You scored up!\n");
                        buffer[0] = '0';
                        write(fp,buffer,1);
                        SDL_ShowSimpleMessageBox(0, "Wheel Event", "Scroll up!", window);
                    }

            }

        }

        SDL_Rect dstrect = { 288, 208, 64, 64 };

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

}