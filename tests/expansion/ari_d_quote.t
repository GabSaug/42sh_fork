<test>
  <name>double quote in arithmetic expansion</name>
  <input_type>no input</input_type>
  <args>-c 'echo $(( 5 + "8" ))'</args>
  <validation>sh</validation>
</test>
