project('RT', 'cpp',
        version: '0.1',
        default_options : [
            'c_std=c11',
            'cpp_std=c++11'
        ],
        meson_version: '>= 0.46.0')

add_project_arguments(['-Wpedantic', '-fopenmp'], language: 'cpp')
add_project_link_arguments(['-fopenmp'], language: 'cpp')

if (get_option('buildtype') == 'release')
    add_project_arguments(['-O3'], language: 'cpp')
endif

sources = files([
    'aabb.cpp',
    'camera.cpp',
    'main.cpp',
#    'utils.cpp'
])

executable('main', sources)
