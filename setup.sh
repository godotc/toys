
#!/bin/bash

INSTALL=""

# Check the system type
if [ "$(uname -s)" = "Linux" ]; then
    # Check for package manager
    if command -v apt-get &> /dev/null; then
		sudo apt-get update && sudo apt-get upgrade
        INSTALL="sudo apt-get install"
    elif command -v pacman &> /dev/null; then
		sudo pacman -Syyu
        INSTALL="sudo pacman -S"
    else
        echo "Unsupported package manager. Please define PKG variable manually."
        exit 1
    fi
else
    echo "Unsupported operating system. Please define PKG variable manually."
    exit 1
fi

set -x

# Use PKG variable to install a package
$INSTALL git
$INSTALL clangd
$INSTALL llvm
$INSTALL nodejs
$INSTALL npm

cd ~
# thought you already in this repo
#git clone git@github.com:godotc/toys.git
cd toys
PWD=`pwd`
rm ~/.vimrc ~/.bashrc ~/.config/nvim/coc-settings.json ~/.vim/coc-settings.json
ln -s $PWD/.bashrc ~/.bashrc
ln -s $PWD/.vimrc ~/.vimrc
ln -s $PWD/coc-settings.json  ~/.config/nvim/coc-settings.json
ln -s $PWD/coc-settings.json  ~/.vim/coc-settings.json


mkdir /tmp/vim-setup
cd /tmp/vim-setup/
git clone git@github.com:junegunn/vim-plug.git
cp vim-plug/plug.vim ~/.vim/autoload/
cp vim-plug/plug.vim "${XDG_DATA_HOME:-$HOME/.local/share}"/nvim/site/autoload/

set +x

cd ~
