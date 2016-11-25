<test>
  <name>various variable expansion in one token 2</name>
  <input_type>no input</input_type>
  <args>-c 'var=VALUE; echo begin${var}$((1+2+var))end$var'</args>
  <validation>sh</validation>
</test>
