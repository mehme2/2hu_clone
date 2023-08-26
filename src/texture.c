#include "texture.h"
#include "platform.h"

Texture LoadBMP(const char *Filename) {
    Texture Return = {0};
    u64 FileSize;
    void *File = LoadFile(Filename, &FileSize);
    if(File) {
        u8 *PixelArray = (u8 *)File + *(u32 *)((u8 *)File + 10);
        int Width = *(int *)((u8 *)File + 18);
        int Height = *(int *)((u8 *)File + 22);
        Return.Width = Width < 0 ? -Width : Width;
        Return.Height = Height < 0 ? -Height : Height;
        Return.Buffer = AllocateMemory(sizeof(Color) * Return.Width * Return.Height);
        Color *Current = Return.Buffer + Return.Width * (Return.Height - 1);
        int Padding = 4 - (3 * Return.Width) % 4;
        for(int y = 0; y < Return.Height; y++) {
            for(int x = 0; x < Return.Width; x++) {
                Current->Channels.b = *PixelArray++;
                Current->Channels.g = *PixelArray++;
                Current->Channels.r = *PixelArray++;
                Current++;
            }
            PixelArray += Padding;
            Current -= Return.Width * 2;
        }
        FreeFile(File);
    }
    return Return;
}
