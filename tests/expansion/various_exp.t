<test>
  <name>various variable expansion in one token</name>
  <input_type>no input</input_type>
  <args>-c 'var=VALUE; echo begin${var}$((1+2))end$var'</args>
  <validation>sh</validation>
</test>
