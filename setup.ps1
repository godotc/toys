
winget install vim.vim
winget install neovim.neovim
winget install llvm.llvm

# install plug.vim for vim
iwr -useb https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim |`
    ni $HOME/vimfiles/autoload/plug.vim -Force

# install plug.vim for neovim
iwr -useb https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim |`
    ni "$(@($env:XDG_DATA_HOME, $env:LOCALAPPDATA)[$null -eq $env:XDG_DATA_HOME])/nvim-data/site/autoload/plug.vim" -Force

#New-Item -ItemType SymbolicLink -Path ~/.bashrc -Target  "$(Get-Location)/.bashrc"

New-Item -ItemType SymbolicLink -Path ~/.vimrc -Target  "$(Get-Location)/.vimrc"
mkdir ~/AppData/Local/nvim
New-Item -ItemType SymbolicLink -Path ~/AppData/Local/nvim/init.lua -Target  "$(Get-Location)/init.lua"
New-Item -ItemType SymbolicLink -Path ~/AppData/Local/nvim/coc-settings.json -Target  "$(Get-Location)/coc-settings.json"
