import awtk_locator as locator

def Helper(ARGUMENTS):
    locator.init(ARGUMENTS)

    from app_helper_base import AppHelperBase
    from app_helper_base import COMPILE_CONFIG
    
    COMPILE_CONFIG['BUILD_TESTS'] = { 'value' : True, 'type' : bool.__name__, 'desc' : ['build awtk-modbus\'s gtest demo'], 'help_info' : 'build awtk-modbus\'s gtest demo, value is true or false, default value is true' }
    COMPILE_CONFIG['BUILD_DEMOS'] = { 'value' : True, 'type' : bool.__name__, 'desc' : ['build awtk-modbus\'s demo examples'], 'help_info' : 'build awtk-modbus\'s demo examples, value is true or false, default value is true' }

    return AppHelperBase(ARGUMENTS)