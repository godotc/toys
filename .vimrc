call plug#begin()

"Plug 'ycm-core/YouCompleteMe'
Plug 'neoclide/coc.nvim', {'do': 'yarn install --frozen-lockfile'}
Plug 'scrooloose/nerdtree'  "目录树
Plug 'tpope/vim-surround'
Plug 'jiangmiao/auto-pairs' "括号配对
Plug 'vim-scripts/a.vim'

Plug 'octol/vim-cpp-enhanced-highlight'

Plug 'itchyny/lightline.vim' "status line
Plug 'frazrepo/vim-rainbow' "彩虹括号



" Theme
Plug 'joshdick/onedark.vim'
Plug 'rakr/vim-one'

" All of your Plugins must be added before the following line
call plug#end()            " required



set termguicolors
"tmux Error color setting
" Enable true color 启用终端24位色
if exists('+termguicolors')
   let &t_8f = "\<Esc>[38;2;%lu;%lu;%lum"
   let &t_8b = "\<Esc>[48;2;%lu;%lu;%lum"
   set termguicolors
endif



set laststatus=2
let g:lightline = {
	  \ 'colorscheme': 'onedark',
	  \ }
let g:airline_theme='onedark'




let g:rainbow_active=1


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

filetype indent on

" Command Line View
set wildmode=list:full 
"set wildmenu

set background=dark " for the dark version
" set background=light " for the light version
colorscheme onedark


" Make <TAB> to accept selected completion item or notify coc.nvim to format
" <C-g>u breaks current undo, please make your own choice.
 inoremap <silent><expr> <TAB>  coc#pum#visible() ? coc#pum#confirm():"\<TAB>"


" Use `[g` and `]g` to navigate diagnostics
" Use `:CocDiagnostics` to get all diagnostics of current buffer in location list.
nmap <silent> [g <Plug>(coc-diagnostic-prev)
nmap <silent> ]g <Plug>(coc-diagnostic-next)

" GoTo code navigation.
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)
nmap <leader> rn <Plug>(coc-rename)


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
map <F3> :NERDTreeMirror<CR>
map <F3> :NERDTreeToggle<CR>
noremap <C-d> :call SavedAndOpenTerm()<CR>
noremap <C-s> :call SaveAndFormat()<CR>
noremap <C-h> <Plug>(coc-fix-current)

def SavedAndOpenTerm()
	:w
	:bo term
enddef

function SaveAndFormat()
	call CocAction('format')
	:w
endfunction


" Highlight the symbol and its references when holding the cursor.
" autocmd CursorHold * silent call CocActionAsync('highlight')

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

