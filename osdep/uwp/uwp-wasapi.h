#pragma once
#include <unknwn.h>

#ifdef __cplusplus
extern "C" {
#endif

HRESULT wuCreateDefaultAudioRenderer(IUnknown** res);

#ifdef __cplusplus
}
#endif
