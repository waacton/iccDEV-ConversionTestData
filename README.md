# iccDEV-ConversionTestData

This fork of [iccDEV](https://github.com/InternationalColorConsortium/iccDEV) is used to generate ICC colour conversion data 
that is used to validate the .NET colour library [Wacton.Unicolour](https://github.com/waacton/Unicolour).

This table lists the existing code that is required and the new code that has been added:

| Code                              | Description                                                                                                                  |
|-----------------------------------|------------------------------------------------------------------------------------------------------------------------------|
| `ConvertSingle.cpp`               | A new function that takes a single set of input channels and converts them using an ICC profile                              |
| `ConvertBulk.cpp`                 | A new function that takes a CSV of input channels and converts them using an ICC profile                                     |
| `CMakeLists.txt`                  | A new CMake config file to build `ConvertSingle.exe` and `ConvertBulk.exe`                                                   |
| `Utils/`                          | A new set of helper functions used by the new conversion code                                                                |
| `IccProfLib/`                     | The existing `IccProfLib` from the original iccDev repo, all `.cpp` and `.h` files unedited                                  |
| `IccProfLib/CMakeLists.txt`       | A new CMake config file to build `IccProfLib` in a simplified way (instead of using `Build/CMake/IccProfLib/CMakeLists.txt`) |
| `IccConversionTestDataGenerator/` | A new C# solution that generates test data using `ConvertBulk.exe`                                                           |
| `.gitignore`                      | The existing .gitignore, with small additions for JetBrains IDEs                                                             |
| `README.md`                       | This file, prefixed with this information...                                                                                 |

During the [Unicolour release process](https://github.com/waacton/Unicolour/releases) this fork will pull the latest changes from the original repo and generate the latest test data.
The generated data should ideally be identical, though sometimes changes in `IccProfLib` result in very small changes (~±0.000001).
Any notable differences that trigger failures in the [Unicolour test suite](https://github.com/waacton/Unicolour/blob/main/Unicolour.Tests/IccConversionTests.cs)
should be raised in the original iccDEV repo as it's likely an issue with their reference implementation in `IccProfLib`
(and this has caught [multiple issues previously](https://github.com/InternationalColorConsortium/iccDEV/issues/113))

> [!WARNING]  
> [The author](https://github.com/waacton) is not a skilled C or C++ developer

The rest of the contents of this README are from the original repo.

---

# iccDEV

## Quickstart

| Method | Command |
|--------|---------|
| **Homebrew** | `brew install iccdev` |
| **NPM** | `npm install iccdev` |
| **Docker Pull** | `docker pull ghcr.io/internationalcolorconsortium/iccdev:latest` |
| **Docker Run** | `docker run -it ghcr.io/internationalcolorconsortium/iccdev:latest` |

To build from source, see: [Build documentation](docs/build.md)

## Introduction

The purpose of the International Color Consortium (ICC) is to promote
the use and adoption of open, vendor-neutral, cross-platform color management systems.
The International Color Consortium encourages vendors to support the ICC profile
format and the workflows required to use ICC profiles.

The iccDEV project (formerly known as DemoIccMAX) provides an
open source set of libraries and tools that allow for the interaction, manipulation,
and application of ICC based color management profiles based on the 
[ICC profile specification](http://www.color.org/icc_specs2.xalter) and the 
[iccMAX profile specification](http://www.color.org/iccmax.xalter).

All documentation is in the "docs" directory. If you're just getting started, 
here's how we recommend you read the Introduction for a list of features and 
libraries included in iccDEV.

## Contributing

Contributors are ICC members and other individual contributors who have volunteered to
maintain ICC software, documentation, or other technical artifacts. Our CONTRIBUTING
document explains our contribution processes and procedures, so please review it first:
[CONTRIBUTING](https://github.com/InternationalColorConsortium/iccDEV?tab=contributing-ov-file#contributing-to-international-color-consortium-software). Contributors are asked to sign a [Contributor License Agreement](https://github.com/InternationalColorConsortium/.github/blob/main/docs/CLA.md)

## License

iccDEV is licensed under the BSD 3-Clause “New” or “Revised” License

Membership in the ICC is encouraged when this software is used for commercial purposes.
For more information on The International Color Consortium,
please visit [www.color.org](http://www.color.org).
