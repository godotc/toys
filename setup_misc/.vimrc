
function! IsCppSourceFile()
  	let filetype = &ft
	if filetype == 'cpp'
		"echo filetype
		return 1
	endif 
	return 0
endfunc



call plug#begin()
"Plug 'ycm-core/YouCompleteMe'
Plug 'neoclide/coc.nvim', {'branch': 'release'}

Plug 'scrooloose/nerdtree'
Plug 'tpope/vim-surround'
Plug 'jiangmiao/auto-pairs' "括号配对
Plug 'voldikss/vim-floaterm'

"Plug 'vim-scripts/DoxygenToolkit.vim'

Plug 'itchyny/lightline.vim' "status line
Plug 'frazrepo/vim-rainbow' "彩虹括号

" Theme
Plug 'joshdick/onedark.vim'
Plug 'rakr/vim-one'

" FZF.vim
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'junegunn/fzf.vim'

Plug 'vim-scripts/a.vim'
Plug 'octol/vim-cpp-enhanced-highlight'
Plug 'pboettch/vim-cmake-syntax'

"Plug 'Exafunction/codeium.vim', { 'branch': 'main' }

"Plug 'nvim-treesitter/nvim-treesitter', {'do': ':TSUpdate'}
"Plug 'puremourning/vimspector'

" All of your Plugins must be added before the following line
Plug 'Exafunction/codeium.vim'
call plug#end()            " required



if has('win32') || has('win64')
    " This is Windows
    "echo "Running on Windows"
elseif has('unix')
    " This is Linux or macOS
    if system('uname -s') =~ "Linux"
        "echo "Running on Linux"
    elseif system('uname -s') =~ "Darwin"
        "echo "Running on macOS"
    endif
else
    "echo "Unknown operating system"
endif


"set termguicolors
"tmux Error color setting
" Enable true color 启用终端24位色
"if exists('+termguicolors')
"   let &t_8f = "\<Esc>[38;2;%lu;%lu;%lum"
"   let &t_8b = "\<Esc>[48;2;%lu;%lu;%lum"
"   set termguicolors
"endif


" cursor shape
if &term =~? 'rxvt' || &term =~? 'xterm' || &term =~? 'st-'
    " 1 or 0 -> blinking block
    " 2 -> solid block
    " 3 -> blinking underscore
    " 4 -> solid underscore
    " Recent versions of xterm (282 or above) also support
    " 5 -> blinking vertical bar
    " 6 -> solid vertical bar
    " Insert Mode
    let &t_SI .= "\<Esc>[6 q"
    " Normal Mode
    let &t_EI .= "\<Esc>[2 q"
endif

set laststatus=2
let g:lightline = {
      \ 'component_function': {
      \   'filename': 'LightlineFilename',
      \   'codeium': 'CodeiumStatusString',
      \ }
      \ }
function! LightlineFilename()
  return expand('%')  
endfunction
function! CodeiumStatusString()
	return eval(\{…\}%3{codeium#GetStatusString()})
endfunction

let g:rainbow_active=1

"tab are spaces
"set expandtab


set encoding=utf-8
" Some servers have issues with backup files, see #649.
set nobackup
set nowritebackup
syntax on
syntax enable
set shiftwidth=4
set tabstop=4
set cindent
set number
set foldmethod=indent
set autoindent
set showcmd
set mouse=a
set hlsearch
set incsearch

set path^=/usr/include


set list
set listchars=tab:→\ ,eol:↓

set timeoutlen=500

filetype indent on

" Command Line View
set wildmode=list:full 
"set wildmenu

"set background=dark " for the dark version
" set background=light " for the light version
colorscheme onedark


" Make <TAB> to accept selected completion item or notify coc.nvim to format
" <C-g>u breaks current undo, please make your own choice.
 "inoremap <silent><expr> <TAB>  coc#pum#visible() ? coc#pum#confirm():"\<TAB>"



function! DoComplete()
  if coc#pum#visible()
    return coc#pum#confirm()
  endif

  let status = codeium#GetStatusString()
  if status != "0" && status != "*"
    return codeium#Accept()
  endif

  return "\<TAB>"
endfunction

imap <script><silent><nowait><expr> <TAB> DoComplete()

let g:codeium_disable_bindings = 1
"imap <script><silent><nowait><expr> <C-l> codeium#AcceptNextLine()	
imap <script><silent><nowait><expr> <C-space> codeium#AcceptNextLine()
imap <C-Down>   <Cmd>call codeium#CycleCompletions(1)<CR>
imap <C-Up>   <Cmd>call codeium#CycleCompletions(-1)<CR>
imap <C-x>   <Cmd>call codeium#Clear()<CR>
"
" use space to esaced completion context window
" then use tab to accept complete
"inoremap <script><silent><nowait><expr> <space>call codeium#CycleOrComplete()<CR>


" Use `[g` and `]g` to navigate diagnostics
" Use `:CocDiagnostics` to get all diagnostics of current buffer in location list.
nmap <silent> [g <Plug>(coc-diagnostic-prev)
nmap <silent> ]g <Plug>(coc-diagnostic-next)

" GoTo code navigation.
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)
nmap <silent> <F2> <Plug>(coc-rename)

" no need for clipboard from linux to windows 
" see "WSL yank support" the post copy action in this file
" vmap <C-c> "+y 
vmap <C-c> y

nnoremap <C-h> :call CocAction('codeAction')<CR>

" hover show detail or 'K'
nnoremap <silent> K :call ShowDocumentation()<CR>

function! ShowDocumentation()
  if CocAction('hasProvider', 'hover')
    call CocActionAsync('doHover')
  else
    call feedkeys('K', 'in')
  endif
endfunction


" Neartree
nmap <F3> :NERDTreeMirror<CR>
nmap <F3> :NERDTreeToggle<CR>


nmap <Space>f :Files<CR>
nmap <silent> <Space>b :Buffers<CR>
nmap <F1> :Commands<CR>

" Function to copy the current file name to the clipboard using xclip
function! CopyFileNameToClipboard()
    let filename = expand('%:p')  " Get the current file name/abs
	call system(s:clip, filename)
    echo 'Copied "' . filename . '" to clipboard'
endfunction

" Map Shift+Alt+C to the function
nnoremap <S-A-C> :call CopyFileNameToClipboard()<CR>



function ToggleFloatTerm()
	:w
	:FloatermToggle  
endfunction

" when open floatterm run in vim (useless)
"autocmd User FloatermOpen  tmux

function WriteAndFormat()
	call CocAction('format')
	:w
endfunction
noremap <C-s> :call WriteAndFormat()<CR>

noremap <C-t> :call ToggleFloatTerm()<CR>
tnoremap   <silent>   <C-t>   <C-\><C-n>:FloatermToggle<CR>
tnoremap   <silent>   <C-w>   <C-\><C-n>:FloatermKill<CR>
tnoremap   <silent>   <C-A-C>   <C-\><C-n>:FloatermNew<CR>
"nnoremap   <silent>   <F8>    :FloatermPrev<CR>
tnoremap   <silent>   <C-A-k>    <C-\><C-n>:FloatermPrev<CR>
"nnoremap   <silent>   <F9>    :FloatermNext<CR>
tnoremap   <silent>   <C-A-j>    <C-\><C-n>:FloatermNext<CR>
"nnoremap   <silent>   <F12>   :FloatermToggle<CR>
"tnoremap   <silent>   <F12>   <C-\><C-n>:FloatermToggle<CR>


" Highlight the symbol and its references when holding the cursor
autocmd CursorHold * silent call CocActionAsync('highlight')

" Mappings for CoCList
" Show all diagnostics.
nnoremap <silent><nowait> <space>a  :<C-u>CocList diagnostics<cr>
" Manage extensions.
nnoremap <silent><nowait> <space>e  :<C-u>CocList extensions<cr>
" Show commands.
nnoremap <silent><nowait> <space>c  :<C-u>CocList commands<cr>
" Find symbol of current document.
nnoremap <silent><nowait> <space>o  :<C-u>CocList outline<cr>
" Search workspace symbols.
nnoremap <silent><nowait> <space>s  :<C-u>CocList -I symbols<cr>
" Do default action for next item.
nnoremap <silent><nowait> <space>j  :<C-u>CocNext<CR>
" Do default action for previous item.
nnoremap <silent><nowait> <space>k  :<C-u>CocPrev<CR>
" Resume latest coc list.
nnoremap <silent><nowait> <space>p  :<C-u>CocListResume<CR>


nnoremap <silent><nowait> <space>l0  :set foldlevel=0<CR>
nnoremap <silent><nowait> <space>l1  :set foldlevel=1<CR>
nnoremap <silent><nowait> <space>l2  :set foldlevel=2<CR>
nnoremap <silent><nowait> <space>l3  :set foldlevel=3<CR>
nnoremap <silent><nowait> <space>l4  :set foldlevel=4<CR>
nnoremap <silent><nowait> <space>l5  :set foldlevel=5<CR>
nnoremap <silent><nowait> <space>l6  :set foldlevel=6<CR>


" redirect the [Codeium] keybinds
"let g:codeium_disable_bindings = 1
"imap <script><silent><nowait><expr> <C-g> codeium#Accept()
"imap <C-;>   <Cmd>call codeium#CycleCompletions(1)<CR>
"imap <C-,>   <Cmd>call codeium#CycleCompletions(-1)<CR>
"imap <C-x>   <Cmd>call codeium#Clear()<CR>

		

" Status Line Color
"hi User1 cterm=none ctermfg=27 ctermbg=0 
"hi User2 cterm=none ctermfg=208 ctermbg=0
"hi User3 cterm=none ctermfg=169 ctermbg=0
"hi User4 cterm=none ctermfg=100 ctermbg=0
"hi User5 cterm=none ctermfg=green ctermbg=0

if has("autocmd")
  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif


" cpp highlight
let g:cpp_class_scope_highlight=1
let g:cpp_variable_highlight=1
let g:cpp_class_decl_highlight=1
let g:cpp_posix_standard = 1
let g:cpp_experimental_simple_template_highlight = 1
let g:cpp_concepts_highlight = 1

"let g:codeium_disable



" float term config
let g:floaterm_height=0.8 
let g:floaterm_with=0.9
"let g:floaterm_wintype=float 
let g:floaterm_autoclose=2
if has("win32")
	let g:floaterm_shell='pwsh'
endif


" WSL yank support
let s:clip = '/mnt/c/Windows/System32/clip.exe'  " change this path according to your mount point
if executable(s:clip)
    augroup WSLYank
        autocmd!
        autocmd TextYankPost * if v:event.operator ==# 'y' | call system(s:clip, @0) | endif
    augroup END
endif
" codeium


