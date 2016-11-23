<test>
  <name>prefix and element with redirection</name>
  <input_type>no input</input_type>
  <args>-c '>out a=b > out2 echo salut > out3 echo bonjour; echo out; cat out; echo out2; cat out2; echo out3; cat out3'</args>
  <validation>sh</validation>
</test>
