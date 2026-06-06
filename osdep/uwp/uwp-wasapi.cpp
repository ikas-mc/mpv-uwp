#include "uwp-wasapi.h"

#include <windows.h>
#include <AudioClient.h>
#include <mmdeviceapi.h>
#include <winrt/windows.media.devices.h>

using namespace winrt::Windows::Media::Devices;

struct WinrtAudioDeviceCreator : winrt::implements<WinrtAudioDeviceCreator, IActivateAudioInterfaceCompletionHandler> {
    HANDLE completionEventHandle;
    winrt::com_ptr<IUnknown> createdInterface{nullptr};

    WinrtAudioDeviceCreator() { 
        completionEventHandle = CreateEvent(nullptr, TRUE, FALSE, nullptr); 
    }

    ~WinrtAudioDeviceCreator() { 
        CloseHandle(completionEventHandle); 
    }

    void beginCreate(const std::wstring_view &deviceId) {
        winrt::com_ptr<IActivateAudioInterfaceAsyncOperation> asyncOperation{nullptr};
        auto deviceIdData = deviceId.empty() ? MediaDevice::GetDefaultAudioRenderId(AudioDeviceRole::Default).data() : deviceId.data();
        auto result = ActivateAudioInterfaceAsync(deviceIdData, __uuidof(IAudioClient3), nullptr, this, asyncOperation.put());
        if (FAILED(result)) {
            setResult(nullptr);
        }
    }

    HRESULT STDMETHODCALLTYPE ActivateCompleted(IActivateAudioInterfaceAsyncOperation *activateOperation) override {
        HRESULT activationResult = S_FALSE;
        winrt::com_ptr<IUnknown> audioInterface{nullptr};
        activateOperation->GetActivateResult(&activationResult, audioInterface.put());
        if (SUCCEEDED(activationResult)) {
            setResult(audioInterface.get());
        } else {
            setResult(nullptr);
        }
        return S_OK;
    }

    void setResult(IUnknown *interfacePointer) {
        this->createdInterface.copy_from(interfacePointer);
        SetEvent(completionEventHandle);
    }

    void waitForCompletion() { WaitForSingleObject(completionEventHandle, 30000); }
};

extern "C" HRESULT wuCreateAudioRenderer(IUnknown **audioRenderer, const char *deviceId) {
    auto deviceCreator = winrt::make_self<WinrtAudioDeviceCreator>();
    auto deviceIdHString = winrt::to_hstring(deviceId);
    deviceCreator->beginCreate(deviceIdHString);
    deviceCreator->waitForCompletion();
    if (deviceCreator->createdInterface) {
        deviceCreator->createdInterface.copy_to(audioRenderer);
        return S_OK;
    } else {
        return S_FALSE;
    }
}

extern "C" HRESULT wuCreateDefaultAudioRenderer(IUnknown **audioRenderer) { 
    return wuCreateAudioRenderer(audioRenderer, ""); 
}
