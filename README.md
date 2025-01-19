# 🚧  Minishell (WIP) 🚧

A minimal shell implementation inspired by Bash.

## 📋 Checklist

### Built-in Commands
- [x] `echo` with option `-n`
  - [ ] Delete " and '
- [x] `cd` with relative or absolute path
- [x] `pwd` without options
- [x] `export` without options
  - [x] export VAR+=
  - [x] export VAR=
- [x] `unset` without options
- [x] `env` without options or arguments
- [x] `exit` without options

### Command Line Parsing
- [x] Single quotes `'` prevent interpretation of meta-characters
- [x] Double quotes `"` prevent interpretation except for `$`
- [x] Basic command execution with PATH search
- [x] Arguments handling
- [x] Environment variables expansion (`$VAR`)
- [ ] Check expansion (`$$` => `$`)
- [ ] Exit status variable expansion (`$?`)

### Redirections
- [x] `<` input redirection
- [x] `>` output redirection
- [x] `>>` append output redirection
- [x] `<<` heredoc
- [x] Pipes `|` between commands

### Signal Handling
- [x] `Ctrl-C` displays new prompt
- [x] `Ctrl-D` exits shell
- [x] `Ctrl-\` does nothing

### Error Handling
- [ ] Command not found (return 127)
- [x] Permission denied
- [x] File not found
- [ ] Syntax errors
- [ ] Memory leaks prevention

### Edge Cases
- [x] Empty input
- [x] Multiple spaces between arguments
- [ ] Multiple redirections
- [x] Multiple pipes
- [x] Quotes inside quotes
- [x] Environment variables in different contexts
- [x] Non-existent environment variables
- [ ] Invalid file descriptors

## 🛠️ Building

```bash
make run
```
