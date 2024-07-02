#!/usr/bin/env lua

if #arg ~= 1 then
   io.stderr:write[[
Usage: bin2h.lua symbol_name < binary_file > output_file.h
]]
   os.exit(1)
end

assert(arg[1]:match("^[_a-zA-Z][_a-zA-Z0-9]*$"), "Invalid symbol name")

io.write("const unsigned char "..arg[1].."[] = {")
local chars_left = 0
repeat
   local b = io.read(1)
   if not b then break end
   local out = ("%i,"):format(b:byte())
   if chars_left < #out then
      io.write("\n")
      chars_left = 79 -- approximate
   end
   io.write(out)
   chars_left = chars_left - #out
until false
io.write("\n};\n")
