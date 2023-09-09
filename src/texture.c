#include "texture.h"
#include "platform.h"
#include "meth.h"

#include <string.h>
#include <stdio.h>

texture loadBMP(const u8 *file) {
    texture ret = {0};
    const u8 *pixArray = (u8 *)file + *(u32 *)((u8 *)file + 10);
    const int width = *(int *)((u8 *)file + 18);
    const int height = *(int *)((u8 *)file + 22);
    const int bpp = *(u16 *)((u8 *)file + 28) / 8;
    const u32 compMet = *(u32 *)((u8 *)file + 30);
    printf("\nbpp: %d\n", bpp);
    ret.width = width < 0 ? -width : width;
    ret.height = height < 0 ? -height : height;
    ret.buffer = allocMem(sizeof(color) * ret.width * ret.height);
    color *cur = ret.buffer + ret.width * (ret.height - 1);
    const int padding = (4 - ((bpp * ret.width) % 4)) % 4;
    for(int y = 0; y < ret.height; y++) {
        for(int x = 0; x < ret.width; x++) {
            switch(bpp) {
                case 3:
                    cur->b = *pixArray++;
                    cur->g = *pixArray++;
                    cur->r = *pixArray++;
                    cur->a = 255;
                    break;
                case 4:
                    cur->b = *pixArray++;
                    cur->g = *pixArray++;
                    cur->r = *pixArray++;
                    // BI_ALPHABITFIELDS = 6
                    if(compMet == 6) {
                        cur->a = *pixArray++;
                    }
                    else {
                        cur->a = 255;
                        pixArray++;
                    }
                    break;
            }
            cur++;
        }
        pixArray += padding;
        cur -= ret.width * 2;
    }
    return ret;
}

void inflate(const u8 *data, void *out) {
    u64 outIdx = 0;
    const u8 *idx = data;
    u8 bitIdx = 0;
    u8 bfinal, btype;
    idx += 2;
#define NEXTBIT ((bitIdx > 7 ? *++idx >> ((bitIdx = (bitIdx + 1) & 7) - 1) : *idx >> bitIdx++) & 1)
    do {
        bfinal = NEXTBIT;
        btype = NEXTBIT;
        btype |= NEXTBIT << 1;
        if(btype == 0b00) {
            u16 len, nlen;
            if(bitIdx > 7) {
                bitIdx &= 7;
            }
            else {
                idx++;
            }
            bitIdx = 0;
            len = *(u16 *)idx;
            idx += 2;
            nlen = *(u16 *)idx;
            idx += 2;
            if((len ^ nlen) == 0xFFFF) {
                memcpy(((char *)out) + outIdx, idx, len);
                outIdx += len;
                idx += len;
            }
            else {
                idx++;
            }
            continue;
        }
        u16 tree[16][288] = {0};
        u16 codeStart[16] = {0};
        u16 blCount[16] = {0};
        u16 distTree[16][32] = {0};
        u16 distCodeStart[16] = {0};
        u16 distBLCount[16] = {0};
        u16 code = 0;
        u16 val = 0;
        u16 extra = 0;
        if(btype == 0b10) {
            static const u8 codeLengthCodes[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
            u8 codeLengthTree[8][19] = {0};
            u8 codeLengthCodeStart[8] = {0};
            u8 codeLengthBLCount[8] = {0};
            u16 prevLength = 0;
            u8 hLit = 0, hDist = 0, hCLen = 0;
            for(int i = 0; i < 5; i++) hLit = hLit | (NEXTBIT << i);
            for(int i = 0; i < 5; i++) hDist = hDist | (NEXTBIT << i);
            for(int i = 0; i < 4; i++) hCLen = hCLen | (NEXTBIT << i);
            for(int i = 0; i < (hCLen + 4); i++) {
                int codeLengthCodeLength = 0;
                for(int j = 0; j < 3; j++) codeLengthCodeLength = codeLengthCodeLength | (NEXTBIT << j);
                codeLengthTree[codeLengthCodeLength][codeLengthBLCount[codeLengthCodeLength]++] = codeLengthCodes[i];
            }
            codeLengthBLCount[0] = 0;
            for(int i = 1; i < 8; i++) {
                codeLengthCodeStart[i] = (codeLengthCodeStart[i - 1] + codeLengthBLCount[i - 1]) << 1;
                for(int j = 0; j < codeLengthBLCount[i]; j++) {
                    int min = j;
                    int tmp;
                    for(int k = j + 1; k < codeLengthBLCount[i]; k++) {
                        if(codeLengthTree[i][k] < codeLengthTree[i][min]) min = k;
                    }
                    tmp = codeLengthTree[i][j];
                    codeLengthTree[i][j] = codeLengthTree[i][min];
                    codeLengthTree[i][min] = tmp;
                }
            }
            for(int i = 0; i < hLit + 257; i++) {
                code = 0;
                val = 31;
                for(int j = 1; j < 8; j++) {
                    code = (code << 1) | NEXTBIT;
                    if(code >= codeLengthCodeStart[j] && code < codeLengthCodeStart[j] + codeLengthBLCount[j]) {
                        val = codeLengthTree[j][code - codeLengthCodeStart[j]];
                        break;
                    }
                }
                if(val <= 15) {
                    prevLength = val;
                    tree[val][blCount[val]++] = i;
                }
                else if(val == 16) {
                    extra = NEXTBIT;
                    extra |= NEXTBIT << 1;
                    for(int j = 0; j < 3 + extra; j++) tree[prevLength][blCount[prevLength]++] = i++;
                    i--;
                }
                else if(val == 17) {
                    extra = NEXTBIT;
                    extra |= NEXTBIT << 1;
                    extra |= NEXTBIT << 2;
                    for(int j = 0; j < 3 + extra; j++) tree[0][blCount[0]++] = i++;
                    i--;
                }
                else if(val == 18) {
                    extra = 0;
                    for(int j = 0; j < 7; j++) extra = extra | (NEXTBIT << j);
                    for(int j = 0; j < 11 + extra; j++) tree[0][blCount[0]++] = i++;
                    i--;
                }
            }
            for(int i = 0; i < hDist + 1; i++) {
                code = 0;
                for(int j = 1; j < 8; j++) {
                    code = (code << 1) | NEXTBIT;
                    if(code >= codeLengthCodeStart[j] && code < codeLengthCodeStart[j] + codeLengthBLCount[j]) {
                        val = codeLengthTree[j][code - codeLengthCodeStart[j]];
                        break;
                    }
                }
                if(val <= 15) {
                    prevLength = val;
                    distTree[val][distBLCount[val]++] = i;
                }
                else if(val == 16) {
                    extra = NEXTBIT;
                    extra |= NEXTBIT << 1;
                    for(int j = 0; j < 3 + extra; j++) distTree[prevLength][distBLCount[prevLength]++] = i++;
                    i--;
                }
                else if(val == 17) {
                    extra = NEXTBIT;
                    extra |= NEXTBIT << 1;
                    extra |= NEXTBIT << 2;
                    for(int j = 0; j < 3 + extra; j++) distTree[0][distBLCount[0]++] = i++;
                    i--;
                }
                else if(val == 18) {
                    extra = 0;
                    for(int j = 0; j < 7; j++) extra = extra | (NEXTBIT << j);
                    for(int j = 0; j < 11 + extra; j++) distTree[0][distBLCount[0]++] = i++;
                    i--;
                }
            }
        }
        else if(btype == 0b01) {
            // fixed Huffman codes
            blCount[7] = 24;
            codeStart[7] = 0;
            blCount[8] = 152;
            codeStart[8] = 48;
            blCount[9] = 112;
            codeStart[9] = 400;
            for(int i = 0; i <= 23; i++) {
                tree[7][i] = i + 256;
            }
            for(int i = 0; i <= 143; i++) {
                tree[8][i] = i;
            }
            for(int i = 144; i <= 151; i++) {
                tree[8][i] = i + 136;
            }
            for(int i = 0; i <= 111; i++) {
                tree[9][i] = i + 144;
            }
            distBLCount[5] = 32;
            for(int i = 0; i < 32; i++) {
                distTree[5][i] = i;
            }
        }
        blCount[0] = 0;
        for(int i = 1; i < 16; i++) {
            codeStart[i] = (codeStart[i - 1] + blCount[i - 1]) << 1;
        }
        distBLCount[0] = 0;
        for(int i = 1; i < 16; i++) {
            distCodeStart[i] = (distCodeStart[i - 1] + distBLCount[i - 1]) << 1;
        }
        while(1) {
            code = 0;
            for(int i = 1; i < 16; i++) {
                code = (code << 1) | NEXTBIT;
                if(code >= codeStart[i] && code < codeStart[i] + blCount[i]) {
                    val = tree[i][code - codeStart[i]];
                    break;
                }
            }
            if(val <= 255) {
                ((u8 *)out)[outIdx++] = val;
            }
            else if(val == 256) {
                break;
            }
            else if(val <= 285) {
                u16 length = 0;
                u16 distance = 0;
                u8 extraBits = 0;
                if(val <= 260) {
                    length = val - 257 + 3;
                }
                else if(val <= 284) {
                    length = 7;
                    for(int i = 0; i <= 5; i++) {
                        if(val <= 264 + i * 4) {
                            extraBits = i;
                            length += (val - (261 + i * 4)) << i;
                            break;
                        }
                        length += 4 << i;
                    }
                }
                else if(val == 285) {
                    length = 258;
                }
                extra = 0;
                for(int i = 0; i < extraBits; i++) extra = extra | (NEXTBIT << i);
                length += extra;
                code = 0;
                for(int i = 1; i < 16; i++) {
                    code = (code << 1) | NEXTBIT;
                    if(code >= distCodeStart[i] && code < distCodeStart[i] + distBLCount[i]) {
                        val = distTree[i][code - distCodeStart[i]];
                        break;
                    }
                }
                if(val <= 1) {
                    distance = val + 1;
                    extraBits = 0;
                }
                else {
                    distance = 3;
                    for(int i = 0; i <= 13; i++) {
                        if(val <= 3 + i * 2) {
                            extraBits = i;
                            distance += (val - (2 + i * 2)) << i;
                            break;
                        }
                        distance += 2 << i;
                    }
                }
                extra = 0;
                for(int i = 0; i < extraBits; i++) extra = extra | (NEXTBIT << i);
                distance += extra;
                for(int i = 0; i < length; i++) {
                    ((u8 *)out)[outIdx] = ((u8 *)out)[outIdx - distance];
                    outIdx++;
                }
            }
        }
    } while(!bfinal);
#undef NEXTBIT
}

typedef struct {
    u32 width;
    u32 height;
    u8 bitDepth;
    u8 colorType;
    u8 compression;
    u8 filter;
    u8 interlace;
} png_IHDR;

int paeth(int a, int b, int c) {
    int p = a + b - c;
    int pa = p - a;
    if(pa < 0) pa = -pa;
    int pb = p - b;
    if(pb < 0) pb = -pb;
    int pc = p - c;
    if(pc < 0) pc = -pc;
    if(pa <= pb && pa <= pc) return a;
    else if(pb <= pc) return b;
    else return c;
}

texture loadPNG(const u8 *file) {
    texture ret = {0};
    void *data = 0;
    u64 dataOffset = 0;
    void *decodedData;
    const u8 *idx = file;
    u32 chunkLength;
    char *chunkType;
    png_IHDR *ihdr;
    idx += 8;
    while(1) {
        chunkLength = swapInt32(*(u32 *)idx);
        idx += 4;
        chunkType = (char *)idx;
        idx += 4;
        if(!strncmp(chunkType, "IHDR", 4)) {
            ihdr = (png_IHDR *)idx;
            ret.width = swapInt32(ihdr->width);
            ret.height = swapInt32(ihdr->height);
            ret.buffer = allocMem(ret.width * ret.height * 4);
        }
        else if(!strncmp(chunkType, "IDAT", 4)) {
            data = reAllocMem(data, dataOffset + chunkLength);
            memcpy(((char *)data) + dataOffset, idx, chunkLength);
            dataOffset += chunkLength;
        }
        else if(!strncmp(chunkType, "IEND", 4)) {
            break;
        }
        idx += chunkLength + 4;
    }
    if(ret.buffer && data) {
        decodedData = allocMem(ret.height * (4 * ret.width + 1));
        inflate(data, decodedData);
        for(int y = 0; y < ret.height; y++) {
            u8 *mid = (u8 *)decodedData + y * (ret.width * 4 + 1);
            u8 *left = (u8 *)&ret.buffer[y * ret.width] - 4;
            u8 *topLeft = (u8 *)&ret.buffer[(y - 1) * ret.width] - 4;
            u8 *top = topLeft + 4;
            u8 *out = left + 4;
            u8 filterMethod = *mid++;
            for(int x = 0; x < ret.width * 4; x++) {
                switch(filterMethod) {
                    case 0:
                        *out = *mid;
                        break;
                    case 1:
                        *out = *mid + (x >= 4 ? *left : 0);
                        break;
                    case 2:
                        *out = *mid + (y ? *top : 0);
                        break;
                    case 3:
                        *out = *mid + ((x >= 4 ? *left : 0) + (y ? *top : 0)) / 2;
                        break;
                    case 4:
                        *out = *mid + paeth((x >= 4 ? *left : 0), y ? *top : 0, y && x >= 4 ? *topLeft : 0);
                        break;
                }
                out++;
                mid++;
                left++;
                top++;
                topLeft++;
            }
        }
        freeMem(decodedData);
    }
    if(data) {
        freeMem(data);
    }
    return ret;
}

texture loadTextureFile(const char *path) {
    static const u8 pngHeader[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    u64 fileSize;
    u8 *file = loadFile(path, &fileSize);
    texture ret = {0};
    if(file) {
        if(!strncmp((char *)file, "BM", 2)) {
            ret = loadBMP(file);
        }
        else if (!strncmp((char *)file, (char *)pngHeader, 8)){
            ret = loadPNG(file);
        }
    }
    return ret;
}
