#include "libsnake.c"
#include <SDL.h>
typedef struct {
    int w, h, bpp, z;
    SDL_Surface *screen;
} sdlstate_t;
void sdldraw(state_t state, sdlstate_t *sdlstate) {
    SDL_Rect rect;
    rect.x = 0 * sdlstate->z;
    rect.y = 0 * sdlstate->z;
    rect.w = sdlstate->w * sdlstate->z;
    rect.h = sdlstate->h * sdlstate->z;
    //Uint32 col = SDL_MapRGB(sdlstate->screen->format, 255, 255, 255);
    Uint32 col = SDL_MapRGB(sdlstate->screen->format, 0, 0, 0);
    SDL_FillRect(sdlstate->screen, &rect, col);

    int ref = sdlstate->h;
    int balld = ref / HH;
    uint64_t dir_ = state.dir >> 2;
    int d_x = state.h_x, d_y = state.h_y;
    for (int i = 1; i < state.len; i++) {
        int dir = dir_ & 0x3;
        if (dir == S) {
            if (d_y <= 0) d_y = HH;
            d_y--;
        } else if (dir == W) {
            if (d_x >= (WW-1)) d_x = -1;
            d_x++;
        } else if (dir == N) {
            if (d_y >= (HH-1)) d_y = -1;
            d_y++;
        } else if (dir == E) {
            if (d_x <= 0) d_x = WW;
            d_x--;
        }
        rect.x = balld * d_x;
        rect.y = balld * d_y;
        rect.w = balld;
        rect.h = balld;
        col = SDL_MapRGB(sdlstate->screen->format, 255*(MAX_LEN-i)/MAX_LEN, 0, 0);
        SDL_FillRect(sdlstate->screen, &rect, col);
        dir_ >>= 2;
    }

    rect.x = balld * state.f_x;
    rect.y = balld * state.f_y;
    rect.w = balld;
    rect.h = balld;
    col = SDL_MapRGB(sdlstate->screen->format, 0, 255, 0);
    SDL_FillRect(sdlstate->screen, &rect, col);
}
int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return 1;
    atexit(SDL_Quit);
    SDL_Window *sdlWindow = 0;
    SDL_Renderer *sdlRenderer = 0;
    SDL_Texture *sdlTexture = 0;
    int z = 4;
    int w = 160 * z, h = 144 * z, bpp = 32;
    SDL_CreateWindowAndRenderer(w, h, 0, &sdlWindow, &sdlRenderer);
    SDL_Surface *screen = SDL_CreateRGBSurface(0, w, h, bpp,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    sdlstate_t sdlstate = { .w = w, .h = h, .bpp = bpp, .z = z, .screen = screen};

    int quit = 0;
    while (!quit) {
        static state_t state = {
            .dead = 0,
            .f_y = 3, .f_x = 5,
            .len = 2,
            .h_y = 4, .h_x = 3,
            .dir = 0x00000000005
        };

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                    break;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    state.dir = (state.dir & ~0x3) | N;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    state.dir = (state.dir & ~0x3) | S;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    state.dir = (state.dir & ~0x3) | W;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    state.dir = (state.dir & ~0x3) | E;
                }
            }
        }
        if (quit)
            break;

        uint64_t data = state_to_data(state);
        data = evolve(data);
        state = data_to_state(data);
        sdldraw(state, &sdlstate);

        SDL_UpdateTexture(sdlTexture, NULL, screen->pixels, screen->pitch);
        //SDL_RenderClear(sdlRenderer);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);

        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(400);
    }
    return 0;
}
