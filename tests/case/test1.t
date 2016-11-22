<test>
  <name>simple first field</name>
  <input_type>no input</input_type>
  <args>-c 'test=4242; case $test in 4242) ls; ;; *) echo fail; ;; esac'</args>
  <validation>sh</validation>
</test>
