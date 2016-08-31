require(File.join(ENV['gubg'], 'shared'))
require('gubg/build/Executable')
include GUBG

rocket = Build::Executable.new('rocket')
rocket.add_define('DEBUG')
rocket.add_include_path('src')
rocket.add_include_path(shared_dir('include'))
rocket.add_sources(FileList.new('src/**/*.cpp'))
rocket.add_sources(FileList.new('src/**/*.hpp'))
rocket.add_library_path(shared_dir('lib'))
rocket.add_library('gubg.io')

task :build do
    rocket.build
end
task :run => :build do
    sh "./#{rocket.exe_filename}"
end
