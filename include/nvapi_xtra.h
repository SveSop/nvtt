/*
 * Process Hacker Extra Plugins -
 *   Nvidia GPU Plugin
 *
 * Copyright (C) 2015 dmex
 *
 * This file is part of Process Hacker.
 *
 * Process Hacker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Process Hacker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Process Hacker.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _NVAPI_XTRA_H
#define _NVAPI_XTRA_H

#define NVAPI_MAX_COOLERS_PER_GPU   3
#define NVAPI_MIN_COOLER_LEVEL      0
#define NVAPI_MAX_COOLER_LEVEL      100
#define NVAPI_MAX_COOLER_LEVELS     24

typedef enum _NV_COOLER_TYPE
{
    NVAPI_COOLER_TYPE_NONE = 0,
    NVAPI_COOLER_TYPE_FAN,
    NVAPI_COOLER_TYPE_WATER,
    NVAPI_COOLER_TYPE_LIQUID_NO2,
} NV_COOLER_TYPE;

typedef enum _NV_COOLER_CONTROLLER
{
    NVAPI_COOLER_CONTROLLER_NONE = 0,
    NVAPI_COOLER_CONTROLLER_ADI,
    NVAPI_COOLER_CONTROLLER_INTERNAL,
} NV_COOLER_CONTROLLER;

typedef enum _NV_COOLER_POLICY
{
    NVAPI_COOLER_POLICY_NONE = 0,
    NVAPI_COOLER_POLICY_MANUAL,                     // Manual adjustment of cooler level. Gets applied right away independent of temperature or performance level.
    NVAPI_COOLER_POLICY_PERF,                       // GPU performance controls the cooler level.
    NVAPI_COOLER_POLICY_TEMPERATURE_DISCRETE = 4,   // Discrete thermal levels control the cooler level.
    NVAPI_COOLER_POLICY_TEMPERATURE_CONTINUOUS = 8, // Cooler level adjusted at continuous thermal levels.
    NVAPI_COOLER_POLICY_HYBRID,                     // Hybrid of performance and temperature levels.
} NV_COOLER_POLICY;

typedef enum _NV_COOLER_TARGET
{
    NVAPI_COOLER_TARGET_NONE = 0,
    NVAPI_COOLER_TARGET_GPU,
    NVAPI_COOLER_TARGET_MEMORY,
    NVAPI_COOLER_TARGET_POWER_SUPPLY = 4,
    NVAPI_COOLER_TARGET_ALL = 7                    // This cooler cools all of the components related to its target gpu.
} NV_COOLER_TARGET;

typedef enum _NV_COOLER_CONTROL
{
    NVAPI_COOLER_CONTROL_NONE = 0,
    NVAPI_COOLER_CONTROL_TOGGLE,                   // ON/OFF
    NVAPI_COOLER_CONTROL_VARIABLE,                 // Suppports variable control.
} NV_COOLER_CONTROL;

typedef enum _NV_COOLER_ACTIVITY_LEVEL
{
    NVAPI_INACTIVE = 0,                             // inactive or unsupported
    NVAPI_ACTIVE = 1,                               // active and spinning in case of fan
} NV_COOLER_ACTIVITY_LEVEL;

typedef struct _NV_GPU_COOLER_SETTINGS
{
    NvU32 version;                           // structure version
    NvU32 count;                             // number of associated coolers with the selected GPU
    struct
    {
        NV_COOLER_TYPE type;                 // type of cooler - FAN, WATER, LIQUID_NO2...
        NV_COOLER_CONTROLLER controller;     // internal, ADI...
        NvU32 defaultMinLevel;               // the min default value % of the cooler
        NvU32 defaultMaxLevel;               // the max default value % of the cooler
        NvU32 currentMinLevel;               // the current allowed min value % of the cooler
        NvU32 currentMaxLevel;               // the current allowed max value % of the cooler
        NvU32 currentLevel;                  // the current value % of the cooler
        NV_COOLER_POLICY defaultPolicy;      // cooler control policy - auto-perf, auto-thermal, manual, hybrid...
        NV_COOLER_POLICY currentPolicy;      // cooler control policy - auto-perf, auto-thermal, manual, hybrid...
        NV_COOLER_TARGET target;             // cooling target - GPU, memory, chipset, powersupply, canoas...
        NV_COOLER_CONTROL controlType;       // toggle or variable
        NV_COOLER_ACTIVITY_LEVEL active;     // is the cooler active - fan spinning...
    } cooler[NVAPI_MAX_COOLERS_PER_GPU];
} NV_GPU_COOLER_SETTINGS, *PNV_GPU_COOLER_SETTINGS;

#define NV_GPU_COOLER_SETTINGS_VER  MAKE_NVAPI_VERSION(NV_GPU_COOLER_SETTINGS, 1)

///////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME:   NvAPI_GPU_GetCoolerSettings
//
// DESCRIPTION:     Retrieves the cooler information of all coolers or a specific cooler associated with the selected GPU.
//                  Coolers are indexed 0 to NVAPI_MAX_COOLERS_PER_GPU-1.
//                  To retrieve specific cooler info set the coolerIndex to the appropriate cooler index.
//                  To retrieve info for all cooler set coolerIndex to NVAPI_COOLER_TARGET_ALL.
//
// PARAMETERS :     hPhysicalGPU(IN) - GPU selection.
//                  coolerIndex(IN)  - Explict cooler index selection.
//                  pCoolerInfo(OUT) - Array of cooler settings.
//
///////////////////////////////////////////////////////////////////////////////
typedef NvAPI_Status (WINAPIV *_NvAPI_GPU_GetCoolerSettings)(_In_ NvPhysicalGpuHandle hPhysicalGpu, NvU32 coolerIndex, NV_GPU_COOLER_SETTINGS* pCoolerInfo);
_NvAPI_GPU_GetCoolerSettings NvAPI_GPU_GetCoolerSettings;

#endif /* __NVAPI_XTRA_H */
