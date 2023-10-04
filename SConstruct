import os
import scripts.app_helper as app

helper = app.Helper(ARGUMENTS)
helper.add_libs(['modbus'])
helper.set_dll_def('src/modbus.def').call(DefaultEnvironment)

SConsFiles = [
    'src/SConscript',
    'demos/SConscript',
    'tests/SConscript',
]

SConscript(SConsFiles)
