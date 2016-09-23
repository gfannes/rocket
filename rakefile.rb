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
    submods = %w[build std io].map{|n|"gubg.#{n}"}
    GUBG::each_submod(submods, &block)
end

#Installation of the gubg modules currently in use
task :declare do
    each_submod{sh 'rake declare'}
end
task :define => :declare do
    each_submod{sh 'rake define'}
end

#Update each submodule to the head
task :uth do
    each_submod{sh 'git checkout master'; sh 'git pull --rebase'}
end

rocket = nil
task :setup do
    require('gubg/build/Executable')

    #This is the rocket simulator
    rocket = Build::Executable.new('rocket')
    # rocket.add_define('DEBUG')
    rocket.add_include_path('src')
    rocket.add_include_path(GUBG::shared_dir('include'))
    rocket.add_sources(FileList.new('src/**/*.cpp').exclude('src/test/**/*.cpp'))
    rocket.add_sources(FileList.new('src/**/*.hpp'))
    rocket.add_sources(FileList.new(GUBG::shared('include/**/*.hpp')))
    rocket.add_library_path(GUBG::shared_dir('lib'))
end

task :build => [:define, :setup] do
    #Build the rocket simulator
    rocket.build
end
task :run => :build do
    #Run the rocket simulator
    sh "./#{rocket.exe_filename}"
    sh "gnuplot output.gnuplot"
end
task :default => :run

task :test do
    require('gubg/build/Executable')

    ut = Build::Executable.new('unit_tests')
    ut.add_define('DEBUG')
    ut.add_include_path('src')
    ut.add_include_path(GUBG::shared_dir('include'))
    ut.add_sources(FileList.new('src/test/*.cpp'))
    ut.add_sources(FileList.new('src/**/*.hpp'))
    ut.add_sources(GUBG::shared_file('source', 'catch_runner.cpp'))
    ut.add_sources(FileList.new(GUBG::shared('include/**/*.hpp')))
    ut.add_library_path(GUBG::shared_dir('lib'))
    ut.build
    sh "./#{ut.exe_filename} -a -d yes"
end
