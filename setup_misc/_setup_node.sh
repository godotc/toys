
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

$INSTALL nodejs
$INSTALL npm

sudo npm config set strict-ssl false --global
npm install n -g
n use latest
	sudo npm install yarn -g
	sudo npm install pnpm -g
	sudo yarn config set strict-ssl false --global
	sudo pnpm config set strict-ssl false --global

cd ~
