<test>
  <name>(command returning true) && (command returning false)</name>
  <input_type>no input</input_type>
  <args>-c 'ls && cat sdfqsf'</args>
  <err_eq>false</err_eq>
  <validation>sh</validation>
</test>
