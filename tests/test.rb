#! /usr/bin/env ruby

# Test Suite in Ruby for 42sh project
# Configuration variables :

$bin_name = "sh"
$Timeout = 5

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
          @file = $1
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
    if (test_content =~ /<validation>(.*)<\/validation>/) != nil then
      @validation = $1
      if @validation == "manual" then
        test_content =~ /<stdout>(.*)<\/stdout>/m
        @ref_stdout = File.read(path.lines("/")[0] + $1)
        test_content =~ /<stderr>(.*)<\/stderr>/m
        @ref_stderr = File.read(path.lines("/")[0] + $1)
        test_content =~ /<exit_code>(.*)<\/exit_code>/
        @ref_exit_code = $1.to_i
      else
        self.run_ref
      end
    else
      raise "No <validation> balise in " + path +
      " test file (mandatory balise)."
    end
  end
  def gen_command(bin)
    command = bin + " " + @additional_args
    if @type == "file" then
      command += " " + @file
    end
    return command
  end
  def run_ref
    command = self.gen_command(@validation)
    @ref_stdout = ""
    Open3.popen3(command) do |stdin, stdout, stderr, t|
      if @type == "tty" then
        input_content = File.read(@file)
        stdin.puts(input_content)
      end
      begin
        Timeout.timeout($Timeout) do
          while (line = stdout.gets) do
            @ref_stdout += line
          end
        end
      rescue Timeout::Error => ex
        Process.kill(9, t.pid)
      end
      stdin.close
      @ref_stderr = stderr.read
      @ref_exit_code = t.value.exitstatus
    end
  end
  def print_result(extended_output)
    errors = 0
    if extended_output then
      puts("    [\e[93mINFO\e[0m] " + @name + " started") 
    end
    if @timeout then
      errors += 1
      if extended_output then
        puts("    [\e[95mWARN\e[0m] Timeout")
      end
    else
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
      if @ref_stderr != @stderr then
        errors += 1
        if extended_output then
          puts("    [\e[95mWARN\e[0m] Diff in stderr")
          puts("      expected :")
          print("\e[7m")
          puts(@ref_stderr)
          print("\e[0m")
          puts("        got      :")
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
    if errors == 0 then
      puts("    [\e[92mPASS\e[0m] " + @name)
    else
      puts("    [\e[91mFAIL\e[0m] " +  @name)
    end
    if extended_output then
      puts("")
    end
    return errors
  end
  def run_test(out)
    command = self.gen_command($bin_name)
    @stdout = ""
    @timeout = false
    Open3.popen3(command) do |stdin, stdout, stderr, t|
      if @type == "tty" then
        input_content = File.read(@file)
        stdin.puts(input_content)
      end
      stdin.close
      begin
        Timeout.timeout($Timeout) do
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

parsing_cat = false
parsing_bin = false
ARGV.each do |arg|
  if parsing_cat then
    if arg[0] == "-" then
       parsing_cat = false
    else
       $cat_list << arg
    end
  elsif parsing_bin then
    if arg[0] == "-" then
       raise "Expected binary name after -b"
    else
       $bin_name = arg
       parsing_bin = false
    end
  else
    if arg == "-l" || arg == "--list" then
      $mode = :list
    elsif arg == "-c" || arg == "--category" then
      $cat = :select
    elsif arg == "-b" || arg == "--binary" then
      parsing_bin = true
    elsif arg == "-e" || arg == "--extended_output" then
      $out = :extended
    else
      raise "Unknown argument : " + arg
    end
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
