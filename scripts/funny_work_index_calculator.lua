local prompt = "> "
local function read_number()
    repeat
        io.write(prompt)
        local line = io.read("n")
        return line, true
    until true
end

local function what(text)
    print(text)
    local result = read_number()
    print("================================")
    return result
end

local function what_opt(text, options)
    print(text)

    local kv = {}
    if options then
        local i = 1
        for k, v in pairs(options) do
            print(i .. ". " .. k .. " >>  " .. v)
            kv[i] = v
            i = i + 1
        end
    end

    local index = read_number()
    local result = kv[index]
    print(result)
    print("================================")
    return result
end

salary = what("How much is your salary?")

local work_env =
    nil or
    what_opt(
        "How is your work environment?(number)",
        {
            ["normal"] = 1.0,
            ["remote surbubs/factory"] = 0.8,
            ["closer surbubs/factory"] = 0.9,
            ["cdb or offical system"] = 1.1
        }
    )
local sex_env =
    nil or what_opt("How many nice man/women around your?(number)", {["normal"] = 1.0, ["less"] = 0.9, ["more"] = 1.1})
local colleagues_env =
    nil or
    what_opt(
        "How many  good colleagues around you?(number)",
        {
            ["normal"] = 1.0,
            ["more bad colleagues"] = 0.95,
            ["more good colleagues"] = 1.05
        }
    )

integrated_env_index = work_env * sex_env * colleagues_env

local work_times = nil or what("How much time you work per day?(in hours)")
local commuting_times =
    nil or what("How much time you take on the way from home/work to work/home  every day? (in hours)")
local touchfish_times =
    nil or what("How long your could spend for yourself?(in hours, within the time for eating and noon rest)")
local eaducation_index =
    nil or
    what_opt(
        "how about your education?",
        {
            ["below beachlor"] = 0.8,
            ["nomral bechlor"] = 1.0,
            ["985/211 beachlor"] = 1.2,
            ["nomral master"] = 1.4,
            ["985/211 master"] = 1.6,
            ["nomral doctor"] = 1.8,
            ["985/211 doctor"] = 2.0
        }
    )

work_index = 35 * (work_times + commuting_times - touchfish_times) * eaducation_index

print("your salary is: " .. salary)
print("your integrated_env_index is: " .. integrated_env_index)
print("your work index is: " .. work_index)
oxen_horses_index = salary * integrated_env_index / work_index

print("your oxen_horses_index is" .. oxen_horses_index)
