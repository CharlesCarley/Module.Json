# Json

Is a simple custom JSON parser that builds upon the `Module.Utils` repository.

## Current state

- [ ] Needs refactored to derive from the `Module.ParserBase` repository.
- [ ] It should still use the Visitor class interface so that smaller fragments can easily be extracted from larger json files.  

## Testing

The testing directory is setup to work with [googletest](https://github.com/google/googletest).
It contains the initial setup for testing this library as a standalone module using GitHub actions.

## Building

It builds with cmake, and it has been tested on Windows and Unix platforms using
the Visual Studio an Unix MakeFile cmake generators.

### Building with CMake and Make

```sh
mkdir build
cd build
cmake .. -DJson_BUILD_TEST=ON -DJson_AUTO_RUN_TEST=ON
make
```

### Optional defines

| Option                 | Description                                          | Default |
| :--------------------- | :--------------------------------------------------- | :-----: |
| Json_BUILD_TEST         | Build the unit test program.                         |   ON    |
| Json_AUTO_RUN_TEST      | Automatically run the test program.                  |   OFF   |
| Json_USE_STATIC_RUNTIME | Build with the MultiThreaded(Debug) runtime library. |   ON    |
