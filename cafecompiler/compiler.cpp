#include "gx2_definitions.h"
#include "cafe_glsl_compiler.h" // internal

#include "CafeGLSLCompiler.h" // the public header

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

void DebugLog(const char *format, ...);

char* loadFile(const char* fileName){
    FILE* f = fopen(fileName, "rb");
    if(!f) return NULL;

    // Check for file size
    fseek(f, 0L, SEEK_END);
    size_t sz = ftell(f);
    fseek(f, 0L, SEEK_SET);

    // Alloc memory
    char* data = (char*) malloc(sz);
    if(!data) return NULL;
    fread(data, sz, 1, f);
    fclose(f);

    return data;
}


int main(int argc, char* argv[]){
    DebugLog("Initialize compiler...\n");
    if (!GLSL_Init())
    {
        DebugLog("Failed\n");
        return -1;
    }

    if(argc < 3){
        fprintf(stderr, "Usage: %s vertexShader.vs pixelShader.fs\n", argv[0]);
        return -1;
    }

    char* vSource = loadFile(argv[1]);
    char* pSource = loadFile(argv[2]);

    if(!vSource || !pSource){
        fprintf(stderr, "Cannot open shader sources!\n");
        return -2;
    }

    
    char infoLogBuffer[1024];

    DebugLog("Compile vertex shader...\n");
    GX2VertexShader* vs = GLSL_CompileVertexShader(vSource, infoLogBuffer, 1024, GLSL_COMPILER_FLAG_GENERATE_DISASSEMBLY);


    DebugLog("Compile pixel shader...\n");
    GX2PixelShader* ps = GLSL_CompilePixelShader(pSource, infoLogBuffer, 1024, GLSL_COMPILER_FLAG_GENERATE_DISASSEMBLY);


    free(vSource);
    free(pSource);

    return 0;
}