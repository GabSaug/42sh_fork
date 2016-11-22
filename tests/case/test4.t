<test>
  <name>simple globing</name>
  <input_type>no input</input_type>
  <args>-c 'test=444; case $test in 4*) echo success; ;; 5*) echo fail; ;; esac'</args>
  <validation>sh</validation>
</test>
