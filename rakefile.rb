ENV['gubg'] = Dir.pwd
require('./gubg.build/shared.rb')

task :clean do
    rm_rf('.cache')
    rm(FileList.new("*.exe"))
end

def each_submod(&block)
    submods = %w[build std].map{|n|"gubg.#{n}"}
    GUBG::each_submod(submods, &block)
end

task :declare do
    each_submod{sh 'rake declare'}
end
task :define => :declare do
    each_submod{sh 'rake define'}
end

rocket = nil
task :setup do
    require('gubg/build/Executable')
    rocket = Build::Executable.new('rocket')
    rocket.add_define('DEBUG')
    rocket.add_include_path('src')
    rocket.add_include_path(GUBG::shared_dir('include'))
    rocket.add_sources(FileList.new('src/**/*.cpp'))
    rocket.add_sources(FileList.new('src/**/*.hpp'))
    rocket.add_library_path(GUBG::shared_dir('lib'))
end

task :build => [:define, :setup] do
    rocket.build
end
task :run => :build do
    sh "./#{rocket.exe_filename}"
end
task :default => :run
