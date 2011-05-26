############################################################
# concat.rb 
#  :concat *.k , *.c files into posix.c, posix.k !!!
#   for konoha.posix use only
#
#   shinpeinakata(c)2011
############################################################


############################################################
# compile bindings partially
############################################################

class Command
  def explanation
    return "no explanation"
  end
end

class PartialCompileCommand < Command
  def initialize
    @key = "p"
    @exp = "partial compile specify target files as arguments"
  end

  def explanation
    return @exp;
  end

  def bindKey()
    return @key;
  end
end

class Concat 
  def initialize(filename)
    @libname, @ext = filename.split(".")
    @outputfile = filename;
    @partialMode = false
  end

  def setPartialMode
    @partialMode = true
  end

  def searchFiles(ext, dir)
    mfiles = []
    if @partialMode == false
      # all mode
      Dir::foreach(dir) {|f|
        rext = ext + "$"
        reg = Regexp.new(rext)
        if reg.match(f) && f != @outputfile
          mfiles.push(f)
        end
      }
    else
      # mode is partial
      # TODO
    end
    return mfiles;
  end

  def generateFile(files)
    contents = ""
    files.each {|f|
      buf = open(f)
      contents = contents + buf.read
      buf.close
    }
    file = File.open(@outputfile, "w")
    file << contents
    file.close
  end

  # class method
  def Concat.explainCommands(commands)
    printf("usage: ruby concat.rb (command) [options]\n")
    printf("commands and their explanations\n")
    commands.each { |c|
      printf(" -%s\t: %s\n", c.bindKey, c.explanation)
    }
  end
end

cmds = [PartialCompileCommand.new()]

if __FILE__ == $0
  # arguments check
  if ARGV[0] != nil
    Concat.explainCommands(cmds)
  end
  # concat k files
  ["posix.k", "posix.c"].each {|f|
    con = Concat.new(f)
    a, ext = f.split(".")
    files = con.searchFiles("." + ext, ".");
    con.generateFile(files)
  }
  
end

