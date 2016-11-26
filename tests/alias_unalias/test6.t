<test>
  <name>unalias multiple</name>
  <input_type>no input</input_type>
  <args>-c "alias test='echo blabla' asd='echo ppp'; unalias test asd; alias"</args>
  <validation>bash</validation>
</test>
