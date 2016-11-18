<test>
  <name>if (true) with multiples command returning false</name>
  <input_type>no input</input_type>
  <args>-c 'if true; then ls; cat sdfqf; fi'</args>
  <err_eq>false</err_eq>
  <validation>sh</validation>
</test>
