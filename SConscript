from building import *

cwd     = GetCurrentDir()

src = Split('''
main.c
esp_foc.c
esp_foc_devices.c
esp_foc_consts.c
''')

if GetDepend(['BSP_USING_ON_CHIP_FS']):
    src += ['mnt.c']


CPPPATH = [str(Dir('#')), cwd]

group = DefineGroup('Applications', src, depend = [''], CPPPATH = CPPPATH)

Return('group')