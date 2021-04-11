project('project2', 'cpp',
        license : ['Boost'],
        version : '0.1',
        default_options : ['cpp_std=c++2a', 'warning_level=3', 'werror=true'])

thread_dep = dependency('threads')

inc_dir = include_directories(['include',
                                get_option('cli11_include_dir'),
                                get_option('spdlog_include_dir'),
                                get_option('toml_include_dir'),
#                               get_option('json_include_dir'),
                                get_option('asio_include_dir'),
                                get_option('tabulate_include_dir'),
                                                
                              ])


# begin protobuf: https://developers.google.com/protocol-buffers/
protoc = find_program('protoc', required : true)
protobuf_dep = dependency('protobuf', required : true)

gen = generator(protoc, \
  output    : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
  arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/messages/', '--cpp_out=@BUILD_DIR@', '@PLAINNAME@'])

generated = gen.process('messages/player_message.proto')
# end protobuf

src_server = ['src_server/main_server.cpp', 'src_server/game_master.cpp', 'src_server/player.cpp', generated]

src_client = ['src_client/main_client.cpp', 'src_server/game_master.cpp', 'src_server/player.cpp', generated]

executable('start_server',
            sources : src_server,
            include_directories : inc_dir,
            dependencies : [thread_dep, protobuf_dep]
           )

executable('start_client',
            sources : src_client,
            include_directories : inc_dir,
            dependencies : [thread_dep, protobuf_dep]
           )
