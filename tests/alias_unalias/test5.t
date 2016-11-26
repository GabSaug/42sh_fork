<test>
  <name>unalias simple</name>
  <input_type>no input</input_type>
  <args>-c "alias test='echo blabla'; unalias test; alias"</args>
  <validation>bash</validation>
</test>
