import platform
import os
import inspect

# System specific directory separator
if platform.system() in ["Darwin", "Linux"]:
    SEP = "/"
else: # platform.system() == "Windows":
    SEP = "\\"


DIR_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))) + SEP
DIR_OUT = DIR_ROOT + "out" + SEP
DIR_OUT_IM = DIR_ROOT + SEP + "images"