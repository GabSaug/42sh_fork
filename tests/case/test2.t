<test>
  <name>simple last field</name>
  <input_type>no input</input_type>
  <args>-c 'test=wrong; case $test in 4242) echo fail; ;; *) echo success; ;; esac'</args>
  <validation>sh</validation>
</test>
