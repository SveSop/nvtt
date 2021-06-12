#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include "nvapi.h"

typedef void *(*NvAPI_QueryInterface_t)(unsigned int offset);
typedef int (*NvAPI_Initialize_t)();
typedef int (*NvAPI_Unload_t)();
typedef int (*NvAPI_EnumPhysicalGPUs_t)(int **handles, int *count);
typedef int (*NvAPI_GPU_GetFullName_t)(int *handle, char *sysname);
typedef int (*NvAPI_GPU_GetBusId_t)(int *handle, NvU32 *pBusId);

NvAPI_QueryInterface_t NvQueryInterface = 0;
NvAPI_Initialize_t NvInit = 0;
NvAPI_Unload_t NvUnload = 0;
NvAPI_EnumPhysicalGPUs_t NvEnumGPUs = 0;
NvAPI_GPU_GetFullName_t NvGetName = 0;
NvAPI_GPU_GetBusId_t BusId = 0;

int main(int argc, char **argv)
{
    int nGPU=0;
    int *hdlGPU[64]={0};
    char sysname[64]={0};
    NvU32 pBusId;

    NvAPI_Status ret = NVAPI_OK;

    NvQueryInterface = (void*)GetProcAddress(LoadLibrary("nvapi.dll"), "nvapi_QueryInterface");
    NvInit          = NvQueryInterface(0x0150E828);
    NvUnload        = NvQueryInterface(0xD22BDD7E);
    NvEnumGPUs      = NvQueryInterface(0xE5AC921F);
    NvGetName       = NvQueryInterface(0xCEEE8E9F);
    BusId           = NvQueryInterface(0x1BE0B8E5);

    NvInit();
    NvEnumGPUs(hdlGPU, &nGPU);
    ret = NvGetName(hdlGPU[0], sysname);
    if(ret == NVAPI_OK){
      printf("Name: %s\n", sysname);
    }
    ret = BusId(hdlGPU[0], &pBusId);
    if(ret == NVAPI_OK){
      printf("BusId: %ld\n", pBusId);
    }
    NvUnload();

    printf("Press ENTER to Continue\n");
    getchar();
    return 0;
}
