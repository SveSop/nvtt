#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include "nvapi.h"
#include "nvapi_xtra.h"

typedef void *(*NvAPI_QueryInterface_t)(unsigned int offset);
typedef int (*NvAPI_Initialize_t)();
typedef int (*NvAPI_Unload_t)();
typedef int (*NvAPI_EnumPhysicalGPUs_t)(int *handle, int *count);
typedef int (*NvAPI_EnumLogicalGPUs_t)(int *handle, int *count);
typedef int (*NvAPI_GetLogicalGPUFromPhysicalGPU_t)(int *physhandle, int *loghandle);
typedef int (*NvAPI_GetPhysicalGPUsFromLogicalGPU_t)(int *loghandle, int *physhandle, int *count);
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
typedef int (*NvAPI_GPU_GetThermalSettings_t)(int *handle, NvU32 sensorIndex, NV_GPU_THERMAL_SETTINGS *pThermalSettings);
typedef int (*NvAPI_GPU_GetTachReading_t)(int *handle, NvU32 *pValue);
typedef int (*NvAPI_GPU_GetVbiosVersionString_t)(int *handle, char *biosname);
typedef int (*NvAPI_GPU_GetAllClockFrequencies_t)(int *handle, NV_GPU_CLOCK_FREQUENCIES *pClkFreqs);
typedef int (*NvAPI_GPU_GetCoolerSettings_t)(int *handle, NvU32 coolerIndex, NV_GPU_COOLER_SETTINGS *pCoolerInfo);
typedef int (*NvAPI_SYS_GetDisplayDriverInfo_t)(NV_DISPLAY_DRIVER_INFO *pDriverInfo);
typedef int (*NvAPI_GPU_CudaEnumComputeCapableGpus_t)(NV_CUDA *pParam);
typedef int (*NvAPI_GetGPUIDfromPhysicalGPU_t)(int *handle, NvU32 *retHandle);
typedef int (*NvAPI_GetPhysicalGPUFromGPUID_t)(int *handle, NvU32 *retHandle);
typedef int (*NvAPI_GPU_GetCurrentPstate_t)(int *handle, NV_GPU_PERF_PSTATE_ID *pCurrentPstate);
typedef int (*NvAPI_GPU_GetPstates20_t)(int *handle, NV_GPU_PERF_PSTATES20_INFO *pPstatesInfo);

NvAPI_QueryInterface_t NvQueryInterface = 0;
NvAPI_Initialize_t NvInit = 0;
NvAPI_Unload_t NvUnload = 0;
NvAPI_EnumPhysicalGPUs_t NvEnumGPUs = 0;
NvAPI_EnumLogicalGPUs_t NvEnumLogGPUs = 0;
NvAPI_GetLogicalGPUFromPhysicalGPU_t NvGetLog = 0;
NvAPI_GetPhysicalGPUsFromLogicalGPU_t NvGetPhys = 0;
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
NvAPI_GPU_GetThermalSettings_t NvThermals = 0;
NvAPI_GPU_GetTachReading_t NvTach = 0;
NvAPI_GPU_GetVbiosVersionString_t NvGetBiosName = 0;
NvAPI_GPU_GetAllClockFrequencies_t NvClkfq = 0;
NvAPI_GPU_GetCoolerSettings_t NvCooler = 0;
NvAPI_SYS_GetDisplayDriverInfo_t NvDrvInfo = 0;
NvAPI_GPU_CudaEnumComputeCapableGpus_t NvCuda = 0;
NvAPI_GetGPUIDfromPhysicalGPU_t NvGpuUid = 0;
NvAPI_GetPhysicalGPUFromGPUID_t NvGpuPhys = 0;
NvAPI_GPU_GetCurrentPstate_t NvPerfState = 0;
NvAPI_GPU_GetPstates20_t NvPerf20 = 0;

int main(int argc, char **argv)
{
    int i=0, p=0, psID=0, nGPU=0, pGpuType=0, memsize=0, memtype=0, pBusType=0;
    int *hdlGPU[NVAPI_MAX_PHYSICAL_GPUS]={0}, *hdllGPU[NVAPI_MAX_LOGICAL_GPUS]={0}, *hdlDisp[NVAPI_MAX_DISPLAYS]={0};
    NV_GPU_PERF_PSTATE_ID pCurrentPstate;
    NvAPI_ShortString sysname, biosname;
    NvU32 pBusId=0, pDeviceId=0, pSubSystemId=0, pRevisionId=0, pExtDeviceId=0, pValue=0, retHandle=0, physHandle=0;
    NV_DISPLAY_DRIVER_VERSION pVersion;
    pVersion.version = NV_DISPLAY_DRIVER_VERSION_VER;
    NV_GPU_ARCH_INFO pGpuArchInfo;
    pGpuArchInfo.version = NV_GPU_ARCH_INFO_VER;
    NV_HDR_CAPABILITIES pHdrCapabilities;
    pHdrCapabilities.version = NV_HDR_CAPABILITIES_VER;
    NV_GPU_DYNAMIC_PSTATES_INFO_EX pDynamicPstatesInfoEx;
    pDynamicPstatesInfoEx.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
    NV_GPU_THERMAL_SETTINGS pThermalSettings;
    pThermalSettings.version = NV_GPU_THERMAL_SETTINGS_VER;
    NV_GPU_CLOCK_FREQUENCIES pClkFreqs;
    pClkFreqs.version = NV_GPU_CLOCK_FREQUENCIES_VER;
    NV_GPU_COOLER_SETTINGS pCoolerInfo;
    pCoolerInfo.version = NV_GPU_COOLER_SETTINGS_VER;
    NV_DISPLAY_DRIVER_INFO pDriverInfo;
    pDriverInfo.version = NV_DISPLAY_DRIVER_INFO_VER;
    NV_CUDA pParam;
    pParam.version = NV_CUDA_VER;
    NV_GPU_PERF_PSTATES20_INFO pPstatesInfo;
    pPstatesInfo.version = NV_GPU_PERF_PSTATES20_INFO_VER;

    NvQueryInterface = (void*)GetProcAddress(LoadLibrary("nvapi.dll"), "nvapi_QueryInterface");
    if(NvQueryInterface == NULL){
      printf("nvAPI interface not available!\n");
      return 0;
    }
    NvInit          = NvQueryInterface(0x0150E828);
    NvUnload        = NvQueryInterface(0xD22BDD7E);
    NvEnumGPUs      = NvQueryInterface(0xE5AC921F);
    NvEnumLogGPUs   = NvQueryInterface(0x48b3ea59);
    NvGetLog        = NvQueryInterface(0xadd604d1);
    NvGetPhys       = NvQueryInterface(0xaea3fa32);
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
    NvThermals      = NvQueryInterface(0xe3640a56);
    NvTach          = NvQueryInterface(0x5f608315);
    NvGetBiosName   = NvQueryInterface(0xA561FD7D);
    NvClkfq         = NvQueryInterface(0xdcb616c3);
    NvCooler        = NvQueryInterface(0xda141340);
    NvDrvInfo       = NvQueryInterface(0x721faceb);
    NvCuda          = NvQueryInterface(0x5786cc6e);
    NvGpuUid        = NvQueryInterface(0x6533ea3e);
    NvGpuPhys       = NvQueryInterface(0x5380ad1a);
    NvPerfState     = NvQueryInterface(0x927da4f6);
    NvPerf20        = NvQueryInterface(0x6ff81213);

    NvInit();
    NvEnumLogGPUs((void *)hdllGPU, &nGPU);
    printf("Logical  gpu's: %d\n", nGPU);
    if (nGPU > 0){
      for (i = 0; i < (nGPU); i++){
        printf("Logical  gpu handle: %p\n", hdllGPU[i]);
      }
    }
    NvEnumGPUs((void *)hdlGPU, &nGPU);
    printf("Physical gpu's: %d\n", nGPU);
    if (nGPU > 0){
      for (i = 0; i < (nGPU); i++){
        printf("Physical gpu handle: %p\n\n", hdlGPU[i]);
      }
    }

  void geninfo()
  // Probably needs some work, but assuming you cannot use 2 different nVidia drivers in
  // the same systeminstallation, it is "safe" to only report this once.
  {
    // Driver version and branchstring. nVidia uses rxxx_xx for "branch". This is afaik not
    // reported very well in Linux drivers, so it will usually be a "faked" number.
    if(NvDriver && NvDriver(hdlDisp[0], &pVersion) == NVAPI_OK){
      if(pVersion.drvVersion > 99999){
        printf("Display Driver Version: %ld.%.2ld.%.2ld\n", (pVersion.drvVersion / 10000), ((pVersion.drvVersion % 10000) / 100), (pVersion.drvVersion % 100));
      }
      else printf("Display Driver Version: %ld.%.2ld\n", (pVersion.drvVersion / 100), (pVersion.drvVersion % 100));
      printf("Display Driver Branch: %s\n", pVersion.szBuildBranchString);
      printf("Adapter name: %s\n", pVersion.szAdapterString);
    }
    else printf("Unable to obtain display driver info and adaptername!\n");
    // Get type of driver
    if(NvDrvInfo && NvDrvInfo(&pDriverInfo) == NVAPI_OK){
      if(pDriverInfo.bIsNVIDIAStudioPackage == 1) printf("Nvidia Studio branch.\n");
      else if(pDriverInfo.bIsNVIDIAGameReadyPackage == 1) printf("Nvidia GameReady branch.\n");
      else if(pDriverInfo.bIsNVIDIARTXProductionBranchPackage == 1) printf("Nvidia RTX Production branch.\n");
      else if(pDriverInfo.bIsNVIDIARTXNewFeatureBranchPackage == 1) printf("Nvidia RTX New Feature branch.\n");
    }
    else printf("NvAPI_SYS_GetDisplayDriverInfo not available!\n");
    // Check for HDR capabilities. DXVK does currently not support this. Is this "driver related only"?
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
    // Get Cuda capable GPU's
    if(NvCuda && NvCuda(&pParam) == NVAPI_OK){
      printf("\nCuda:\nNumber of Cuda adapters: %ld\n", pParam.gpu_count);
      for(int u = 0; u < (pParam.gpu_count); u++){
        printf("  ->Cuda GPU handle of adapter %d: %p\n", u, pParam.gpus[u].gpuHandle);
        printf("  ->Cuda GPUID of adapter %d: %ld\n", u, pParam.gpus[u].GetGPUIDfromPhysicalGPU);
      }
    }
    else printf("NvAPI_GPU_CudaEnumComputeCapableGpus is not available!\n");
  }

  void info()
  {
    // Get adaptername for board. Simple text string.
    if(NvGetName && NvGetName(hdlGPU[i], sysname) == NVAPI_OK){
      printf("Name: %s\n", sysname);
    }
    else printf("NvAPI_GPU_GetFullName not available!\n");
    // Adapter handle
    printf("GPU adapter handle: %p\n", hdlGPU[i]);
    // Get Logical GPU from Physical GPU
    if(NvGetLog && NvGetLog(hdlGPU[i], (void *)hdllGPU) == NVAPI_OK){
      printf("Logical GPU Handle: %p, from Physical GPU Handle: %p\n", hdllGPU[i], hdlGPU[i]);
    }
    else printf("NvAPI_GetLogicalGPUFromPhysicalGPU not available!\n");
    // Get Physical GPU from Logical GPU
    if(NvGetPhys && NvGetPhys(hdllGPU[i], (void *)hdlGPU, &nGPU) == NVAPI_OK){
      printf("Physical GPU Handle: %p, from Logical GPU Handle: %p (%d adapters)\n", hdlGPU[i], hdllGPU[i], nGPU);
    }
    else printf("NvAPI_GetPhysicalGPUsFromLogicalGPU not available!\n");
    // Get the bios version info.
    if(NvGetBiosName && NvGetBiosName(hdlGPU[i], biosname) == NVAPI_OK){
      printf("BIOS version: %s\n", biosname);
    }
    else printf("NvAPI_GPU_GetVbiosVersionString not available!\n");
    // Get BusID
    if(BusId && BusId(hdlGPU[i], &pBusId) == NVAPI_OK){
      printf("BusId: %ld\n", pBusId);
    }
    else printf("NvAPI_GPU_GetBusId not available!\n");
    // GPU Type.
    if(NvGPUType && NvGPUType(hdlGPU[i], &pGpuType) == NVAPI_OK){
      switch(pGpuType){
        case 1:      printf("GPU Type: CPU\n"); break;
        case 2:      printf("GPU Type: Discrete\n"); break;
        default:     printf("GPU Type: Unknown\n"); break;
      }
    }
    else printf("NvAPI_GPU_GetBusId not available!\n");
    // Onboard memory size for Graphics Adapter
    if(NvGetMemSize && NvGetMemSize(hdlGPU[i], &memsize) == NVAPI_OK){
      printf("VRAM: %dMB\n", memsize/1024);
    }
    else printf("NvAPI_GPU_GetPhysicalFrameBufferSize not available!\n");
    // Get device and vendor ID for adapter. Susbsystem ID and revision ID requires nvml usage
    // since this does not seem to be reported by vulkan.
    if(NvPCIID && NvPCIID(hdlGPU[i], &pDeviceId, &pSubSystemId, &pRevisionId, &pExtDeviceId) == NVAPI_OK){
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
    if(NvArch && NvArch(hdlGPU[i], &pGpuArchInfo) == NVAPI_OK){
      printf("Architecture: %ld\n", pGpuArchInfo.architecture);
      printf("Revision: %ld\n", pGpuArchInfo.revision);
    }
    else printf("NvAPI_GPU_GetArchInfo not available!\n");
    // Get memory type for onboard memory on adapter.
    // This needs some tuning to pick up all types of GDDR ram, so not very useful atm.
    // Currently not implemented in dxvk-nvapi
    if(NvGetMemType && NvGetMemType(hdlGPU[i], &memtype) == NVAPI_OK){
      printf("Onboard memory type: GDDR%d\n", memtype<=8?5:6);
    }
    else printf("NvAPI_GPU_GetRamType is not available!\n");
    // Get GPUuid from physical GPU
    if(NvGpuUid && NvGpuUid(hdlGPU[i], &retHandle) == NVAPI_OK){
      printf("GPU handle: %p, have GPU id: %ld\n", hdlGPU[i], retHandle);
    }
    else printf("NvAPI_GetGPUIDfromPhysicalGPU is not available!\n");
    // Get physical GPU from GPUuid
    if(NvGpuPhys && NvGpuPhys((void *)retHandle, &physHandle) == NVAPI_OK){
      printf("GPU id: %ld, have GPU handle: %p\n", retHandle, (void *)physHandle);
    }
    else printf("NvAPI_GetPhysicalGPUFromGPUID is not available!\n");
    // Get type of bus interface for adapter.
    if(NvBus && NvBus(hdlGPU[i], &pBusType) == NVAPI_OK){
      switch(pBusType){
        case 0: printf("Undefined GPU Bus\n"); break;
        case 1: printf("PCI Bus\n"); break;
        case 2: printf("AGP Bus\n"); break;
        case 3: printf("PCIexpress Bus\n"); break;
        default: printf("Unknown GPU Bus\n"); break;
      }
    }
    else printf("NvAPI_GPU_GetBusType is not available!\n");
    // Get performancestate of adapter
    if(NvPerfState && NvPerfState(hdlGPU[i], &pCurrentPstate) == NVAPI_OK){
      printf("GPU current performance state: %d\n", pCurrentPstate);
    }
    else printf("NvAPI_GPU_GetCurrentPstate not available!\n");
    // This gets GPU and memory controller utilization
    if(NvPstate && NvPstate(hdlGPU[i], &pDynamicPstatesInfoEx) == NVAPI_OK){
      printf("GPU utilization: %ld%%\n", pDynamicPstatesInfoEx.utilization[0].percentage);
      printf("Memory controller utilization: %ld%%\n", pDynamicPstatesInfoEx.utilization[1].percentage);
    }
    else printf("NvAPI_GPU_GetDynamicPstatesInfoEx not available!\n");
    // Get GPU and Memory controller utilization from GetPstates20
    if(NvPerf20 && NvPerf20(hdlGPU[i], &pPstatesInfo) == NVAPI_OK){
      printf("\n---------- Pstates info -----------\n");
      printf("  Number of pStates: %ld\n", pPstatesInfo.numPstates);
      int pstateIDarray[NVAPI_MAX_GPU_PERF_PSTATES];
      for(p = 0; p < pPstatesInfo.numPstates; p++){
        printf("  ID of Pstate %d: %d\n", p, pPstatesInfo.pstates[p].pstateId);
        pstateIDarray[p] = pPstatesInfo.pstates[p].pstateId;
        if(pstateIDarray[p] == pCurrentPstate) psID = p;
        else psID = 0;
      }
      printf("  Current running pstate is: %d\n", pCurrentPstate);
      printf("  GPU max frequency: %ldMHz\n", (pPstatesInfo.pstates[psID].clocks[0].data.range.maxFreq_kHz / 1000));
      printf("  GPU min frequency: %ldMHz\n", (pPstatesInfo.pstates[psID].clocks[0].data.range.minFreq_kHz / 1000));
      printf("  Memory clock frequency: %ldMHz\n", (pPstatesInfo.pstates[psID].clocks[1].data.single.freq_kHz / 1000));
      printf("  GPU default voltage: %ldmV\n", (pPstatesInfo.pstates[psID].baseVoltages[0].volt_uV / 1000));
      printf("  GPU current OC: %dMHz\n", (pPstatesInfo.pstates[psID].clocks[0].freqDelta_kHz.value / 1000));
      printf("  Mem current OC: %dMHz\n", (pPstatesInfo.pstates[psID].clocks[1].freqDelta_kHz.value / 1000));
      printf("--------- End Pstates info --------\n\n");
    }
    else printf("NvAPI_GPU_GetPstates20 not available!\n");
    // This gets GPU temperatures.
    if(NvThermals && NvThermals(hdlGPU[i], 0, &pThermalSettings) == NVAPI_OK){
      printf("GPU Current Temperature: %dC\n", pThermalSettings.sensor[0].currentTemp);
      printf("GPU Max Temperature: %dC\n", pThermalSettings.sensor[0].defaultMaxTemp);
    }
    else printf("NvAPI_GPU_GetThermalSettings not available!\n");
    // This gets cpu fan speed in RPM. This is not available in nvml.
    if(NvTach && NvTach(hdlGPU[i], &pValue) == NVAPI_OK){
      printf("GPU Fan speed: %ldRPM\n", pValue);
    }
    else printf("NvAPI_GPU_GetTachReading not available!\n");
    // Get cooler information from adapter. Fan speed in %.
    if(NvCooler && NvCooler(hdlGPU[i], NVAPI_COOLER_TARGET_ALL, &pCoolerInfo) == NVAPI_OK){
      printf("GPU cooler load: %ld%%\n", pCoolerInfo.cooler[NVAPI_COOLER_TARGET_NONE].currentLevel);
      // I dont really know the values for water/liquid or fan, so i assume fan if there is one
      if(pCoolerInfo.cooler[NVAPI_COOLER_TARGET_NONE].type != 0){
        printf("GPU cooler type: FAN\n");
      }
      else printf("GPU cooler type: NONE!\n");
    }
    else printf("NvAPI_GPU_GetCoolerSettings not available!\n");
    // Get all clockfrequencies from adapter - gpu, memory and video.
    for(int c = 0; c < 2; c++){
      switch(c){
        case 0: pClkFreqs.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;
          if(NvClkfq && NvClkfq(hdlGPU[i], &pClkFreqs) == NVAPI_OK){
          printf("GPU Current clock: %ldMHz\n", (pClkFreqs.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency / 1000));
          printf("Memory Current clock: %ldMHz\n", (pClkFreqs.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 1000));
          printf("Video Current clock: %ldMHz\n", (pClkFreqs.domain[NVAPI_GPU_PUBLIC_CLOCK_VIDEO].frequency / 1000)); break;
          }
          else{
            printf("NvAPI_GPU_GetAllClockFrequencies GPU CURRENT_FREQ not available!\n"); break;
          }
        case 1: pClkFreqs.ClockType = NV_GPU_CLOCK_FREQUENCIES_BOOST_CLOCK;
          if(NvClkfq && NvClkfq(hdlGPU[i], &pClkFreqs) == NVAPI_OK){
          printf("GPU boost clock: %ldMHz\n", (pClkFreqs.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency / 1000));
          printf("Memory boost Current clock: %ldMHz\n", (pClkFreqs.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 1000));
          printf("Video boost clock: %ldMHz\n", (pClkFreqs.domain[NVAPI_GPU_PUBLIC_CLOCK_VIDEO].frequency / 1000)); break;
          }
          else{
            printf("NvAPI_GPU_GetAllClockFrequencies GPU BOOST_CLOCK not available!\n");
            break;
          }
      }
    }
  }

  printf("-----------------------\nGeneral info for all adapters\n-----------------------\n");
  geninfo();

  if (nGPU > 0){
    for (i = 0; i < (nGPU); i++){
      printf("\n--------------\nAdapter %d info\n--------------\n", i);
      info();
      printf("--------------\nAdapter %d done\n--------------\n", i);
    }
  }

  NvUnload();

  printf("Press ENTER to Continue\n");
  getchar();
  return 0;
}
