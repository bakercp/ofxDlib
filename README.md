_This is under development currently so, your please post questions to the [issues](https://github.com/bakercp/ofxDlib/issues) for now._

# ofxDlib


## Description

An [openFrameworks](http://openframeworks.cc) wrapper for [dlib](http://dlib.net/).

## Features

-   Toll-free `openFrameworks` ↔ `dlib` bridges using `dlib` `dlib/generic_image.h` interface.
-   `openFrameworks` ↔ `dlib` type conversions.
-   `dlib` helper functions.

## Getting Started

### TL;DR
-   From the `scripts/` directory ...
    -   `./boostrap.sh`
-   From the `example_dlib_3d_point_cloud/` directory ...
    -   `make && make run`

For more, see [docs/GETTING_STARTED.md](docs/GETTING_STARTED.md).

## Troubleshooting

For more, see [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md).

## Documentation

API documentation can be found here.

## Build Status

Linux, macOS [![Build Status](https://travis-ci.org/bakercp/ofxDlib.svg?branch=master)](https://travis-ci.org/bakercp/ofxDlib)

Visual Studio, MSYS [![Build status](https://ci.appveyor.com/api/projects/status/4rkeucq1y07qc3gr/branch/master?svg=true)](https://ci.appveyor.com/project/bakercp/ofxdlib/branch/master)


## Compatibility

**Currently NOT Compatible with < 0.10.0, thus we have no stable branch as noted below.**
~~The `stable` branch of this repository is meant to be compatible with the openFrameworks [stable branch](https://github.com/openframeworks/openFrameworks/tree/stable), which corresponds to the latest official openFrameworks release.~~

The `master` branch of this repository is meant to be compatible with the openFrameworks [master branch](https://github.com/openframeworks/openFrameworks/tree/master).

Some past openFrameworks releases are supported via tagged versions, but only `stable` and `master` branches are actively supported.

## Versioning

This project uses Semantic Versioning, although strict adherence will only come into effect at version 1.0.0.

## Licensing

See [LICENSE.md](LICENSE.md).

## Contributing

Check out the [Help Wanted](https://github.com/bakercp/ofxDlib/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22) tag in the issues section for specific ideas or propose your own new ideas.

You can also take a look at the [docs/TODO.md](docs/TODO.md) for more ideas.

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1.  Fork this repository.
2.  Create your feature branch (`git checkout -b my-new-feature`).
3.  Commit your changes (`git commit -am 'Add some feature'`).
4.  Push to the branch (`git push origin my-new-feature`).
5.  Create new Pull Request.
