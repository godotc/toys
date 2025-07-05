import contextlib
import os
import platform
import subprocess
import sys
import shutil

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
HOME_DIR = os.path.expanduser("~")

SCRIPT_DIR=  SCRIPT_DIR.replace("\\", "/")
print(f"SCRIPT_DIR={SCRIPT_DIR}")
assert SCRIPT_DIR.endswith("toys/setup_misc")

def run_command(command, shell=False):
    """Run a shell command and check for errors."""
    try:
        print(f"run> {command}")
        subprocess.run(command, shell=shell, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        #sys.exit(1)

def install_packages(install_command, packages):
    """Install packages using the specified install command."""
    for package in packages:
        if  isinstance(install_command, list):
            cmd = []
            cmd .extend(install_command)
            cmd.extend( [package])
            run_command(cmd)
        else:
            run_command([install_command, package])


def create_symlink(src, dst):
    """
    Create a symbolic link. Works on both Linux and Windows.
    :param src: The source file or directory.
    :param dst: The destination path for the symbolic link.
    """
    src = os.path.abspath(src)
    dst = os.path.abspath(dst)

    # Ensure the destination directory exists
    if not os.path.exists(os.path.dirname( dst)):
        print(f"Creating directory: {os.path.dirname(dst)}")
        os.makedirs(os.path.dirname(dst))

    # Remove the destination if it already exists
    print(f"Destination exists: {os.path.exists(dst)}")
    print(f"Destination is a symlink: {os.path.islink(dst)}")
    print(f"Destination is a directory: {os.path.isdir(dst)}")
    print(f"Destination is a file: {os.path.isfile(dst)}")
    if os.path.exists(dst) or os.path.islink(dst):
        print(f"Removing existing file or directory or link: {dst}")
        try: 
            if os.path.isdir(dst):
                os.rmdir(dst)
            elif os.path.islink(dst):
                os.remove(dst)
            else:
                os.remove(dst)
        except Exception as e:
            prinf(f"failed to remvoe {dst}, {e}")

    # Create the symbolic link
    print(f"os.symlink ({src}, {dst})")
    os.symlink(src, dst)

    print(f"Symbolic link created: {dst} -> {src}")
    print("-----------------------------")


def install_vim_plug():

    system = platform.system()
    # Install vim-plug for Vim and Neovim


    if system == "Linux":
        vim_plug_dir = os.path.join("/tmp/vim-setup", "vim-plug")
        if not os.path.exists(vim_plug_dir):
            run_command(["git", "clone", "https://github.com/junegunn/vim-plug.git", vim_plug_dir])

        plug_vim_source = os.path.join(vim_plug_dir, "plug.vim")
        plug_vim_targets = [
                # vim
                os.path.join(HOME_DIR, ".vim", "autoload", "plug.vim"),
                # neovim
                os.path.join( 
                        os.getenv( "XDG_DATA_HOME",
                        os.path.join(HOME_DIR, ".local", "share") # default?
                        ),
                    "nvim", "site", "autoload", "plug.vim") 
            ]

        for target in plug_vim_targets:
            create_symlink(plug_vim_source, target)

    elif system == "Windows":

        # vim
        vim_plug_vim_target = os.path.join(HOME_DIR, "vimfiles", "autoload", "plug.vim")
        run_command(["powershell", "-Command", f'iwr -useb https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim | ni "{vim_plug_vim_target}" -Force'])

        # Install vim-plug for Neovim
        LOCALAPPDATA_DIR = os.getenv("LOCALAPPDATA", os.path.join(HOME_DIR, "AppData", "Local"))

        nvim_data_dir = os.getenv( "XDG_DATA_HOME", os.path.join(LOCALAPPDATA_DIR, "nvim-data"))
        vim_plug_nvim_target = os.path.join(nvim_data_dir, "site", "autoload", "plug.vim")
        run_command(["powershell", "-Command", f'iwr -useb https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim | ni "{vim_plug_nvim_target}" -Force'])


def set_fish_as_default_shell():
    # Get the full path of Fish
    fish_path = shutil.which("fish")
        
    if not fish_path:
        print("Fish shell is not installed. Please install Fish first.")
        return

    # Check if Fish is already in /etc/shells
    with open("/etc/shells", "r") as f:
        content = f.read()
        if fish_path in content:
            print(f"Fish is already listed in /etc/shells: {fish_path}")
        elif "/bin/fish" in content:
            fish_path = "/bin/fish"
        else:
            # Add Fish to /etc/shells
            try:
                with open("/etc/shells", "a") as f:
                    f.write(f"{fish_path}\n")
                print(f"Added Fish to /etc/shells: {fish_path}")
            except PermissionError:
                print("Permission denied. Run the script with sudo.")
                return

    # Set Fish as the default shell
    try:
        subprocess.run(["chsh", "-s", fish_path], check=True)
        print(f"Fish shell has been set as the default shell: {fish_path}")
    except subprocess.CalledProcessError as e:
        print(f"Failed to set Fish as the default shell: {e}")


def main():
    # Determine the operating system
    system = platform.system()

    # Change directory to the setup_misc directory
    setup_misc_dir = SCRIPT_DIR

    # Create symbolic links
    files_to_link= []
    if system == "Linux":
        files_to_link = [
            (".bashrc", os.path.join(HOME_DIR, ".bashrc")),
            (".vimrc", os.path.join(HOME_DIR, ".vimrc")),
            (".ideavimrc", os.path.join(HOME_DIR, ".ideavimrc")),
            ("coc-settings.json", os.path.join(HOME_DIR, ".config", "nvim", "coc-settings.json")),
            ("coc-settings.json", os.path.join(HOME_DIR, ".vim", "coc-settings.json")),

            ("init.lua", os.path.join(HOME_DIR, ".config", "nvim", "init.lua")),
            ("init.lua",  os.path.join(HOME_DIR, ".vimrc.lua")),

            ("config.fish", os.path.join(HOME_DIR, ".config", "fish", "config.fish")),
            ("config.fish", os.path.join(HOME_DIR,  ".fishrc")),
        ]
    elif system == "Windows":
        files_to_link = [
            (".vimrc", os.path.join(HOME_DIR, ".vimrc")),
            (".ideavimrc", os.path.join(HOME_DIR, ".ideavimrc")),
            (".vsvimrc", os.path.join(HOME_DIR, ".vsvimrc")),
            (".vscodevimrc", os.path.join(HOME_DIR, ".vscodevimrc")),

            ("init.lua", os.path.join(HOME_DIR, "AppData", "Local", "nvim", "init.lua")),
            ("init.lua",  os.path.join(HOME_DIR, ".vimrc.lua")),

            ("coc-settings.json", os.path.join(HOME_DIR, "AppData", "Local", "nvim", "coc-settings.json")),

            ("Profile.ps1", os.path.join(HOME_DIR, "Documents", "PowerShell", "Profile.ps1")),
            ("Profile.ps1", os.path.join(HOME_DIR,  ".pwshrc"))
        ]

    for source, target in files_to_link:
            create_symlink(os.path.join(setup_misc_dir, source), target)


    # install packages
    install_command = None
    if system == "Linux":
        # Check for package manager
        if subprocess.run(["which", "apt-get"], stdout=subprocess.PIPE).returncode == 0:
            run_command(["sudo", "apt-get", "update"])
            run_command(["sudo", "apt-get", "upgrade", "-y"])
            install_command = "sudo apt-get install -y"
        elif subprocess.run(["which", "pacman"], stdout=subprocess.PIPE).returncode == 0:
            run_command(["sudo", "pacman", "-Syyu"])
            install_command = "sudo pacman -S --noconfirm"
        elif subprocess.run(["which", "yum"], stdout=subprocess.PIPE).returncode == 0:
            run_command(["sudo", "yum", "update", "-y"])
            install_command = "sudo yum install -y"
        else:
            print("Unsupported package manager. Please define PKG variable manually.")
            sys.exit(1)

        install_command = install_command.split(" ")

        # List of packages to install
        packages = ["git", "clang", "llvm", "nodejs", "npm", "fish"]

        # Install packages
        install_packages(install_command, packages)
    elif system == "Windows":
        install_command = "winget install"
        install_command = install_command.split(" ")

        # List of packages to install using winget
        packages = ["vim.vim", "neovim.neovim", "llvm.llvm"]
        install_packages(install_command, packages)

    else:
        print("Unsupported operating system.")
        sys.exit(1)

    if system == "Linux" :
        set_fish_as_default_shell()
    install_vim_plug()

    # Change back to home directory
    os.chdir(HOME_DIR)

if __name__ == "__main__":
    main()

