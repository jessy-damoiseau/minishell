echo "--- TEST REDIR LEFT ---"
#A completer avec double redir left

ls > 0test.log
echo phase 1 ok
echo
sort < 0test.log
echo
echo phase 2 ok
echo
grep Makefile < 0test.log
echo
echo phase 3 ok
echo
rm 0test.log

echo coucou les zouzous > 0test.log
echo bonjour.sh >> 0test.log
echo salut.sh >> 0test.log
echo hello.sh >> 0test.log

#0test.log << cat Makefile -> fait planter le programme
#sort -n << FIN > testnb.txt -> a test manuellement

echo phase 1 ok
echo
sort < 0test.log
echo
echo phase 2 ok
echo
grep .sh < 0test.log
echo
echo phase 3 ok
echo
rm 0test.log

sort < 0test.log