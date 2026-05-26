import os
import scripts.app_helper as app

helper = app.Helper(ARGUMENTS)
helper.add_libs(['modbus'])
helper.set_dll_def('src/modbus.def').call(DefaultEnvironment)

SConsFiles = ['src/SConscript']

if helper.get_curr_config().get_value('BUILD_DEMOS', True) :
  SConsFiles += ['demos/SConscript']

if helper.get_curr_config().get_value('BUILD_TESTS', True) :
  SConsFiles += ['tests/SConscript']

SConscript(SConsFiles)
