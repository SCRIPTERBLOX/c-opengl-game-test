# Combined Triangle and Cube Rendering

This minimal project renders both a static triangle and a spinning cube in the same window using OpenGL ES 2.0 and Wayland.

## Features

- **Left side**: Static red triangle
- **Right side**: Spinning colorful cube
- Uses the common windowing and EGL handling from the `../common/` directory
- Minimal self-contained implementation

## Building

```bash
make
```

## Running

```bash
./combined
```

## Dependencies

- Wayland client libraries
- EGL
- OpenGL ES 2.0
- Wayland cursor library

## Controls

- Close the window to exit
- Ctrl+C to interrupt