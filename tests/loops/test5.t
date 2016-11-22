<test>
  <name>while with variable in condition</name>
  <input_type>no input</input_type>
  <args>-c 'var=5; while [ $var -eq 5 ]; do var=4; ls; done'</args>
  <validation>sh</validation>
</test>
