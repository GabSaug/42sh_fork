<test>
  <name>various variable expansion in one token 3</name>
  <input_type>no input</input_type>
  <args>-c 'var=0; echo begin${var}$((1+2/var))end$var'</args>
  <validation>sh</validation>
  <err_eq>false</err_eq>
</test>
