<test>
  <name>multiple variable expansion in one token</name>
  <input_type>no input</input_type>
  <args>-c 'var=VALUE; echo begin${var}end;'</args>
  <validation>sh</validation>
</test>
