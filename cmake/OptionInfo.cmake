# Takes:
# option - A CMake option
# name - A custom name for that option that describes that option
# flag - A flag that will be passed to add_definitions if the given option is active
function(printOptionInfo option name flag)
    if (${option})
        add_definitions(-D${flag})
        message("  WITH ${name} (${option} = ON)")
    else()
        message("  WITHOUT ${name} (${option} = OFF)")
    endif()
endfunction()