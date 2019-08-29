# Automated Build Versioning system

The purpose if this system is to generate and update a versioning file each time a compilation is issued. The system supports C++ and Python. The versioning is kept into a file "BuildVersion.hpp" which can be included in the projects as support file. In general the folder `BuildVersion` is a supportive project and it's files are not shared in the final product. This should not be confused with the `python/BuildVersion` library which should be shipped to the clients among with the rest of the python products.

# C++

## Auto-update versions

Before each compilation you need to call a file to update the version number. Such a file written in C++ is provided at `BuildVersion/UpdateBuildVersion-src` folder in `main.cpp`. Compiled versions of that file are also provided at `BuildVersion` folder for Windows and Ubuntu.

Thus before each compilation run the corresponding `UpdateBuildVersion` executable file and include the produced `BuildVersion.hpp` file in your project.

## Access the version via C++ code

You may include the file `C++/vkpBuildVersioner.hpp` in your project after having include the `BuildVersion.hpp` file. Then you can display the version as:

```c++
#include "../BuildVersion/BuildVersion.hpp"
#include "vkpBuildVersioner.hpp"
#include <iostream>

using namespace std;

static vkpBuildVersioner BV1(1, VERSION_NUMBER);
static vkpBuildVersioner BV2(2, VERSION_NUMBER);
static vkpBuildVersioner BV3(3, VERSION_NUMBER);

int main(int argc, char** argv) {
	cout << BV1.version << endl;
	cout << BV2.version << endl;
	cout << BV3.version << endl;
	return 0;
}
```

# Python (3)

Include the module `BuildVersion` (`python/BuildVersion`) in your python script. Then use it as shown in the `python/Test.py` script below. Notice that in Python we don't have a 'compilation' phase. Instead developer should have a flag to denote a Development/Debug phase from a Release phase. This flag is in the file `BuildVersion/current.state` and has value `1` for Debug, and `0` for Release cases. If the file does not exist then it does created with value `1`=Debug.

## Development Phases (DEBUG / RELEASE)

While the value in the file `BuildVersion/current.state` is `1` each time the python script runs the build version is increased (Debug mode).  If the value is `0` then we are in release mode and the value doesn't increase any more.

Update the version calling `BV.BuildVersion()` as below. The function is working on a common file with the C++ case which is not actually included in the release project (this file is not belonging to the shipping project, but to a supportive folder during development; eg the folder `BuildVersion` in the repository.)

Check the version using the `BV.version()` function.

```python
import BuildVersion as BV
BV.BuildVersion("../BuildVersion/BuildVersion.hpp")
print("Version: %s" %(BV.version(1)))
```

# Release AUTO-TAG feature

When there is a code ready for release, then the developer can call the non-released python file `BuildVerion/TagReleaseVersion.py`. This will enforce the versioning system to be converted to RELEASE mode (`BuildVersion/current.state = 0`), it will commit it to repo and add a new tag there.

It's up to user's preference then to push tags and commit to different remotes.

The `TagReleaseVersion.py` file should be configured at the start of the project with proper values as shown below.

- PyBuildVersionPath: Is the Relative path to the python [BuildVersion] module.
- PyFormat: Is the format that will store the tags (1,2 or 3)
- VersionFile: Is the relative path to the global Versioning file which is as C++ definition.
- CppMakes: Is a list of relative paths to all batch files that should be called in order to compile C++ relatives libraries and executables with the proper version before commit it. In general only one such batch file should call the C++ version updating system (i.e. using pre-build call) thus version to be updated correcly for all related C++ and Python compilations. If there are no C++ projects the list can be empty.

Note: If there is no Python project then `TagReleaseVersion.py` is not actually required.

```python
# Update this path to show to project's [BuildVersion] library
PyBuildVersionPath = "../python/BuildVersion/"
# Define the format of the tag.
PyFormat = 1
# Define Global Version File
VersionFile = "BuildVersion.hpp"
# Define C++ Makes
CppMakes=["../C++/make-build.cmd"]
```



## Demo Example

This repo is also a demo where you want to send to a client an executable (Example.exe) along with a python project (Test.py) marked with specific version. This demo example is written for the version 0.002 of this repo.

Clone this repo to a temporal folder to work on the demo.

First lest build the C++ example.

```shell
cd C++
cmake-build.cmd
make-build.cmd
cd build
Example.exe
# >> [0.003]
# >> [0.000.003]
# >> [0.000.000.003]
cd ../../python
python3 Test.py
# >> Version: [0.004]
cd ..
```

Thus now we have a compiled C++ executable with version [0.003] and a Test.py with version [0.004]. We want to share these to a client but also keep a common version to our git. Use:

```shell
cd BuildVersion
python3 TagReleaseVersion.py
cd ..
```

Now we have a new commit [0.005] in our repo with tag 0.005.
We have to check that the shared files (Example.exe and Test.py) have the correct version.

```shell
cd C++/build
Example.exe
# >> [0.005]
# >> [0.000.005]
# >> [0.000.000.005]
cd ../../python
python3 Test.py
# >> [0.005]
```

The released files C++ and Python are synced with git tag 0.005. SUCCESS.

Now notice that every time that you run the Test.py the version remains to [0.005]:

```shell
python3 Test.py
# >> [0.005]
python3 Test.py
# >> [0.005]
python3 Test.py
# >> [0.005]
```

This is because this is a Release version.  In order to revert to Debug version again in order to continue development we have to change the flag of the `project.state` file in the `BuildVersion` python library:

```shell
echo 1 > BuildVersion/project.state
python3 Test.py
# >> [0.006]
python3 Test.py
# >> [0.007]
```

If you try now again the `TagReleaseVersion.py` you will get a synchronized set of files at tag 0.008. 

It works! :)