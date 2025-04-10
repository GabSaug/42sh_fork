#! /usr/bin/env ruby

# Test Suite in Ruby for 42sh project
# Configuration variables :

$Bin_default_name = "sh"
$Timeout_default = 1
$Err_eq_def = true
$Sanity_bin = "valgrind --track-fds=yes --xml=yes --xml-file=outxml --log-file=outend --child-silent-after-fork=yes"

require 'open3'
require 'timeout'

class Test
  def initialize(path)
    file_content = File.read(path)
    if (file_content =~ /<test>(.*)<\/test>/m) != nil then
      test_content = $1
    else
      raise "No <test> balise in " + path + " test file (mandatory balise)."
    end
    if (test_content =~ /<name>(.*)<\/name>/) != nil then
      @name = $1
    else
      raise "No <name> balise in " + path + " test file (mandatory balise)."
    end
    if (test_content =~ /<input_type>(.*)<\/input_type>/) != nil then
      @type = $1
      if  @type == "file" || @type == "tty" then
        if (test_content =~ /<file>(.*)<\/file>/) != nil then
          @file = path.lines("/")[0] + $1
        else
          raise "No <file> balise in " + path +
          " test file (mandatory balise when input_type is file or tty)"
        end
      elsif @type != "no input" then
        raise "input_type error in " + path + " test file: " + @type +
        " (must be \"file\", \"tty\" or \"no input\")."
      end
    else
      raise "No <input_type> balise in " + path +
      " test file (mandatory balise)."
    end
    if (test_content =~ /<args>(.*)<\/args>/) != nil then
      @additional_args = $1
    else
      @additional_args = ""
    end
    if (test_content =~ /<err_eq>(.*)<\/err_eq>/) != nil then
      bool = $1
      if bool == "true" then
        @err_eq = true
      elsif bool == "false" then
        @err_eq = false
      else
        raise "Expected true or false in <err_eq> balise"
      end
    else
      @err_eq = $Err_eq_def
    end
    if (test_content =~ /<validation>(.*)<\/validation>/) != nil then
      @validation = $1
      if @validation == "manual" then
        test_content =~ /<stdout>(.*)<\/stdout>/m
        @ref_stdout = File.read(path.lines("/")[0] + $1)
        test_content =~ /<stderr>(.*)<\/stderr>/m
        @ref_stderr = File.read(path.lines("/")[0] + $1)
        test_content =~ /<exit_code>(.*)<\/exit_code>/
        @ref_exit_code = $1.to_i
      end
    else
      raise "No <validation> balise in " + path +
      " test file (mandatory balise)."
    end
  end
  def gen_command(test)
    command = ""
    if $sanity then
      command += $Sanity_bin + " "
    end
    if test then
      command += $bin_name + " "
    else
      command += @validation + " "
    end
    command += @additional_args
    if @type == "file" then
      command += " " + @file
    end
    return command
  end
  def run_ref
    command = self.gen_command(false)
    @ref_stdout = ""
    @ref_timeout = false
    Open3.popen3(command) do |stdin, stdout, stderr, t|
      if @type == "tty" then
        input_content = File.read(@file)
        stdin.puts(input_content)
      end
      stdin.close
      begin
        Timeout.timeout($timeout) do
          while (line = stdout.gets) do
            @ref_stdout += line
          end
        end
      rescue Timeout::Error => ex
        Process.kill(9, t.pid)
        @ref_timeout = true
      end
      if !@ref_timeout then
        @ref_stderr = stderr.read.gsub(@validation + ":", $bin_name.lines("/")[-1] + ":")
        @ref_exit_code = t.value.exitstatus
      end
    end
  end
  def run_test(out)
    if @validation != "manual" then
      self.run_ref
    end
    command = self.gen_command(true)
    @stdout = ""
    @timeout = false
    Open3.popen3(command) do |stdin, stdout, stderr, t|
      if @type == "tty" then
        input_content = File.read(@file)
        stdin.puts(input_content)
      end
      stdin.close
      begin
        Timeout.timeout($timeout) do
          while (line = stdout.gets) do
            @stdout += line
          end
        end
      rescue Timeout::Error => ex
        Process.kill(9, t.pid)
        @timeout = true
      end
      if !@timeout then
        @stderr = stderr.read
        @exit_code = t.value.exitstatus
      end
    end
    return self.print_result(out) == 0
  end
  def print_result(extended_output)
    errors = 0
    if extended_output then
      puts("    [\e[93mINFO\e[0m] " + @name + " started") 
      puts("    [\e[93mINFO\e[0m] Command : " + self.gen_command($bin_name)) 
      if @ref_timeout then
        puts("    [\e[93mINFO\e[0m] Reference test has timeout") 
      end
    end
    if !@ref_timeout && @timeout then
      errors += 1
      if extended_output then
        puts("    [\e[95mWARN\e[0m] Timeout")
      end
    elsif @ref_timeout && !@timeout then
      errors += 1
      if extended_output then
        puts("    [\e[95mWARN\e[0m] Did not timeout, but reference test did")
      end
    elsif !@ref_timeout && !@timeout then
      if @ref_stdout != @stdout then
        errors += 1
        if extended_output then
          puts("    [\e[95mWARN\e[0m] Diff in stdout")
          puts("      expected :")
          print("\e[7m")
          puts(@ref_stdout)
          print("\e[0m")
          puts("      got      :")
          print("\e[7m")
          puts(@stdout)
          print("\e[0m")
        end
      end
      if @err_eq && @ref_stderr != @stderr then
        errors += 1
        if extended_output then
          puts("    [\e[95mWARN\e[0m] Diff in stderr")
          puts("      expected :")
          print("\e[7m")
          puts(@ref_stderr)
          print("\e[0m")
          puts("      got      :")
          print("\e[7m")
          puts(@stderr)
          print("\e[0m")
        end
      end
      if @ref_exit_code != @exit_code then
        errors += 1
        if extended_output then
          puts("    [\e[95mWARN\e[0m] Diff in exit code")
          puts("      expected : " + @ref_exit_code.to_s)
          puts("      got      : " + @exit_code.to_s)
        end
      end
    end
    if $sanity then
      sanity_out_xml = File.read("outxml")
      sanity_out_end = File.read("outend")
      while (sanity_out_xml =~ /<error>(.*?)<\/error>(.*)/m) != nil do
        error_content = $1
        sanity_out_xml = $2
        errors += 1
        if extended_output then
          if (error_content =~ /<kind>(.*)<\/kind>/m) != nil then
            error_kind = $1
            puts("    [\e[95mWARN\e[0m] Memory error : " + error_kind)
          else
            puts("    [\e[95mWARN\e[0m] Memory unknown error")
          end
          if (error_content =~ /<stack>(.*)<\/stack>/m) != nil then
            stack_content = $1
            puts("      stack :")
            print("\e[7m")
            while (stack_content =~ /<frame>(.*?)<\/frame>(.*)/m) != nil do
              frame_content = $1
              stack_content = $2
              frame_content =~ /<obj>(.*)<\/obj>/
              obj = $1
              frame_content =~ /<fn>(.*)<\/fn>/
              fn = $1
              frame_content =~ /<file>(.*)<\/file>/
              file = $1
              frame_content =~ /<line>(.*)<\/line>/
              line = $1
              if true || obj != nil && obj.index($bin_name) != nil then
                if file != nil && line != nil
                  puts("function : " + fn + " in " + file + ":" + line)
                else
                  puts("function : " + fn)
                end
              end
            end
            print("\e[0m")
          end
        end
      end
      while (sanity_out_end =~ /Open file descriptor ([[:digit:]]*):(.*?)\n(.*)/m) != nil do
        sanity_out_end = $3
        fd_num = $1.to_i
        fd_path = $2
        if (sanity_out_end =~ /<inherited from parent>/) == nil && fd_num != 0 && fd_num != 1 && fd_num != 2 then
          errors += 1
          if extended_output then
            puts("    [\e[95mWARN\e[0m] File descriptor not closed : " + fd_num.to_s)
            if fd_path != nil && fd_path.size > 1 then
              puts("      path : " + fd_path)
            end
          end
        end
      end
    end
    if errors == 0 then
      puts("    [\e[92mPASS\e[0m] " + @name)
    else
      if extended_output then
        if errors == 1 then
          puts("    [\e[91mFAIL\e[0m] " +  @name + " : one error")
        else
          puts("    [\e[91mFAIL\e[0m] " +  @name + " : " + errors.to_s + " errors")
        end
      else
        puts("    [\e[91mFAIL\e[0m] " +  @name)
      end
    end
    if extended_output then
      puts("")
    end
    return errors
  end
end

class TestGroup
  def initialize(name)
    @group = name
    @tests = []
    Dir.glob(name + "/*.t").map.with_index do |file, i|
      @tests[i] = Test.new(file)
    end
  end
  def run_tests(out)
    self.print_name
    puts("")
    fails = 0
    @tests.map do |test|
      if !test.run_test(out) then
        fails += 1
      end
    end
    if !out then
      puts("")
    end
    if fails == 0 then
      puts("  Sucess : All tests passed.")
    else
      if fails == 1 then
        puts("  Failure : one test failed.")
      else
        puts("  Failure : " + fails.to_s + " tests failed.")
      end
    end
  end
  def print_name
    puts("  \e[4mCategory\e[0m : " + @group)
  end
end

class GroupList
  def initialize(names)
    @groups = []
    if names != nil then
      names.map do |dir|
        if Dir.exist?(dir) then
          @groups << TestGroup.new(dir)
        else
          raise "Test category " + dir + " not found."
        end
      end
    else
      Dir.foreach(".") do |dir|
        if Dir.exist?(dir) && dir != "." && dir != ".." then
          @groups << TestGroup.new(dir)
        end
      end
    end
  end
  def list
    @groups.map do |group|
      group.print_name
    end
  end
  def run(out)
    @groups.map do |group|
      group.run_tests(out)
      puts("")
    end
  end
end

# Arguments parsing :

$mode = :run
$cat = :all
$cat_list = []
$out = :default
$sanity = false
$bin_name = $Bin_default_name
$timeout = $Timeout_default

parsing_cat = false
parsing_bin = false
parsing_time = false
ARGV.each do |arg|
  if parsing_cat then
    if arg[0] == "-" then
      parsing_cat = false
    else
      $cat_list << arg.gsub("/", "");
    end
  elsif parsing_bin then
    if arg[0] == "-" then
       raise "Expected binary name after -b"
    else
      $bin_name = arg
    end
  elsif parsing_time then
    if arg[0] == "-" then
       raise "Expected timeout duration after -t"
    else
      $timeout = arg.to_i
    end
  end
  if !parsing_cat && !parsing_bin && !parsing_time then
    if arg == "-l" || arg == "--list" then
      $mode = :list
    elsif arg == "-c" || arg == "--category" then
      $cat = :select
      parsing_cat = true
    elsif arg == "-b" || arg == "--binary" then
      parsing_bin = true
    elsif arg == "-t" || arg == "--timeout" then
      parsing_time = true
    elsif arg == "-e" || arg == "--extended_output" then
      $out = :extended
    elsif arg == "-s" || arg == "--sanity" then
      $sanity = true
    else
      raise "Unknown argument : " + arg
    end
  else
    parsing_bin = false
    parsing_time = false
  end
end

# creating the group list

if $cat == :all then
  $groups = GroupList.new(nil)
elsif $cat == :select then
  $groups = GroupList.new($cat_list)
end

# Running

puts("TEST SUITE for 42sh project")
if $mode == :list then
  puts("\e[1mAvaible testing categories :\e[0m")
  puts("")
  $groups.list
elsif $mode == :run then
  puts("\e[1mRunning tests :\e[0m")
  puts("")
  if $out == :default then
    $groups.run(false)
  elsif $out == :extended then
    $groups.run(true)
  end
end
