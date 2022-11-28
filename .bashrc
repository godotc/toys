#
# ~/.bashrc
#

# If not running interactively, don't do anything
source /etc/bash.bashrc
[[ $- != *i* ]] && return

alias ls='ls --color=auto'


# ls background color
eval "$(dircolors -p | \
	sed 's/ 4[0-9];/ 01;/; s/;4[0-9];/;01;/g; s/;4[0-9] /;01 /' | \
    	dircolors /dev/stdin)"
PROMPT_COMMAND=build_prompt

build_prompt() {
  EXIT=$?               # save exit code of last command
  red='\[\e[0;31m\]'    # colors
  green='\[\e[0;32m\]'
  cyan='\[\e[1;36m\]'
  diy='\[\e[0;25m\]'
  PS1='${debian_chroot:+($debian_chroot)}'  # begin prompt

  if [ $EXIT != 0 ]; then  # add arrow color dependent on exit code
    PS1+="$red"
  else
    PS1+="$green"
  fi

  #PS1+="→$reset  $cyan\W$reset \\$ " # construct rest of prompt
  PS1+="→$reset  $cyan\W$reset $diy$ " # construct rest of prompt
}

export LANG=zh_CN.UTF-8

export DISPLAY=172.31.208.1:0
export PYTHONSTARTUP=~/.pythonstartup
export PATH="$PATH:$(go env GOPATH)/bin"
export PATH=$PATH:/home/gloria/.local/bin

export PYENV_ROOR="~/.pyenv"
export PATH=$PYENV_ROOT/shims:$PATH
eval "$(pyenv init -)"
#eval "$(pyenv virtualenv-init -)"

eval "$(thefuck --alias)"


source /mbin/win_proxy.sh set
alias w_proxy="source /mbin/win_proxy.sh"
source /mbin/change_comiler_alias.sh clang
alias set_c_toolchain="source /mbin/change_comiler_alias.sh"

alias gr="go run"
alias py="python"
alias tat="tmux attach -t"
alias sshRoot="ssh root@godot.link"
