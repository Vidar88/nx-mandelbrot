#include <string.h>
#include <stdio.h>
#include <switch.h>


#include "../common/common.h"

#define WIDTH 1280
#define HEIGHT 720

#define START_POS   -0.5
#define START_ZOOM  (WIDTH * 0.25296875f)

#define BAIL_OUT        2.0
#define FLIPS           24

#define ZOOM_FACTOR     4

uint8_t* g_framebuf;
u32 g_framebuf_width;
int redraw = 1;
int offset = 0;
color_t colors[16];

void mandelbrot(int redraw) {
    int maxiter = 100;
    if (redraw == 1) {
        int row, col;
        for (row = 0; row < WIDTH; row++) {
            for (col = 0; col < HEIGHT; col++) {
                double c_re = (row - WIDTH / 2.0) * 4.0 / WIDTH;
                double c_im = (col - HEIGHT / 2.0) * 4.0 / WIDTH;
                double x = 0, y = 0;
                int iteration = 0;

                while ( x * x + y * y <= 4 && iteration < maxiter) {
                    double x_new = x * x - y * y + c_re;
                    y = 2 * x * y + c_im;
                    x = x_new;
                    iteration++;
                }
                if (iteration < maxiter) DrawPixelRaw(row, col, colors[iteration % 16]);
                else DrawPixelRaw(row, col, MakeColor(0, 0, 0, 255));
            }
        }
    }
}

void initColors() {
    colors[0] = MakeColor(66, 30, 15, 255);
    colors[1] = MakeColor(25, 7, 26, 255);
    colors[2] = MakeColor(9, 1, 47, 255);
    colors[3] = MakeColor(4, 4, 73, 255);
    colors[4] = MakeColor(0, 7, 100, 255);
    colors[5] = MakeColor(12, 44, 138, 255);
    colors[6] = MakeColor(24, 82, 177, 255);
    colors[7] = MakeColor(57, 125, 209, 255);
    colors[8] = MakeColor(134, 181, 229, 255);
    colors[9] = MakeColor(211, 236, 248, 255);
    colors[10] = MakeColor(241, 233, 191, 255);
    colors[11] = MakeColor(248, 201, 95, 255);
    colors[12] = MakeColor(255, 170, 0, 255);
    colors[13] = MakeColor(204, 128, 0, 255);
    colors[14] = MakeColor(153, 87, 0, 255);
    colors[15] = MakeColor(106, 52, 3, 255);
}

int main(int argc, char **argv)
{
    //Enable max-1080p support. Remove for 720p-only resolution.
    //gfxInitResolutionDefault();

    gfxInitDefault();

    appletSetScreenShotPermission(1);

    //Set current resolution automatically depending on current/changed OperationMode. Only use this when using gfxInitResolution*().
    //gfxConfigureAutoResolutionDefault(true);

    g_framebuf = gfxGetFramebuffer(&g_framebuf_width, NULL);
    memset(g_framebuf, 237, gfxGetFramebufferSize());

    initColors();

    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();


        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_B) break; // break in order to return to hbmenu
        if (kDown & KEY_PLUS) {
            offset++;
            redraw = 1;
        }
        if (kDown & KEY_MINUS) {
            offset--;
            redraw = 1;
        }

        mandelbrot(redraw);
        redraw = 0;

        DrawText(tahoma24, 40, 30, MakeColor(255, 255, 255, 255), "Mandelbrot demo by Vidar");
        DrawText(tahoma24, 1050, 30, MakeColor(255, 255, 255, 255), "Press B to exit");


        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}
