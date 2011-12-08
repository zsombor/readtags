require 'test_helper'

class TagsFileTest < Test::Unit::TestCase

  def test_find_multiple
    Tags::File.open(File.dirname(__FILE__) + '/tags.sample') do |f|
      f.each('name') do |result|
        puts result
      end
    end
  end

end
