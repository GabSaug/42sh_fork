<test>
  <name>if (binary returning false)</name>
  <input_type>no input</input_type>
  <args>-c 'if cat sdqfqsf; then ls; fi'</args>
  <err_eq>false</err_eq>
  <validation>sh</validation>
</test>
