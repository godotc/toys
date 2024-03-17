#
# ~/.bashrc

# If not running interactively, don't do anything
source /etc/bash.bashrc
[[ $- != *i* ]] && return



alias ls='ls --color=auto'
# ls background color
eval "$(dircolors -p | \
	sed 's/ 4[0-9];/ 01;/; s/;4[0-9];/;01;/g; s/;4[0-9] /;01 /' | \
    	dircolors /dev/stdin)"


PROMPT_COMMAND=build_prompt

is_git_repository() {
	git branch > /dev/null 2>&1
}

# Determine the branch/state information for this git repository.
set_git_branch() {
	local state remote branch branch_pattern # keep local

	if ! is_git_repository ; then
		BRANCH=""
		return 1
	fi

# Capture the output of the "git status" command.
		git_status="$(git status 2> /dev/null)"

		# Set color based on clean/staged/dirty.
		if [[ ${git_status} =~ "working tree clean" ]]; then
			state="${GREEN}"
		elif [[ ${git_status} =~ "Changes to be committed" ]]; then
			state="${YELLOW}"
		else
			state="${RED}"
		fi

		# Set arrow icon based on status against remote.
		remote_pattern="# Your branch is (ahead|behind)+ "
		if [[ ${git_status} =~ ${remote_pattern} ]]; then
			if [[ ${BASH_REMATCH[1]} == "ahead" ]]; then
				remote="↑"
			else
				remote="↓"
			fi
		else
			remote=""
		fi

		diverge_pattern="# Your branch and (.*) have diverged"
		if [[ ${git_status} =~ ${diverge_pattern} ]]; then
			remote="↕"
		fi

	  # Get the name of the branch.
	  branch_pattern="^(# )?On branch ([^${IFS}]*)"
	  if [[ ${git_status} =~ ${branch_pattern} ]]; then
		  branch=${BASH_REMATCH[2]}
	  fi

	  # Set the final branch string.
	  BRANCH=${state}"(${branch})"${remote}${NC}
}


build_prompt() {
	EXIT=$?               # save exit code of last command

   green='\[\e[0;32m\]'
   cyan='\[\e[1;36m\]'
   diy='\[\e[0;25m\]'
   purple='\[\e[0;36m\]'
   

   YELLOW='\[\e[1;33m\]'
   GREEN='\[\e[0;32m\]'
   BLUE='\[\e[1;34m\]'
   RED='\[\e[0;31m\]'
   LIGHT_RED='\[\e[1;31m\]'
   LIGHT_GREEN='\[\e[1;32m\]'
   WHITE='\[\e[1;37m\]'
   LIGHT_GRAY='\[\e[0;37m\]'


  PS1='${debian_chroot:+($debian_chroot)}'  # begin prompt

  if [ $EXIT != 0 ]; then  # add arrow color dependent on exit code
    PS1+="$red"
  else
    PS1+="$green"
  fi

  local BRANCH
  set_git_branch

  if is_git_repository ; then
	  BRANCH=$WHITE:${BRANCH}
  fi

  #PS1+="→$reset  $cyan\W$reset \\$ " # construct rest of prompt
  #PS1+="→$reset  $cyan\W$reset:$reset$WHITE${BRANCH}$diy\$ " # construct rest of prompt
  PS1+="→  $cyan\W${BRANCH}$WHITE \$ " # construct rest of prompt
}





export PATH="$PATH:$(go env GOPATH)/bin"
export PATH="$PATH:$(go env GOROOT)/bin"

export PYENV_ROOR="~/.pyenv"
export PATH=$PYENV_ROOT/shims:$PATH
eval "$(pyenv init -)"
#eval "$(pyenv virtualenv-init -)"




# Cargo cannot add 
export CARGO_NET_GIT_FETCH_WITH_CLI=true

export NODE_PATH=/usr/lib/node_modules


alias gr="go run"
alias py="python"
alias tat="tmux attach -t"
alias sshRoot="ssh root@godot.link"
alias dk=docker
alias pm=pacman
alias nhh='npx hardhat'
alias v=nvim

source ~/.bashrc_locale

# >>> xmake >>>
test -f "/home/hypercreep/.xmake/profile" && source "/home/hypercreep/.xmake/profile"
alias xmk=xmake
alias xmr="xmake b && xmake r"
# <<< xmake <<<
. "$HOME/.cargo/env"
