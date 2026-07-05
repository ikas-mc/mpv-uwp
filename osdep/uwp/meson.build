add_languages('cpp', native: false)
cpp = meson.get_compiler('cpp')

features += {'uwp-wasapi': cpp.has_header('winrt/base.h', required: get_option('uwp-wasapi'))}
if features['uwp-wasapi']
    dependencies += cpp.find_library('runtimeobject')
    sources += files('uwp-wasapi.cpp')
endif

features += {'stream-winrt': cpp.has_header('winrt/base.h', required: get_option('stream-winrt'))}
if features['stream-winrt']
    dependencies += cpp.find_library('runtimeobject')
    sources += files('stream-winrt.cpp')
endif
