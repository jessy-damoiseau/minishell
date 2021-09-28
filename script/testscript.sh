#!/bin/bash

#########################################################################
#		Script must be launched from its repository "script"			#
#########################################################################

path="script"
BUILTIN=( echo ls cd cat ) #env exit export -> a ajouter que qd pipe et env regle
PIPE=( pipe )
REDIR=( redir_left redir_right )
QUOTE=( quote )
LOGS=( bash us error_us error_bash diff diff_error )

cd ..
make

clean_logs()
{
	for log in ${LOGS[*]}
		do
			rm "$path"/"$log".log 2> /dev/null
	done
	exit 0
}

blt_tests()
{
	cd "$path" || exit
	for src in ${BUILTIN[*]}
		do
			../minishell < "$src".sh 1>> us.log 2>> error_us.log
			bash < "$src".sh 1>> bash.log 2>> error_bash.log
	done
}

pipe_tests()
{
	cd "$path" || exit
	for src in ${PIPE[*]}
		do
			../minishell < "$src".sh 1>> us.log 2>> error_us.log
			bash < "$src".sh 1>> bash.log 2>> error_bash.log
	done
}

redir_tests()
{
	cd "$path" || exit
	for src in ${REDIR[*]}
		do
			../minishell < "$src".sh 1>> us.log 2>> error_us.log
			bash < "$src".sh 1>> bash.log 2>> error_bash.log
	done
}

quote_tests()
{
	cd "$path" || exit
	for src in ${QUOTE[*]}
		do
			../minishell < "$src".sh 1>> us.log 2>> error_us.log
			bash < "$src".sh 1>> bash.log 2>> error_bash.log
	done
}

test_check()
{
	if [[ "$1" = clean ]]
		then
			clean_logs
	elif [[ "$1" = blt ]]
		then
			blt_tests
	elif [[ "$1" = pipe ]]
		then
			pipe_tests
	elif [[ "$1" = redir ]]
		then
			redir_tests
	elif [[ "$1" = quote ]]
		then
			quote_tests
	fi
}

cut_prompt_in_uslog()
{
	for src in ${LOGS[*]}
		do
			sed -i -e "/script >>/d" "$path"/us.log
	done
}

do_diff()
{
	sdiff -s "$path"/us.log "$path"/bash.log > "$path"/diff.log
	sdiff -s "$path"/error_us.log "$path"/error_bash.log > "$path"/diff_error.log
}

#clean_logs 2>&1 /dev/null

read -r -p 'Choose which tests you want to do ?
- For builtins tests, type "blt"
- To clean logs, type "clean"
- For pipe tests, type "pipe"
- For redirection tests, type "redir"
- For quote parsing tests, type "quote"
> ' test

if [ -z "$1" ]
	then
		test_check "$test"
fi
cd ..
cut_prompt_in_uslog
do_diff