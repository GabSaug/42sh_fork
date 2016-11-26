<test>
  <name>parameter expansion in assignment</name>
  <input_type>no input</input_type>
  <args>-c 'first_name=julien; name=gautier; full_name="$first_name $name"; echo "firs_name is ${first_name} and name is $name and full_name is $full_name; echo test'</args>
  <validation>sh</validation>
</test>
