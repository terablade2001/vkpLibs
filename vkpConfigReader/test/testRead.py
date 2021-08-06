from vkpConfigReader import *

dict = vkpConfigRead("DefaultConfig.cfg",1)

print("Floats: [%s]"%(dict["Floats"]))
