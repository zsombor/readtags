require 'rake/testtask'

begin
  require 'rake/extensiontask'
rescue LoadError
  abort "rake-compile is missing; Install it by running `gem i rake-compiler`"
end


Rake::ExtensionTask.new("tags") do |ext|
  ext.lib_dir = 'lib/tags'
end

require 'rubygems'
require 'fileutils'

task :default => :test

desc "Run tests"
task :test do
  sh "ruby -I test:lib test/tags_extn_test.rb"
end
