


argv = argv
silverchain = silverchain

local function single()

  dtw.remove_any("CerradoSync.h")
  local max_content = camalgamator.ONE_MB * 10
  local max_recursion = 100
  local result = camalgamator.generate_amalgamation("src/imports/imports.def.h", max_content, max_recursion)
  dtw.write_file("CerradoSync.h", result)
end


local function imports()

  silverchain.generate({
      src = "src",
      project_short_cut = "CerradoSync",
      tags = { "inp", "dep",  "mac", "types", "const", "globals", "dec", "def" },
      implement_main=true,
      main_name="main.c"
  })

end

local function is_insid(array, element)

  for i=1, #array do
    if element == array[i] then
      return true
    end
  end

  return false

end

local function args()

  if argv.flags_exist({ "help", "hp", "h"}) then
    print("\n\n\t'function', 'func', 'f'\n\t'single', 'imports'\n\n")
    return
  end

  local used = false

  local out_flags, size = argv.get_flag_args({ "function", "func", "f" })

  if is_insid(out_flags, "imports") then
    imports()
    print("\n\timports called")
    used = true
  end

  if is_insid(out_flags, "single") then
    single()
    print("\n\tsingle called")
    used = true
  end

  if not used then
    print("\n\tnot functions calleds")
  end

  print("\n\tend\n")
end

function main()

  args()

end






