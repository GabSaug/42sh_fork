<test>
  <name>if (true) returning false</name>
  <input_type>no input</input_type>
  <args>-c 'if true; then cat dfqsfsdgf; fi'</args>
  <err_eq>false</err_eq>
  <validation>sh</validation>
</test>
