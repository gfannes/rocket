#We use a local gubg working dir
ENV['gubg'] = Dir.pwd

begin
    require('./gubg.build/shared.rb')
rescue LoadError
    #This is the initial clone: no gubg.build is present yet. Mediate and try to load again
    sh 'git submodule update --init --recursive'
    retry
end

task :clean do
    rm_rf('.cache')
    rm(FileList.new("*.exe"))
end

def each_submod(&block)
    submods = %w[build std].map{|n|"gubg.#{n}"}
    GUBG::each_submod(submods, &block)
end

#Installation of the gubg modules currently in use
task :declare do
    each_submod{sh 'rake declare'}
end
task :define => :declare do
    each_submod{sh 'rake define'}
end

rocket = nil
task :setup do
    require('gubg/build/Executable')

    #This is the rocket simulator
    rocket = Build::Executable.new('rocket')
    rocket.add_define('DEBUG')
    rocket.add_include_path('src')
    rocket.add_include_path(GUBG::shared_dir('include'))
    rocket.add_sources(FileList.new('src/**/*.cpp'))
    rocket.add_sources(FileList.new('src/**/*.hpp'))
    rocket.add_library_path(GUBG::shared_dir('lib'))
end

task :build => [:define, :setup] do
    #Build the rocket simulator
    rocket.build
end
task :run => :build do
    #Run the rocket simulator
    sh "./#{rocket.exe_filename}"
end
task :default => :run
