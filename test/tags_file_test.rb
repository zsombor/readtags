require 'test_helper'

class TagsFileTest < Test::Unit::TestCase

  def test_find_multiple
    results = []
    Tags::File.open(File.dirname(__FILE__) + '/tags.sample') do |f|
      f.each('name') do |res|
        results << res
      end
    end
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.c",
                   :line_pattern=>"/^\t\t\tchar *name;$/",
                   :line_number=>0,
                   :kind=>"m",
                   :file_scope=>true,
                   :ext=>{"struct"=>"sTagFile::__anon3"}},
                 results[0])
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.c",
                   :line_pattern=>"/^\t\tchar *name;$/",
                   :line_number=>0,
                   :kind=>"m",
                   :file_scope=>true,
                   :ext=>{"struct"=>"sTagFile::__anon5"}},
                 results[1])
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.c",
                   :line_pattern=>"/^\tvstring name;$/",
                   :line_number=>0,
                   :kind=>"m",
                   :file_scope=>true,
                   :ext=>{"struct"=>"sTagFile"}},
                 results[2])
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.h",
                   :line_pattern=>"/^\t\tconst char *name;$/",
                   :line_number=>0,
                   :kind=>"m",
                   :ext=>{"struct"=>"__anon8::__anon11"}},
                 results[3])
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.h",
                   :line_pattern=>"/^\tconst char *name;$/",
                   :line_number=>0,
                   :kind=>"m",
                   :ext=>{"struct"=>"__anon13"}},
                 results[4])
    assert_equal 5, results.size
  end

end
