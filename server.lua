

local socket = require("socket")
local m3dp = require("libmoon3dp")

-- poll loop
local printer = nil

while printer == nil do
    print("[host]: trying to find ..")
    printer = m3dp.find_creality_printer()
    socket.sleep(1)   -- sleep without moonusb
end

print("[host]: found 3d printer")


print("[host]:starting server...")

-- SEND <gcode>
local function handle_SEND(gcode)
    if not printer then
        return "ERR no printer\n"
    end

    -- Write G-code to printer (USB bulk OUT)
    local ok, err = printer:bulk_write(0x01, gcode .. "\n", 1000)
    if not ok then
        return "ERR write failed: " .. tostring(err) .. "\n"
    end

    return "OK\n"
end

-- SYSC <system command>
local function handle_SYSC(cmd)
    local f = io.popen(cmd .. " 2>&1")
    local out = f:read("*a")
    f:close()
    return out .. "\n"
end

---------------------------------------------------------
-- Command parser
---------------------------------------------------------

local function process(line)
    local cmd, rest = line:match("^(%S+)%s*(.*)$")
    if not cmd then
        return "ERR invalid\n"
    end

    if cmd == "SEND" then
        return handle_SEND(rest)

    elseif cmd == "SYSC" then
        return handle_SYSC(rest)

    else
        return "ERR unknown\n"
    end
end

---------------------------------------------------------
-- TCP Server
---------------------------------------------------------

local server = assert(socket.bind("*", 9000))
print("[server] listening on port 9000")

while true do
    local client = server:accept()
    client:settimeout(0.5)
    print("[server] client connected")

    while true do
        local line, err = client:receive("*l")
        if not line then
            if err ~= "timeout" then break end
        else
            local reply = process(line)
            client:send(reply)
        end
    end

    print("[server] client disconnected")
    client:close()
end