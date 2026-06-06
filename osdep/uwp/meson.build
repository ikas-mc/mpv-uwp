add_languages('cpp', native: false)
cpp = meson.get_compiler('cpp')

features += {'uwp-wasapi': cpp.has_header('winrt/base.h', required: get_option('uwp-wasapi'))}
if features['uwp-wasapi']
    dependencies += cpp.find_library('runtimeobject')
    sources += files('uwp-wasapi.cpp')
endif
