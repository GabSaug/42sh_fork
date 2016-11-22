<test>
  <name>simple globing 2</name>
  <input_type>no input</input_type>
  <args>-c 'test=4242; case $test in 4??2) echo success; ;; 4?2) echo fail; ;; esac'</args>
  <validation>sh</validation>
</test>
