module Tags
  class File
    include Enumerable

    def each(name, options = { }, &block)
      res = find_first(name, options)
      while res && !res.empty?
        yield res
        res = find_next()
      end
    end

    def self.open(file)
      f = File.new(file)
      yield f
    ensure
      f.close!
    end
  end

end
