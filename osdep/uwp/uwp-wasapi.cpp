#include "uwp-wasapi.h"

#include <windows.h>
#include <ppltasks.h>
#include <AudioClient.h>
#include <mmdeviceapi.h>
#include <winrt/windows.media.devices.h>

using namespace winrt::Windows::Media::Devices;

struct WinrtAudioDeviceCreator : winrt::implements<WinrtAudioDeviceCreator, IActivateAudioInterfaceCompletionHandler> {
    concurrency::task_completion_event<winrt::com_ptr<::IUnknown>> activationCompleted;

    WinrtAudioDeviceCreator() {
    }

    ~WinrtAudioDeviceCreator() {
    }

    concurrency::task<winrt::com_ptr<::IUnknown>> createAsync(winrt::hstring const& deviceId) {
        winrt::com_ptr<IActivateAudioInterfaceAsyncOperation> asyncOperation{ nullptr };
        if (deviceId.empty()) {
            auto defaultId = MediaDevice::GetDefaultAudioRenderId(AudioDeviceRole::Default);
            winrt::check_hresult(ActivateAudioInterfaceAsync(defaultId.c_str(), __uuidof(IAudioClient3), nullptr, this, asyncOperation.put()));
        }
        else {
            winrt::check_hresult(ActivateAudioInterfaceAsync(deviceId.c_str(), __uuidof(IAudioClient3), nullptr, this, asyncOperation.put()));
        }

        return concurrency::task<winrt::com_ptr<::IUnknown>>{ activationCompleted };
    }

    HRESULT STDMETHODCALLTYPE ActivateCompleted(IActivateAudioInterfaceAsyncOperation* activateOperation) override {
        HRESULT activateResult = S_FALSE;
        winrt::com_ptr<IUnknown> audioInterface;
        try
        {
            HRESULT hr = activateOperation->GetActivateResult(&activateResult, audioInterface.put());
            if (FAILED(hr)) {
                activationCompleted.set_exception(winrt::hresult_error(hr));
            }
            else {
                if (FAILED(activateResult)) {
                    activationCompleted.set_exception(winrt::hresult_error(activateResult));
                }
                else {
                    activationCompleted.set(audioInterface);
                }
            }
        }
        catch (const std::exception& e)
        {
            activationCompleted.set_exception(e);
        }

        return S_OK;
    }
};

extern "C" HRESULT wuCreateAudioRenderer(IUnknown** audioRenderer, const char* deviceId) {
    if (!audioRenderer) {
        return E_POINTER;
    }
    *audioRenderer = nullptr;

    try
    {
        winrt::hstring deviceIdString;
        if (deviceId) {
            deviceIdString = winrt::to_hstring(deviceId);
        }

        auto deviceCreator = winrt::make_self<WinrtAudioDeviceCreator>();
        winrt::com_ptr<::IUnknown> audioInterface = deviceCreator->createAsync(deviceIdString).get();
        audioInterface.copy_to(audioRenderer);

        return S_OK;
    }
    catch (winrt::hresult_error const& e)
    {
        return e.code();
    }
    catch (std::exception const&)
    {
        return winrt::to_hresult();
    }
    catch (...)
    {
        return E_FAIL;
    }
}

extern "C" HRESULT wuCreateDefaultAudioRenderer(IUnknown** audioRenderer) {
    return wuCreateAudioRenderer(audioRenderer, nullptr);
}
