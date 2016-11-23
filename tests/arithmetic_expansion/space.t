<test>
  <name>constant with/without space</name>
  <input_type>no input</input_type>
  <args>-c 'echo $((5)); echo $(( 95)); echo $((4 )); echo $((    87   ))'</args>
  <validation>sh</validation>
</test>
