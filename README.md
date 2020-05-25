# Sinsy

![C/C++ CI](https://github.com/r9y9/sinsy/workflows/C/C++%20CI/badge.svg)

A fork of sinsy: http://sinsy.sourceforge.net/

## Why?

Wanted to fork it with git.

To preserve the commit history, the repository was initialized by the following command:

```
git svn clone https://svn.code.sf.net/p/sinsy/code-0/ --no-metadata --authors-file=authors.txt sinsy
```

Subsequent improvements will be done in the git repository as needed.

See [src/README](src/README) for the original README.

## Requirements

- https://github.com/r9y9/hts_engine_API

## Installation

```
cd src
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON  ..
make -j
sudo make install
```

See the complete setup in https://github.com/r9y9/sinsy/blob/master/.github/workflows/ccpp.yaml.


## Changes

- Fixes for recent C++ compilers
- CMake support
- CI using Github actions

## References

- Sinsy website: http://www.sinsy.jp/
- Sinsy official source code: http://sinsy.sourceforge.net/
