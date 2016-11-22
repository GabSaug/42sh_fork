<test>
  <name>no match</name>
  <input_type>no input</input_type>
  <args>-c 'test=4242; case $test in 21) echo fail1; ;; 33) echo fail2; ;; esac'</args>
  <validation>sh</validation>
</test>
