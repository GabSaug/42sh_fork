<test>
  <name>pipe hard</name>
  <input_type>no input</input_type>
  <args>-c 'echo pandalama > pipetest; cat -e pipetest | rev | cat -e; rm pipetest'</args>
  <validation>sh</validation>
</test>
