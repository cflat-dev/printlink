local usb = require("moonusb")
m3dp = {}

m3dp.ctx = usb.init()

-- returns the printers reseponce as a string , each newelikee in src is a new line tthis func that send whole gcode files (marlin impl)
function m3dp.send_gcode(dev, src)
    local out = {}

    -- split src into lines
    for line in src:gmatch("[^\r\n]+") do
        -- send line with newline
        dev:write(line .. "\n")

        -- read response until we get "ok" or an error
        while true do
            local resp = dev:read("*l")   -- read one line
            if not resp then break end    -- device closed or timeout
            table.insert(out, resp)

            -- Marlin ends each command with "ok"
            if resp:match("^ok") or resp:match("^Error") then
                break
            end
        end
    end

    return table.concat(out, "\n")
end

function m3dp.find_creality_printer()
    local usb = require("moonusb")

    -- ensure context exists
    if not m3dp.ctx then
        m3dp.ctx = usb.new_context()
    end

    -- Known Creality USB IDs
    local known = {
        ["1A86:7523"] = true,  -- CH340/CH341
        ["0483:5740"] = true,  -- STM32 CDC ACM
        ["2E3C:5740"] = true,  -- GD32 clone
    }

    -- get device list from context
    local devices = m3dp.ctx:get_device_list()
    local found = nil

    for _, dev in ipairs(devices) do
        local descr = dev:get_device_descriptor()
        local id = string.format("%04X:%04X", descr.vendor_id, descr.product_id)

        if known[id] then
            -- open USB device
            local handle = dev:open()

            -- open serial interface at Marlin default baudrate
            local serial = handle:open_serial(115200)

            dev:free()
            return serial
        end

        dev:free()
    end

    return nil
end





return m3dp