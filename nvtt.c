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
typedef int (*NvAPI_GPU_GetPhysicalFrameBufferSize_t)(int *handle, int *memsize);
typedef int (*NvAPI_GPU_GetPCIIdentifiers_t)(int *handle, NvU32 *pDeviceId, NvU32 *pSubSystemId, NvU32 *pRevisionId, NvU32 *pExtDeviceId);

NvAPI_QueryInterface_t NvQueryInterface = 0;
NvAPI_Initialize_t NvInit = 0;
NvAPI_Unload_t NvUnload = 0;
NvAPI_EnumPhysicalGPUs_t NvEnumGPUs = 0;
NvAPI_GPU_GetFullName_t NvGetName = 0;
NvAPI_GPU_GetBusId_t BusId = 0;
NvAPI_GPU_GetGPUType_t NvGPUType = 0;
NvAPI_GetDisplayDriverVersion_t NvDriver = 0;
NvAPI_GPU_GetPhysicalFrameBufferSize_t NvGetMemSize = 0;
NvAPI_GPU_GetPCIIdentifiers_t NvPCIID = 0;

int main(int argc, char **argv)
{
    int nGPU=0, pGpuType=0, memsize=0;
    int *hdlGPU[64]={0}, *hdlDisp[32]={0};
    char sysname[64]={0};
    NvU32 pBusId, pDeviceId, pSubSystemId, pRevisionId, pExtDeviceId;
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
    NvGetMemSize    = NvQueryInterface(0x46FBEB03);
    NvPCIID         = NvQueryInterface(0x2ddfb66e);

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
    ret = NvGetMemSize(hdlGPU[0], &memsize);
    if(ret == NVAPI_OK){
      printf("VRAM: %dMB\n", memsize/1024);
    }
    ret = NvPCIID(hdlGPU[0], &pDeviceId, &pSubSystemId, &pRevisionId, &pExtDeviceId);
    if(ret == NVAPI_OK){
      u_int ven=(pDeviceId >> 16);
      short dev=pDeviceId;
      printf("DeviceID: %04X-%04X\n", dev, ven);
      u_int subven=(pSubSystemId >> 16);
      short subdev=pSubSystemId;
      printf("Subdevice ID: %04X-%04X\n", subdev, subven);
      printf("Revision ID: %lX\n", pRevisionId);
      printf("Ext Device ID: %lX\n", pExtDeviceId);
    }
    NvUnload();

    printf("Press ENTER to Continue\n");
    getchar();
    return 0;
}
