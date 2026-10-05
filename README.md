# Archervie's Shell
This repository consists of a custom shell written in C named `avsh` (Archervie's Shell). 

--- 

## The Shell
Once you're in the shell, you can use it like a typical shell, being able to use your system's commands from the given `PATH`. The shell has several buit-in commands:
- `cd`: Change directories.
- `path`: Change the PATH of the shell.

You can exit the shell through `exit`. 

# Usage
To use the shell, simply clone the repository and use `make`. 
```
git clone https://github.com/archervie/avsh.git
cd avsh/
make
```

You can add the shell to `PATH` by copying the binary over to `~/.local/bin`: $ `cp avsh ~/.local/bin/`
And exporting the directory by adding it to `PATH`:
- For bash or zsh: $ export PATH="$HOME/.local/bin:$PATH"
- For fish: fish_add_path ~/.local/bin

You can delete object files through `make clean`. You can delete the binaries through `make fclean`.
