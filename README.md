
## Project VR

A Virtual Reality system for visualising complex engineering CAD models

#### With the function of
- model upload (open file / directories)
- save files
- Model Options (visible, color, background color, light intensity)
- camera action (reset model camera)

#### The folder consists of
- icons (GUI icons)
- vrbindings (vr renders)

## Group Members (GROUP 7)

- Shinnachot
- Khemtis
- Shalja

## Getting Started

Install these program before started

[ValveSoftware openvr](https://github.com/ValveSoftware/openvr)

[CMake](https://cmake.org/)

[VTK](https://vtk.org/)

## Environment Variables

To run this project, you will need to add the following environment variables.

`..\VTK\bin`

`..\openvr-master\bin\win64`

## Run Locally

Clone the project

```bash
  git clone https://github.com/nathanjeerk/2023_GROUP_7
```

Go to the project directory

```bash
  cd vr
```
Run CMake to CMakeLists.txt
> **_NOTE:_**  Run with CMake Program easier which you will need to input path
```bash
  cmake .
```
Go to the build

```bash
  cd build
```

build the project

```bash
  msbuild vr.sln
```
Go to the project debug

```bash
  cd debug
```

Start the application

```bash
  ./vr.exe
```

