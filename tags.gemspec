# -*- Mode:Ruby; -*-

require File.expand_path('../lib/tags/version', __FILE__)

Gem::Specification.new do |s|
  s.name = %q{tags}
  s.summary = "A ruby extension for using TAGS files"
  s.version = Tags::VERSION
  s.authors = ["Dee Zsombor"]
  s.email = "zsombor@primalgrasp.com"
  s.date = Time.now.utc.strftime("%Y-%m-%d")
  s.extensions = ["ext/tags/extconf.rb"]
  s.files = `git ls-files`.split("\n")
  s.homepage = "http://primalgrasp.com"
  s.description = ""
end
