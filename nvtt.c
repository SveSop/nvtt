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
typedef int (*NvAPI_GPU_GetArchInfo_t)(int *handle, NV_GPU_ARCH_INFO *pGpuArchInfo);
typedef int (*NvAPI_Disp_GetHdrCapabilities_t)(int *handle, NV_HDR_CAPABILITIES *pHdrCapabilities);
typedef int (*NvAPI_GPU_GetRamType_t)(int *handle, int *memtype);
typedef int (*NvAPI_GPU_GetBusType_t)(int *handle, int *pBusType);
typedef int (*NvAPI_GPU_GetDynamicPstatesInfoEx_t)(int *handle, NV_GPU_DYNAMIC_PSTATES_INFO_EX *pDynamicPstatesInfoEx);

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
NvAPI_GPU_GetArchInfo_t NvArch = 0;
NvAPI_Disp_GetHdrCapabilities_t NvHDR = 0;
NvAPI_GPU_GetRamType_t NvGetMemType = 0;
NvAPI_GPU_GetBusType_t NvBus = 0;
NvAPI_GPU_GetDynamicPstatesInfoEx_t NvPstate = 0;

int main(int argc, char **argv)
{
    int nGPU=0, pGpuType=0, memsize=0, memtype=0, pBusType=0;
    int *hdlGPU[64]={0}, *hdlDisp[32]={0};
    char sysname[64]={0};
    NvU32 pBusId, pDeviceId, pSubSystemId, pRevisionId, pExtDeviceId;
    NV_DISPLAY_DRIVER_VERSION pVersion;
    pVersion.version = NV_DISPLAY_DRIVER_VERSION_VER;
    NV_GPU_ARCH_INFO pGpuArchInfo;
    pGpuArchInfo.version = NV_GPU_ARCH_INFO_VER;
    NV_HDR_CAPABILITIES pHdrCapabilities;
    pHdrCapabilities.version = NV_HDR_CAPABILITIES_VER;
    NV_GPU_DYNAMIC_PSTATES_INFO_EX pDynamicPstatesInfoEx;
    pDynamicPstatesInfoEx.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;

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
    NvArch          = NvQueryInterface(0xd8265d24);
    NvHDR           = NvQueryInterface(0x84f2a8df);
    NvGetMemType    = NvQueryInterface(0x57F7CAAC);
    NvBus           = NvQueryInterface(0x1BB18724);
    NvPstate        = NvQueryInterface(0x60ded2ed);

    NvInit();
    NvEnumGPUs(hdlGPU, &nGPU);
    // Get adaptername for board. Simple text string.
    if(NvGetName && NvGetName(hdlGPU[0], sysname) == NVAPI_OK){
      printf("Name: %s\n", sysname);
    }
    else printf("NvAPI_GPU_GetFullName not available!\n");
    // Get BusID
    if(BusId && BusId(hdlGPU[0], &pBusId) == NVAPI_OK){
      printf("BusId: %ld\n", pBusId);
    }
    else printf("NvAPI_GPU_GetBusId not available!\n");
    // GPU Type.
    if(NvGPUType && NvGPUType(hdlGPU[0], &pGpuType) == NVAPI_OK){
      switch(pGpuType){
        case 1:      printf("\nGPU Type: CPU\n"); break;
        case 2:      printf("\nGPU Type: Discrete\n"); break;
        default:     printf("\nGPU Type: Unknown\n"); break;
      }
    }
    else printf("NvAPI_GPU_GetBusId not available!\n");
    // Driver version and branchstring. nVidia uses rxxx_xx for "branch". This is afaik not
    // reported very well in Linux drivers, so it will usually be a "faked" number.
    if(NvDriver && NvDriver(hdlDisp[0], &pVersion) == NVAPI_OK){
      printf("Display Driver Branch: %s\n", pVersion.szBuildBranchString);
      printf("Display Driver Version: %ld\n", pVersion.drvVersion);
      printf("Adapter name: %s\n", pVersion.szAdapterString);
    }
    else{
      printf("Unable to obtain display driver info and adaptername!\n");
    }
    // Onboard memory size for Graphics Adapter
    if(NvGetMemSize && NvGetMemSize(hdlGPU[0], &memsize) == NVAPI_OK){
      printf("VRAM: %dMB\n", memsize/1024);
    }
    else printf("NvAPI_GPU_GetPhysicalFrameBufferSize not available!\n");
    // Get device and vendor ID for adapter. Susbsystem ID and revision ID requires nvml usage
    // since this does not seem to be reported by vulkan.
    if(NvPCIID && NvPCIID(hdlGPU[0], &pDeviceId, &pSubSystemId, &pRevisionId, &pExtDeviceId) == NVAPI_OK){
      u_int ven=(pDeviceId >> 16);
      short dev=pDeviceId;
      printf("DeviceID: %04X-%04X\n", dev, ven);
      u_int subven=(pSubSystemId >> 16);
      short subdev=pSubSystemId;
      printf("Subdevice ID: %04X-%04X\n", subdev, subven);
      printf("Revision ID: %lX\n", pRevisionId);
      printf("Ext Device ID: %lX\n", pExtDeviceId);
    }
    else printf("NvAPI_GPU_GetPCIIdentifiers not available!\n");
    // Architecture of adapter. WIP needs human readable conversion instead of just decimal numbers.
    if(NvArch && NvArch(hdlGPU[0], &pGpuArchInfo) == NVAPI_OK){
      printf("Architecture: %ld\n", pGpuArchInfo.architecture);
      printf("Revision: %ld\n", pGpuArchInfo.revision);
    }
    else printf("NvAPI_GPU_GetArchInfo not available!\n");
    // Check for HDR capabilities. DXVK does currently not support this.
    if(NvHDR && NvHDR(hdlDisp[0], &pHdrCapabilities) == NVAPI_OK){
      printf("GPU HDR Capabilities:\n");
      printf("  ST2084EotfSupport:          %s\n", pHdrCapabilities.isST2084EotfSupported?"true":"false");
      printf("  TraditionalHdrGamma:        %s\n", pHdrCapabilities.isTraditionalHdrGammaSupported?"true":"false");
      printf("  EdrSupport:                 %s\n", pHdrCapabilities.isEdrSupported?"true":"false");
      printf("  ExpandDefaultHdrParameters: %s\n", pHdrCapabilities.driverExpandDefaultHdrParameters?"true":"false");
      printf("  TraditionalSdrGammaSupport; %s\n", pHdrCapabilities.isTraditionalSdrGammaSupported?"true":"false");
      printf("  DolbyVisionSupport:         %s\n", pHdrCapabilities.isDolbyVisionSupported?"true":"false");
    }
    else printf("NvAPI_Disp_GetHdrCapabilities not supported!\n");
    // Get memory type for onboard memory on adapter.
    // This needs some tuning to pick up all types of GDDR ram, so not very useful atm.
    // Currently not implemented in dxvk-nvapi
    if(NvGetMemType && NvGetMemType(hdlGPU[0], &memtype) == NVAPI_OK){
      printf("Onboard memory type: GDDR%d\n", memtype<=8?5:6);
    }
    else printf("NvAPI_GPU_GetRamType is not available!\n");
    // Get type of bus interface for adapter.
    if(NvBus && NvBus(hdlGPU[0], &pBusType) == NVAPI_OK){
      switch(pBusType){
        case 0: printf("Undefined GPU Bus\n"); break;
        case 1: printf("PCI Bus\n"); break;
        case 2: printf("AGP Bus\n"); break;
        case 3: printf("PCIexpress Bus\n"); break;
        default: printf("Unknown GPU Bus\n"); break;
      }
    }
    else printf("NvAPI_GPU_GetBusType is not available!\n");
    // This gets GPU and memory controller utilization
    if(NvPstate && NvPstate(hdlGPU[0], &pDynamicPstatesInfoEx) == NVAPI_OK){
      printf("GPU utilization: %ld%%\n", pDynamicPstatesInfoEx.utilization[0].percentage);
      printf("Memory controller utilization: %ld%%\n", pDynamicPstatesInfoEx.utilization[1].percentage);
    }
    else printf("NvAPI_GPU_GetDynamicPstatesInfoEx not available!\n");

    NvUnload();

    printf("Press ENTER to Continue\n");
    getchar();
    return 0;
}
