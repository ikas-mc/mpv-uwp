# Compiling uwp

## Native Compilation with Clang (Windows SDK Build)
see compile-windows.md

setup
```
-Dffmpeg:gpl=enabled -Dffmpeg:tests=disabled -Dffmpeg:programs=disabled -Dffmpeg:sdl2=disabled -Dffmpeg:vulkan=disabled -Dffmpeg:libdav1d=disabled -Dffmpeg:libjxl=disabled -Dffmpeg:schannel=disabled -Dffmpeg:openssl=enabled -Dffmpeg:lcms2=disabled -Dlcms2:fastfloat=true -Dlcms2:jpeg=disabled -Dlcms2:tiff=disabled -Dlibplacebo:demos=false -Dlibplacebo:lcms=enabled -Dlibplacebo:shaderc=enabled -Dlibplacebo:vulkan=disabled -Dlibplacebo:d3d11=enabled -Dlibplacebo:uwp=enabled -Dlibplacebo:gl-proc-addr=disabled -Dlibplacebo:opengl=disabled -Dlibplacebo:dovi=disabled -Dlibplacebo:unwind=disabled -Dxxhash:inline-all=true -Dxxhash:cli=false -Dharfbuzz:icu=disabled -Dlibass:directwrite=enabled -Dlibunibreak:tests=disabled -Dfontconfig:tests=disabled -Dlibmpv=true -Dgpl=true -Duwp=enabled -Dd3d11=enabled -Dtests=false -Dvulkan=disabled -Djavascript=disabled -Dwin32-smtc=disabled -Ddrm=disabled -Dlibarchive=disabled -Drubberband=disabled -Dwayland=disabled -Dcplugins=disabled -Dlibbluray=disabled -Dzimg=disabled -Dgl=disabled -Dvaapi=disabled -Dlua=disabled -Dd3d9-hwaccel=disabled -Dcuda-hwaccel=disabled -Dx11=disabled -Djpeg=disabled -Dcplayer=false 


```
c_args
```
-D_WINRT_DLL
-DOPENSSL_NO_CAPIENG -DOPENSSL_SYS_WIN_CORE      
-DWINAPI_FAMILY=WINAPI_FAMILY_APP
-DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00
```

link_args
```
-lksuser
```
