#pragma once
#include <unknwn.h>

#ifdef __cplusplus
extern "C" {
#endif

HRESULT wuCreateAudioRenderer(IUnknown** audioRenderer, const char* deviceId);
HRESULT wuCreateDefaultAudioRenderer(IUnknown** res);

#ifdef __cplusplus
}
#endif
