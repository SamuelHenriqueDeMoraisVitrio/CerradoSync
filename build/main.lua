



local function single()

  local max_content = camalgamator.ONE_MB * 10
  local max_recursion = 100
  result = camalgamator.generate_amalgamation("main.c", max_content, max_recursion)
  dtw.write_file("CerradoSync.h", result)
end


local function imports()

  silverchain.generate({
      src = "src",
      project_short_cut = "CerradoSync",
      tags = { "imp", "dep",  "mac", "types", "const", "globals", "dec", "def" },
      implement_main=true,
      main_name="main.c"
  })

end

local function args()

  if argv.flags_exist({ "help", "hp", "h"}) then
    print("\n\n\t'function', 'func', 'f'\n\t'single', 'imports'\n\n")
  end

  local out_flags = argv.get_flag_args({ "function", "func", "f" })

  if "single" == out_flags[0] then
    single()
    print("generate a single file")
    return
  end

  if "imports" == out_flags[0] then
    imports()
    print("config imports in src")
    return
  end

  print("Error in func params incorrect")

end

function main()

end






