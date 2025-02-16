
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

mkdir ~/AppData/Local/nvim
mkdir ~/Documents/PowerShell

function Force-Add-SymLink($dst, $src, $bRelative=$true) {
    if (Test-Path $dst) {
        Remove-Item $dst
    }
    if($bRelative) {
        New-Item -ItemType SymbolicLink -Path $dst -Target (Resolve-Path $src).Path
    }else{
        New-Item -ItemType SymbolicLink -Path $dst -Target $src
    }
}



Force-Add-SymLink  ~/.vimrc 	    ./.vimrc
Force-Add-SymLink  ~/.ideavimrc     ./.ideavimrc
Force-Add-SymLink  ~/.vsvimrc       ./.vsvimrc
Force-Add-SymLink  ~/.vscodevimrc   ./.vscodevimrc
Force-Add-SymLink  ~/AppData/Local/nvim/init.lua            ./init.lua
Force-Add-SymLink  ~/AppData/Local/nvim/coc-settings.json   ./coc-settings.json
Force-Add-SymLink  ~/Documents/PowerShell/Profile.ps1       ./Profile.ps1
