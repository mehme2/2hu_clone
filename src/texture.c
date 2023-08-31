#include "texture.h"
#include "platform.h"

texture loadBMP(const char *path) {
    texture ret = {0};
    u64 size;
    void *file = loadFile(path, &size);
    if(file) {
        u8 *pixArray = (u8 *)file + *(u32 *)((u8 *)file + 10);
        int width = *(int *)((u8 *)file + 18);
        int height = *(int *)((u8 *)file + 22);
        ret.width = width < 0 ? -width : width;
        ret.height = height < 0 ? -height : height;
        ret.buffer = allocMem(sizeof(color) * ret.width * ret.height);
        //Color *Current = Return.Buffer + Return.Width * (Return.Height - 1);
        color *cur = ret.buffer;
        int padding = 4 - (3 * ret.width) % 4;
        for(int y = 0; y < ret.height; y++) {
            for(int x = 0; x < ret.width; x++) {
                cur->b = *pixArray++;
                cur->g = *pixArray++;
                cur->r = *pixArray++;
                cur++;
            }
            pixArray += padding;
            //Current -= Return.Width * 2;
        }
        freeFile(file);
    }
    return ret;
}
