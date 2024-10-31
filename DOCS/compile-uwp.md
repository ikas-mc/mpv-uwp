# Compiling uwp


## toolchain

https://github.com/mstorsjo/llvm-mingw
https://github.com/videolan/vlc/blob/master/doc/BUILD-win32.md

x86_64-w64-mingw32uwp.txt

```
[binaries]
c = 'x86_64-w64-mingw32uwp-clang'
cpp = 'x86_64-w64-mingw32uwp-clang++'
ar = 'x86_64-w64-mingw32uwp-ar.exe'
strip = 'x86_64-w64-mingw32uwp-strip.exe'
ld = 'x86_64-w64-mingw32uwp-ld'
pkg-config = 'pkg-config'
cmake = 'cmake'

[built-in options]
c_args = ['-DWINAPI_FAMILY=WINAPI_FAMILY_APP']
#c_link_args = ['-APPCONTAINER']
cpp_args = ['-DWINAPI_FAMILY=WINAPI_FAMILY_APP']
#cpp_link_args = ['-APPCONTAINER']
c_winlibs =['-lwindowsappcompat']
cpp_winlibs =['-lwindowsappcompat']

[host_machine]
system = 'windows'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'

```

```

export PATH=/llvm-mingw-ucrt-x86_64/bin:$PATH
export C_INCLUDE_PATH=sdk/x86_64-w64-mingw32uwp/include
export CPLUS_INCLUDE_PATH=sdk/x86_64-w64-mingw32uwp/include
export LIBRARY_PATH=sdk/x86_64-w64-mingw32uwp/lib
export PKG_CONFIG_PATH=sdk/x86_64-w64-mingw32uwp/lib/pkgconfig:$PKG_CONFIG_PATH

```

## mpv
```
# libplacebo, sample
meson setup -Dvulkan=disabled -Dgl-proc-addr=disabled -Dopengl=disabled -Dshaderc=disabled -Dlcms=disabled -Ddovi=disabled -Ddemos=false -Dxxhash=disabled -Dunwind=disabled -Dd3d11=enabled -Dglslang=enabled -Ddefault_library=static --cross-file ../x86_64-w64-mingw32uwp.txt --reconfigure build

# mpv
meson setup build2  -Diconv=disabled -Dcaca=disabled -Dcplugins=disabled -Dlibbluray=disabled -Dlcms2=disabled -Dzimg=disabled  -Dlibarchive=disabled   -Dlibmpv=true -Duwp=enabled -Dcplayer=false -Dgl=disabled -Dvaapi=disabled -Dlua=disabled -Dvulkan=disabled -Dd3d-hwaccel=enabled -Djpeg=disabled -Dd3d9-hwaccel=disabled -Dcuda-hwaccel=disabled -Dspirv-cross=enabled -Duwp=enabled -Dtests=false -Dd3d11=enabled --cross-file  ../x86_64-w64-mingw32uwp.txt --reconfigure

```


## deps

```
ffmpeg
./configure \
    --pkg-config=pkg-config \
    --cross-prefix=x86_64-w64-mingw32uwp- \
    --enable-gpl \
    --arch=x86_64 \
	--disable-doc \
	--disable-decoder=opus \
	--enable-libgsm \
	--enable-libopenjpeg \
	--disable-debug \
	--disable-protocol=concat \
	--disable-bsfs \
	--disable-libvpx \
	--enable-bsf=vp9_superframe \
	--disable-iconv \
	--disable-avisynth \
	--disable-nvenc \
	--disable-linux-perf \
    --target-os=mingw32uwp \
    --enable-w32threads \
    --disable-dxva2 \
    --disable-programs \
    --disable-muxers \
    --disable-encoders \
    --enable-cross-compile \
    --enable-static \
    --disable-shared \
    --enable-openssl \
    --enable-protocol=https \
    --enable-hwaccels \
    --enable-d3d11va \
    --disable-dxva2 \
    --enable-nonfree \
    --extra-cflags="-Wno-error=incompatible-function-pointer-types" 

```

```

Name: spirv-cross-c
Libs: -L${libdir} -lspirv-cross-c  -lSPVRemapper  -lspirv-cross-core  -lspirv-cross-cpp  -lspirv-cross-glsl    -lspirv-cross-util    -lspirv-cross-reflect    -lspirv-cross-hlsl    -lspirv-cross-msl    -lglslang    -lglslang-default-resource-limits 

# shaderc shared , libshaderc_shared.dll,  https://github.com/microsoft/vcpkg/tree/master/ports/shaderc
Name: shaderc
Libs: -L${libdir} -lshaderc_shared  

```