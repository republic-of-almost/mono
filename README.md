# Republic of Almost

Mono-repo for all ROA code. Individual projects are in their own directories. `app` directory contains executables projects, `lib` contains libraries, and `thirdparty` contains all external code.


## Health Report


### Unit Tests

CI | Status
--------|--------
Travis  | [![Build Status](https://travis-ci.org/republic-of-almost/mono.svg?branch=master)](https://travis-ci.org/republic-of-almost/mono)


## How To Build

Project assets are not included in the repo as they are subject copyright.

### macOS

  You need Xcode and Premake4 installed. Xcode can be downloaded from the AppStore

  You can use brew to install Premake

  ```
  brew install premake4
  ```

  Then you can build the solution.

  ```
  git clone rep_of_a
  cd rep_of_a/scripts/solution
  premake4 --file=solution.lua xcode4
  open RepublicOfAlmost.xcworkspace/
  ```

  You can then select the project to build in the top right right and click play.

### Linux

  You need Premake4

  ```
  sudo apt-get install premake4
  ```

  Then you can build the solution

  ```
  git clone rep_of_a
  cd rep_of_a/scripts/solution
  premake4 --file=solution.lua gmake
  ```

  You can then type `make help` to see which project to build.

### Windows

  You need Premake5 you can download it from their website.

  ```
  git clone rep_of_a
  cd rep_of_a/scripts/solution
  premake5 --file=solution.lua vs2015
  ```

  Open the solution, right click on project you wish to run, select 'select as startup', then press F5.

## Credits

### Third Party

Project  | Website                           | License
---------|-----------------------------------|--------
Catch    | https://github.com/philsquared/Catch | MIT
GLFW     | https://github.com/glfw/glfw | zlib
GL3W     | https://github.com/skaslev/gl3w | Public Domain
Imgui    | https://github.com/ocornut/imgui | MIT
Imguizmo | https://github.com/CedricGuillemet/ImGuizmo | MIT
Remotery | https://github.com/Celtoys/Remotery | Apache 2.0
STB      | https://github.com/nothings/stb | Public Domain
TinyDIR  | https://github.com/cxong/tinydir.git | Simplified BSD


## License

Each project is covered by its own terms. If not stated then assume its reserved by the author.
