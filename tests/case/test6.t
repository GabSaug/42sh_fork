<test>
  <name>simple choice in array</name>
  <input_type>no input</input_type>
  <args>-c 'test=4; case $test in [12345]) ls; ;; *) echo fail; ;; esac'</args>
  <validation>sh</validation>
</test>
