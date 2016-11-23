<test>
  <name>expansion with quote</name>
  <input_type>no input</input_type>
  <args>-c 'a=\'""\'; echo $a'</args>
  <validation>sh</validation>
  <err_eq>false</err_eq>
</test>
