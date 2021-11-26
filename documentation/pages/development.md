<!-- omit in toc -->
# Development

<!-- omit in toc -->
## Table of Contents
- [Run Instructions](#run-instructions)
  - [Standalone Applications](#standalone-applications)
  - [Unit Tests](#unit-tests)
  - [Clang-format and CMake formatting](#clang-format-and-cmake-formatting)
    - [Install dependencies](#install-dependencies)
    - [Build and run](#build-and-run)
  - [Documentation](#documentation)
    - [Install dependencies](#install-dependencies-1)
    - [Build and open documentation:](#build-and-open-documentation)
  - [Build all](#build-all)
- [Other Development Tools](#other-development-tools)
  - [Code quality](#code-quality)
    - [Sanitizers](#sanitizers)
    - [Static analyzers and linters](#static-analyzers-and-linters)
      - [Notes](#notes)
      - [Static analyzer installation](#static-analyzer-installation)
        - [clang-tidy](#clang-tidy)
        - [Include What You Use](#include-what-you-use)
        - [cppcheck](#cppcheck)
  - [Ccache](#ccache)
  - [Package Manager](#package-manager)
  - [Project Template](#project-template)

## Run Instructions

### Standalone Applications

Use the following commands to build and run standalone applications.

```bash
# Configure the build
cmake -S standalone -B build/standalone

# Build
cmake --build build/standalone -j4

# Run one of the sample apps
./build/standalone/${SAMPLE_EXECUTABLE_NAME} --help
```

### Unit Tests

We use [doctest](https://github.com/onqtam/doctest) as a testing framework. To build and run the test suite, use the following commands:

```bash
# Build
cmake -S test -B build/test
cmake --build build/test -j4

# Run
./build/test/path_planning_tests
```

### Clang-format and CMake formatting

We use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) for formatting C/C++ files, and [cmake-format](https://github.com/cheshirekow/cmake_format) for formatting CMake files. See settings in the `.clang-format` and `.cmake-format` files, respectively.

Some IDEs support formatting on save using the .clang-format settings. For example, with Visual Studio Code see the [formatOnSave](https://code.visualstudio.com/docs/cpp/cpp-ide#_code-formatting) settings.

This section is only necessary to run clang-format, or format CMake files, without using your IDE's built-in functionality.

#### Install dependencies

- PyYaml: `pip install PyYAML`
- cmake-format: `pip install cmakelang`
- clang-format
  - Ubuntu: `sudo apt install clang-format`
  - MacOS: `brew install clang-format`

#### Build and run

```bash
cmake -Htest -Bbuild/test

# preview changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

If no changes need to be made then you will see `clang-format did not modify any files`. Otherwise, the process will output a git diff.

### Documentation

We use [doxygen](https://www.doxygen.nl/index.html) for code documentation rendering to HTML.

#### Install dependencies

- jinga2: `pip install jinja2`
- pygments: `pip install pygments`
- doxygen
  - Ubuntu: `sudo apt-get install doxygen`
  - MacOS: `brew install doxygen`

#### Build and open documentation:

```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target GenerateDocs
```

To view the docs, open `build/doc/doxygen/html/index.html` in your web browser

### Build all

The project also includes an `all` directory that allows building all targets at the same time. This is useful during development, as it exposes all sub-projects to your IDE and eliminates redundant builds of the library.

```bash
cmake -S all -B build
cmake --build build -j4
```

After building, you can use the targets from the preceding sections, but some of them appear under a different build path. The differences are noted below:

```bash
# Code formatting
cmake --build build --target fix-format

# Building documentation
cmake --build build --target GenerateDocs
```

## Other Development Tools

### Code quality

`cmake/tools.cmake` uses [cmake-scripts](https://github.com/StableCoder/cmake-scripts) to import code quality tools through CMake configuration arguments.

#### Sanitizers

Sanitizers check for problems, such as memory leaks, at runtime. They will output any detected problems to the console.

Sanitizers are enabled by passing the following CMake option:

```
-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>
```

A description of each sanitizer can be found [here](https://github.com/StableCoder/cmake-scripts#sanitizer-builds-sanitizerscmake).

For example, to build with the `Address;Undefined` sanitizer, do the following:

```bash
cmake -S all -B build -DUSE_SANITIZER='Address;Undefined'
cmake --build build -j4
```

#### Static analyzers and linters

Static analyzers detect potential issues by inspecting source code. Static analyzers can be enabled by passing the following CMake option:

```
-DUSE_STATIC_ANALYZER=<clang-tidy | iwyu | cppcheck>
```

You will need to install the static analyzers first before using them, as shown [below](#static-analyzer-installation).

To use multiple static analyzers, write more than one of the options, separated by semicolons, in quotation marks.

A description of each sanitizer can be found [here](https://github.com/StableCoder/cmake-scripts#tools-toolscmake).

For example, to build with `clang-tidy` and `iwyu`, do the following:

```bash
cmake -S all -B build -DUSE_STATIC_ANALYZER='clang-tidy;iwyu'
cmake --build build -j4
```

By default, analyzers will automatically find configuration files such as `.clang-format`.

Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS` or `CPPCHECK_ARGS` variables. For example:

```bash
cmake -S all -B build -DUSE_STATIC_ANALYZER='clang-tidy' -DCLANG_TIDY_ARGS='-checks=*'
cmake --build build -j4
```

##### Notes
- Static analyzers will not check files that are not being recompiled. You may wish to delete the `build/` directory to clean the build.
- `clang-tidy` will not output anything unless some of its checks are turned on using `CLANG_TIDY_ARGS`.

##### Static analyzer installation

###### clang-tidy
- Ubuntu: `sudo apt install clang-tidy`
- MacOS ([reference](https://stackoverflow.com/questions/53111082/how-to-install-clang-tidy-on-macos))
  ```bash
  brew install llvm
  ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy"
  ```

###### Include What You Use
- Ubuntu: `sudo apt install iwyu`
- MacOS: `brew install include-what-you-use`

###### cppcheck
- Ubuntu: `sudo apt install cppcheck`
- MacOS: `brew install cppcheck`

### Ccache

[Ccache](https://ccache.dev/) is a cache for compiled files. It can be enabled by running CMake with `-DUSE_CCACHE=<ON | OFF>`.

### Package Manager

This project is using [CPM](https://github.com/TheLartians/CPM.cmake) as a package manager. See their documentation for usage instructions.

**Tip:** To avoid redundant downloads of packages, set a CPM.cmake cache directory in your shell startup file. For example:

```bash
export CPM_SOURCE_CACHE=$HOME/.cache/CPM
```

### Project Template

This project was bootstrapped using the [ModernCppStarter](https://github.com/TheLartians/ModernCppStarter) template.

Notable changes applied to the template:

- Removed codecov code coverage
- Removed Github Actions continuous integration
- Made some changes to the `.clang-format` file to match our existing code style:
  - ColumnLimit: '120'
  - AlwaysBreakTemplateDeclarations: 'true'
  - NamespaceIndentation: None
  - SortIncludes: 'false' (added this one)

