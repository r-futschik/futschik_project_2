project('project2', 'cpp',
        license : ['Boost'],
        version : '0.1',
        default_options : ['cpp_std=c++2a', 'warning_level=3', 'werror=true'])

thread_dep = dependency('threads')

inc_dir = include_directories(['include',
                                get_option('cli11_include_dir'),
                                get_option('spdlog_include_dir'),
#                               get_option('json_include_dir'),
                                get_option('asio_include_dir')                               
                              ])



src_server = ['src_server/main_server.cpp', 'src_server/ship.cpp']

src_client = ['src_client/main_client.cpp']

executable('start_server',
            sources : src_server,
            include_directories : inc_dir,
            dependencies : thread_dep
           )

executable('start_client',
            sources : src_client,
            include_directories : inc_dir,
            dependencies : thread_dep
           )
