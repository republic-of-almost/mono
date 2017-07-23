# premake_data

Script to transform ROA project and test_project lua tables into premake calls.

## Useage

```
  premake4 --file=solution.lua xcode4 # macOS
  premake4 --file=solution.lua gmake # Linux
  premake5 --file=solution.lua vs2015 # Windows
```

## Dependencies

  Have premake downloaded, you can download from their website.

  - premake4 (mac)
  - premake4 (linux)
  - premake5 (win)

## Design

Data Orientated. Projects list their data only! Then the `premake_data.lua` will transform that data into premake calls.
