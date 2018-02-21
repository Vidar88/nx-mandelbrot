#include <string.h>
#include <stdio.h>
#include <switch.h>


#include "../common/common.h"

uint8_t* g_framebuf;
u32 g_framebuf_width;

void mandelbrot() {
    int row, col;
    int width = 1280;
    int height = 720;
    int max = 100;

    for (row = 0; row < width; row++) {
        for (col = 0; col < height; col++) {
            double c_re = (row - width/2.0)*4.0/width;
            double c_im = (col - height/2.0)*4.0/width;
            double x = 0, y = 0;
            int iteration = 0;

            while ( x * x + y * y <= 4 && iteration < max) {
                double x_new = x * x - y * y + c_re;
                y = 2 * x * y + c_im;
                x = x_new;
                iteration++;
            }
            if (iteration < max) DrawPixelRaw(row, col, MakeColor(iteration % 4 * 64, iteration % 8 * 32, iteration % 16 * 16, 255));
            else DrawPixelRaw(row, col, MakeColor(0, 0, 0, 255));
        }
    }
}

int main(int argc, char **argv)
{
    //Enable max-1080p support. Remove for 720p-only resolution.
    //gfxInitResolutionDefault();

    gfxInitDefault();

    //Set current resolution automatically depending on current/changed OperationMode. Only use this when using gfxInitResolution*().
    //gfxConfigureAutoResolutionDefault(true);

    g_framebuf = gfxGetFramebuffer(&g_framebuf_width, NULL);
    memset(g_framebuf, 237, gfxGetFramebufferSize());

    mandelbrot();
    DrawText(tahoma24, 40, 30, MakeColor(255, 255, 255, 255), "Mandelbrot demo by Vidar");
    DrawText(tahoma24, 1050, 30, MakeColor(255, 255, 255, 255), "Press B to exit");

    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();


        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_B) break; // break in order to return to hbmenu

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}
