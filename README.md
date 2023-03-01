# minishell

<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/c/c-original.svg"  width="30" height="30" /> <img src="https://camo.githubusercontent.com/bbb327d6ba7708520eaafd13396fed64d73bf5df5c4cdd0ba03cf0843f7a9340/68747470733a2f2f7777772e766563746f726c6f676f2e7a6f6e652f6c6f676f732f676e755f626173682f676e755f626173682d69636f6e2e737667"  width="30" height="30" />

## Overview
 - Code our own mini bash following bash POSIX implementation as technical guideline
 - Learn about contextual parsing and execution, advanced file descriptor use, pipes, signal processing, processes and parent / child
 - As usual per 42 standard : no memory or fd leaks allowed, and project is up to 42's coding norm
## Specifications
 - Display a prompt when waiting for a new command.
 - Have a working history.
 - Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).
 - Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon).
 - Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
 - Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
 - Implement redirections:
    - '<' should redirect input.
    - '>' should redirect output.
    - '<<' should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
    - '>>' should redirect output in append mode.
 - Implement pipes (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
 - Handle environment variables ($ followed by a sequence of characters) which should expand to their values.
 - Handle $? which should expand to the exit status of the most recently executed foreground pipeline.
 - Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
 - In interactive mode:
 - ctrl-C displays a new prompt on a new line.
 - ctrl-D exits the shell.
 - ctrl-\ does nothing.
 - Your shell must implement the following builtins:
 - echo with option -n
 - cd with only a relative or absolute path
 - pwd with no options
 - export with no options
 - unset with no options
 - env with no options or arguments
 - exit with no options
 ## Credit
 @[Jdamoise](https://github.com/jessy-damoiseau) @[Pgueugno](https://github.com/plgueugnon)
