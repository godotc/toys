task("test")
do
	set_menu {}
	on_run(function()
		import("core.project.project")
		for targetname, target in pairs(project.targets()) do
			print("==================")
			print(targetname)
			print(target:name())
			print(target:kind())
			-- print(target)
			print(target:targetfile())
			print(target:basename())
			print(target:filename())
			print(target:linkname())
			print(target:targetdir())
			-- return

			-- local target_name = arg[1] or "__DEFAULT_VAR__"
			-- local target_dir = arg[2] or "__DEFAULT_VAR__"
			-- local target_base_name = arg[3] or "__DEFAULT_VAr__"
		end

		-- exec_cmds(
		--     "xmake f -m debug --test=y",
		--     -- "xmake f -m debug",
		--     "xmake build -g test",
		--     "xmake run -g test"
		-- )
	end)
end

task("cpcm")
do
	set_menu {
		usage = "xmake cpcm",
		options = {
			{ nil, "rule", "v", "debug", "the rule to config build mode " }
		}
	}
	on_run(function()
		local cmd     = import("cmd") -- works
		local opt     = import("core.base.option")
		-- print(cmd)

		local profile = opt.get("rule")
		cmd.exec_cmds(
			"xmake f -c",
			string.format("xmake f -m %s ", profile), --toolchain=llvm",
			"xmake project -k compile_commands"
		)
	end)
end

task("targets")
do
	set_menu {}
	on_run(function()
		for targetname, target in pairs(project.targets()) do
			print(target:targetfile())
		end
	end)
end
