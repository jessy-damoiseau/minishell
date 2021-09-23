#!/bin/bash

#########################################################################
#		Script must be launched from its repository "script"			#
#########################################################################

path="script"
BUILTIN=( echo ls )
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
	cd ..
}

test_check()
{
	if [[ "$1" = clean ]]
		then
			clean_logs
	elif [[ "$1" = blt ]]
		then
			blt_tests
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

read -r -p 'Choose which tests you want to do ?
- For builtins tests, type "blt"
- To clean logs, type "clean"
> ' test

if [ -z "$1" ]
	then
		test_check "$test"
fi
cut_prompt_in_uslog
do_diff