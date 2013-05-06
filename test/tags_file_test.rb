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
                   :kind=>"m",
                   :file_scope=>true,
                   :ext=>{"struct"=>"sTagFile::__anon3"}},
                 results[0])
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.c",
                   :line_pattern=>"/^\t\tchar *name;$/",
                   :kind=>"m",
                   :file_scope=>true,
                   :ext=>{"struct"=>"sTagFile::__anon5"}},
                 results[1])
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.c",
                   :line_pattern=>"/^\tvstring name;$/",
                   :kind=>"m",
                   :file_scope=>true,
                   :ext=>{"struct"=>"sTagFile"}},
                 results[2])
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.h",
                   :line_pattern=>"/^\t\tconst char *name;$/",
                   :kind=>"m",
                   :ext=>{"struct"=>"__anon8::__anon11"}},
                 results[3])
    assert_equal({ :name=>"name",
                   :file=>"ext/tags/vendor/readtags.h",
                   :line_pattern=>"/^\tconst char *name;$/",
                   :kind=>"m",
                   :ext=>{"struct"=>"__anon13"}},
                 results[4])
    assert_equal 5, results.size
  end


  def test_find_partial_match
    results = []
    Tags::File.open(File.dirname(__FILE__) + '/tags.sample') do |f|
      f.each('na', partial_match: true) do |res|
        results << res
      end
    end
    assert results.size >= 5
  end


  def test_entry_with_line_number
    results = []
    Tags::File.open(File.dirname(__FILE__) + '/tags.sample') do |f|
      f.each('DB8500_PRCM_LINE_VALUE') do |res|
        results << res
      end
    end
    assert_equal 1, results.size
    assert_equal({ name: "DB8500_PRCM_LINE_VALUE",
                   file: "include/linux/mfd/db8500-prcmu.h",
                   line_number: 19,
                   kind: "d",
                   ext:{}}, results[0])
  end



end
