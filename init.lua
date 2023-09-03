vim.cmd('source ~/.vimrc')


local function run_current_file()
	vim.cmd(':w')
	local mapping = {
		python = function() vim.cmd [[!python3 %]] end
	}
	local ft = vim.bo.filetype
	local fn = mapping[ft]
	if fn then
		fn()
	else
		print("[error] You did't config runable function for (" .. ft .. ")")
	end
end

function util_function_map(key)
	local map = {
		run = run_current_file
	}
	local fn = map[key]
	if fn then fn() end
end

vim.keymap.set('n', '<F5>', ':lua util_function_map("run")<CR>')


