local socket = require("socket")

local HOST = "12.0.0.1"
local PORT =  "9000"

local function connect()
    local tcp = assert(socket.tcp())
    tcp:settimeout(5)
    assert(tcp:connect(HOST, PORT))
    tcp:settimeout(1)
    return tcp
end

local function send_gcode(tcp, line)
    tcp:send(line .. "\n")
    local reply, err = tcp:receive("*l")
    return reply, err
end

local function stream_file(tcp, path)
    local f = assert(io.open(path, "r"))
    print("Connected to printlink host")

    for line in f:lines() do
        if line:match("%S") and not line:match("^;") then
            print("SEND:", line)

            local reply, err = send_gcode(tcp, line)
            if not reply then
                print("ERR:", err)
                break
            end

            print("REPLY:", reply)
        end
    end

    f:close()
end


print("host ip:")
HOST = io.read("l")


local file = arg[1]

local tcp = connect()

if not file then
    print("printlink gcode shell v0.1.0")
    print("Type 'exit' to quit")

    while true do
        io.write("> ")
        local input = io.read("*l")

        if input == "exit" then
            break
        end

        local reply, err = send_gcode(tcp, input)
        if not reply then
            print("ERR:", err)
        else
            print("printer> " .. reply)
        end
    end
else
    -- file streaming mode
    stream_file(tcp, file)
end

tcp:close()
