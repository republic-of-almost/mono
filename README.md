# Republic of Almost

Mono-repo for all ROA code. Individual projects are in their own directories. `app` directory contains executables projects, `lib` contains libraries, and `thirdparty` contains all external code.

[![Discored](https://img.shields.io/badge/Discord-Republic_Of_Almost-green.svg?style=flat)](https://discord.gg/DU3s4fS)

## Health Report


### Unit Tests

CI          | Status
------------|--------
Travis      | [![Build Status](https://travis-ci.org/republic-of-almost/mono.svg?branch=master)](https://travis-ci.org/republic-of-almost/mono)


## How To Build

Project assets are not included in the repo as they are subject copyright.

### macOS

  _Warning: I'm not currently using macOS much so this platform may go out of date._

  You need Xcode and Premake installed. Xcode can be downloaded from the AppStore

  You can use brew to install Premake

  ```
  brew install premake5
  ```

  Then you can build the solution.

  ```
  git clone https://github.com/republic-of-almost/mono.git
  cd mono/scripts/solution
  premake5 --file=solution.lua xcode4
  open RepublicOfAlmost.xcworkspace/
  ```

  You can then select the project to build in the top right right and click play.

### Linux

  You need Premake

  ```
  sudo apt-get install premake4
  ```

  Then you can build the solution

  ```
  git clone https://github.com/republic-of-almost/mono.git
  cd mono/scripts/solution
  premake5 --file=solution.lua gmake
  ```

  You can then type `make help` to see which project to build.

### Windows

  You need Premake you can download it from their [website](https://premake.github.io/).

  ```
  git clone https://github.com/republic-of-almost/mono.git
  cd mono/scripts/solution
  premake5 --file=solution.lua vs2015
  ```

  Open the solution, right click on project you wish to run, select 'select as startup', then press F5.

## Credits

### People

Name                  | Links
----------------------|----------------------------
Phil Cooper-King      | [Web](http://www.cooperking.net), [Linkedin](https://www.linkedin.com/in/philcooperking/), [Twitter](https://twitter.com/rep_of_a)

### Third Party Code

Name     | Website                                       | License
---------|-----------------------------------------------|--------
Catch    | https://github.com/philsquared/Catch          | MIT
GLFW     | https://github.com/glfw/glfw                  | zlib
GL3W     | https://github.com/skaslev/gl3w               | Public Domain
Json     | https://github.com/sheredom/json.h            | Public Domain
MMX      | https://github.com/vurtun/mmx                 | zlib / BSD
STB      | https://github.com/nothings/stb               | Public Domain
TinyDIR  | https://github.com/cxong/tinydir.git          | Simplified BSD


## License

Each project is covered by its own terms. If not stated then assume its reserved by the author.
