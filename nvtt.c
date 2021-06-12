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
typedef int (*NvAPI_GPU_GetGPUType_t)(int *handle, int *pGpuType);
typedef int (*NvAPI_GetDisplayDriverVersion_t)(int *handle, NV_DISPLAY_DRIVER_VERSION *pVersion);

NvAPI_QueryInterface_t NvQueryInterface = 0;
NvAPI_Initialize_t NvInit = 0;
NvAPI_Unload_t NvUnload = 0;
NvAPI_EnumPhysicalGPUs_t NvEnumGPUs = 0;
NvAPI_GPU_GetFullName_t NvGetName = 0;
NvAPI_GPU_GetBusId_t BusId = 0;
NvAPI_GPU_GetGPUType_t NvGPUType = 0;
NvAPI_GetDisplayDriverVersion_t NvDriver = 0;

int main(int argc, char **argv)
{
    int nGPU=0, pGpuType=0;
    int *hdlGPU[64]={0}, *hdlDisp[32]={0};
    char sysname[64]={0};
    NvU32 pBusId;
    NV_DISPLAY_DRIVER_VERSION pVersion;
    pVersion.version = NV_DISPLAY_DRIVER_VERSION_VER;

    NvAPI_Status ret = NVAPI_OK;

    NvQueryInterface = (void*)GetProcAddress(LoadLibrary("nvapi.dll"), "nvapi_QueryInterface");
    NvInit          = NvQueryInterface(0x0150E828);
    NvUnload        = NvQueryInterface(0xD22BDD7E);
    NvEnumGPUs      = NvQueryInterface(0xE5AC921F);
    NvGetName       = NvQueryInterface(0xCEEE8E9F);
    BusId           = NvQueryInterface(0x1BE0B8E5);
    NvGPUType       = NvQueryInterface(0xc33baeb1);
    NvDriver        = NvQueryInterface(0xF951A4D1);

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
    ret = NvGPUType(hdlGPU[0], &pGpuType);
    if(ret == NVAPI_OK){
      switch(pGpuType){
        case 1:      printf("\nGPU Type: CPU\n"); break;
        case 2:      printf("\nGPU Type: Discrete\n"); break;
        default:     printf("\nGPU Type: Unknown\n"); break;
      }
    }
    ret = NvDriver(hdlDisp[0], &pVersion);
    if(ret == NVAPI_OK){
      printf("Display Driver Branch: %s\n", pVersion.szBuildBranchString);
      printf("Display Driver Version: %ld\n", pVersion.drvVersion);
      printf("Adapter name: %s\n", pVersion.szAdapterString);
    }
    else{
      printf("Unable to obtain display driver info and adaptername!\n");
    }
    NvUnload();

    printf("Press ENTER to Continue\n");
    getchar();
    return 0;
}
