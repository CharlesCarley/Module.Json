# Json

Is a basic C++ json loader.

## Testing

The testing directory is setup to work with [googletest](https://github.com/google/googletest). As well as the initial setup for testing the standalone module using GitHub actions.

## Building

Building with CMake and Make

```sh
mkdir build
cd build
cmake ..
make
```

Optional defines.

| Option                 | Description                         | Default |
|:-----------------------|:------------------------------------|:-------:|
| Json_BUILD_TEST    | Build the unit test program.        |   OFF   |
| Json_AUTO_RUN_TEST | Automatically run the test program. |   OFF   |
