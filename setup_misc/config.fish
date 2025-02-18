# ~/.config/fish/config.fish

# Source global Fish configuration
source /etc/fish/config.fish

# If not running interactively, don't do anything
if not status is-interactive
    exit
end

# Aliases
alias ls='ls --color=auto'

# Set ls background color(for bash)
#eval (dircolors -p | sed 's/ 4[0-9];/ 01;/; s/;4[0-9];/;01;/g; s/;4[0-9] /;01 /' | dircolors /dev/stdin)

function is_git_repository
    git rev-parse --git-dir > /dev/null 2>&1
end

function set_git_branch
    set -l state
    set -l remote
    set -l branch
    set -l git_status

    if not is_git_repository
        set -g BRANCH ""
        return 1
    end

    # Capture the output of the "git status" command
    set git_status (git status)
	#echo "Git Status:" $git_status

    # Set color based on clean/staged/dirty
    if string match -q "*working tree clean*" "$git_status"
		#echo "Repository is clean"
        set state (set_color green)
    else if string match -q "*Changes to be committed*" "$git_status"
		#echo "Changes to be committed"
        set state (set_color yellow)
    else
		#echo "Repository is dirty"
        set state (set_color red)
    end

    # Set arrow icon based on status against remote
    if string match -q "*Your branch is ahead*" "$git_status"
        set remote "↑"
    else if string match -q "*Your branch is behind*" "$git_status"
        set remote "↓"
    else if string match -q "*Your branch is up to date*" "$git_status"
        set remote ""
    end

    if string match -q "*Your branch and .* have diverged*" "$git_status"
        set remote "↕"
    end

    # Get the name of the branch
    if string match -rq "On branch ([^[:space:]]*)" "$git_status"
        set branch (string match -r "On branch ([^[:space:]]*)" "$git_status")[2]
    end

    # Set the final branch string
    set -g BRANCH "$state($branch)$remote"(set_color normal)
	#echo "Branch Info:" $BRANCH
end


# Build the prompt
function fish_prompt
    set -l exit_code $status
    set -l green (set_color green)
    set -l cyan (set_color cyan)
    set -l purple (set_color purple)
    set -l yellow (set_color yellow)
    set -l blue (set_color blue)
    set -l red (set_color red)
    set -l light_red (set_color brred)
    set -l light_green (set_color brgreen)
    set -l white (set_color white)
    set -l light_gray (set_color brwhite)

    # Set prompt color based on exit code
    if test $exit_code -ne 0
        set prompt_color $red
    else
        set prompt_color $green
    end

    # Set Git branch information
    set -g BRANCH
    set_git_branch

    if is_git_repository
        set BRANCH "$white:$BRANCH"
    end

	#echo $BRANCH


    # Construct the prompt

#set -l max_length 20
#echo -n "$prompt_color→ $cyan"$(echo "$PWD" | sed -e "s/\(.\{$max_length\}\).*/\1/")"$BRANCH"
#echo -n "$prompt_color→ $cyan"(pwd)"$BRANCH"
    echo -n "$prompt_color→ $cyan"(pwd)"$BRANCH"

	# 获取上一条命令的返回值
    set -l last_status $status

    # 设置颜色
    set -l status_color (set_color red)  # 默认红色表示错误
    if test $last_status -eq 0
        set status_color (set_color green)  # 绿色表示成功
		echo -n "$status_color[$last_status]"()  # 显示上一条命令的返回值
    end

	echo -n (set_color normal)"\$ "
end

# Add Go paths to PATH
set -x PATH $PATH (go env GOPATH)/bin
set -x PATH $PATH (go env GOROOT)/bin

# Pyenv configuration
set -x PYENV_ROOT ~/.pyenv
set -x PATH $PYENV_ROOT/shims $PATH
pyenv init - | source
# pyenv virtualenv-init - | source

# Cargo configuration
set -x CARGO_NET_GIT_FETCH_WITH_CLI true

# Node.js configuration
set -x NODE_PATH /usr/lib/node_modules

# Aliases
alias gr="go run"
alias py="python"
alias tat="tmux attach -t"
alias sshRoot="ssh root@godot.link"
alias dk="docker"
alias pm="pacman"
alias spm="sudo pacman"
alias nhh="npx hardhat"
alias v="nvim"

# Source local Fish configuration
source ~/.bashrc_locale.fish

# Cargo environment
source ~/.cargo/env

# WSL proxy configuration
set nameserver (grep -m 1 nameserver /etc/resolv.conf | awk '{print $2}')
set port 10809
set -x http_proxy http://$nameserver:$port
set -x https_proxy http://$nameserver:$port
set -x all_proxy http://$nameserver:$port

# Set DISPLAY if needed
# set -x DISPLAY $nameserver:0

# Xmake configuration
if test -f "/root/.xmake/profile"
    source "/root/.xmake/profile"
end
