function execute_commands(commands)
    print("Executing...")

    local command_list = {}

    if type(commands) == "string" then
        -- Remove leading and trailing whitespaces
        -- commands = commands:gsub("^%s*(.-)%s*$", "%1")
        
        -- Split the string into lines using line breaks
        for line in commands:gmatch("[^\r\n]+") do
            table.insert(command_list, line)
        end
    elseif type(commands) == "table" then
        -- Assume the table contains the commands
        command_list = commands
    else
        print("Invalid input type for 'commands'")
        return
    end

    print(command_list)

    for _, cmd in ipairs(command_list) do
        print(cmd)
        print( os.exec(cmd))
    end
end
