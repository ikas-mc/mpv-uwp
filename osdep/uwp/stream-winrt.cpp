#include <unknwn.h> 
#include <winrt/windows.foundation.h>
#include <winrt/windows.storage.h>
#include <winrt/windows.storage.streams.h>
#include <winrt/windows.storage.accesscache.h>
#include <shcore.h>
#include <string>
#include <memory>
#include <algorithm>

extern "C" {
#include "stream-winrt.h"
}

using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;

struct winrt_stream_priv {
    IRandomAccessStream winrt_stream{ nullptr };
    winrt::com_ptr<IStream> com_stream{ nullptr };
    uint64_t total_size = 0;
};

static int fill_buffer_winrt(stream_t* s, void* buffer, int max_len) {
    auto p = static_cast<winrt_stream_priv*>(s->priv);
    if (!p || !p->com_stream || max_len <= 0) return -1;

    ULONG bytes_read = 0;
    HRESULT hr = p->com_stream->Read(buffer, static_cast<ULONG>(max_len), &bytes_read);
    if (FAILED(hr)) return -1;

    return static_cast<int>(bytes_read);
}

static int seek_winrt(stream_t* s, int64_t pos) {
    auto p = static_cast<winrt_stream_priv*>(s->priv);
    if (!p || !p->com_stream) return 0;

    LARGE_INTEGER move_pos{};
    move_pos.QuadPart = pos;
    ULARGE_INTEGER actual_pos;

    HRESULT hr = p->com_stream->Seek(move_pos, STREAM_SEEK_SET, &actual_pos);
    if (SUCCEEDED(hr)) {
        s->pos = actual_pos.QuadPart;
        s->eof = 0;
        return 1;
    }
    return 0;
}

static int64_t get_size_winrt(stream_t* s) {
    auto p = static_cast<winrt_stream_priv*>(s->priv);
    return p ? p->total_size : -1;
}

static void close_winrt(stream_t* s) {
    auto p = static_cast<winrt_stream_priv*>(s->priv);
    if (p) {
        p->com_stream = nullptr;
        p->winrt_stream = nullptr;
        delete p;
        s->priv = nullptr;
    }
}

static int open_winrt(stream_t* s) {
    auto p = std::make_unique<winrt_stream_priv>();
    StorageFile file{ nullptr };

    try {
        std::string url_str(s->url);

        if (url_str.rfind("winrtfile://", 0) == 0) {
            file = StorageFile::GetFileFromPathAsync(winrt::to_hstring(s->path)).get();
        }
        else if (url_str.rfind("winrtrecent://", 0) == 0) {
            file = AccessCache::StorageApplicationPermissions::MostRecentlyUsedList().GetFileAsync(winrt::to_hstring(s->path)).get();
        }
        else if (url_str.rfind("winrtaccess://", 0) == 0) {
            file = AccessCache::StorageApplicationPermissions::FutureAccessList().GetFileAsync(winrt::to_hstring(s->path)).get();
        }
        else {
            return STREAM_UNSUPPORTED;
        }

        if (!file) return STREAM_ERROR;

        p->winrt_stream = file.OpenAsync(FileAccessMode::Read).get();
        p->total_size = p->winrt_stream.Size();

        HRESULT hr = CreateStreamOverRandomAccessStream(
            reinterpret_cast<::IUnknown*>(winrt::get_abi(p->winrt_stream)),
            IID_PPV_ARGS(p->com_stream.put())
        );
        if (FAILED(hr)) {
            return STREAM_ERROR;
        }

        s->priv = p.release();

        s->fill_buffer = fill_buffer_winrt;
        s->seek = seek_winrt;
        s->get_size = get_size_winrt;
        s->close = close_winrt;

        s->seekable = true;
        s->is_local_fs = true;
        s->is_regular = true;

        return STREAM_OK;
    }
    catch (...) {
        return STREAM_ERROR;
    }
}

extern "C" {
    const stream_info_t stream_info_winrt = {
        .name = "winrt",
        .open = open_winrt,
        .protocols = (const char* const []){"winrtaccess", "winrtfile", "winrtrecent", NULL},
        .can_write = false,
    };
}
