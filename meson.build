project('nvtt', 'c')

cpu_family = target_machine.cpu_family()

include_path = include_directories('include')

target_arch = cpu_family == 'x86_64' ? '-m64' : '-m32'

executable('nvtt', 'nvtt.c', include_directories : include_path)
