# libultrahdr wasm

WASM version of the [libultrahdr](https://github.com/google/libultrahdr) library.

This library is primarily intended to be used in conjunction with the [gainmap-js](https://github.com/MONOGRID/gainmap-js) library which is, in turn, focused on providing an alternative lightweight HDR format for WebGL 3D Engines like (three.js) 

This library is provided "as-is" and not tested nor optimized for general use.

## Format Introduction
https://developer.android.com/guide/topics/media/platform/hdr-image-format

## WASM Bindings

At the time of writing (2023-10-20): The only exposed function is `appendGainMap` which can be used like this:

```js
import libultrahdr from './build/libultrahdr-esm.js'
const libraryInstance = await libultrahdr()

// given:
// # width: width of the final image 
// # height: height of the final image 
// # sdr: Uint8Array containing the SDR rendition encoded in jpeg
// # gainmap: Uint8Array containing the Gainmap encoded in jpeg
// # metadata: an object containing the parameters of the gainmap

const result = libraryInstance.appendGainMap(
    width, height,
    sdr, sdr.length,
    gainmap, gainmap.length,
    metadata.gainMapMax,  metadata.gainMapMin,
    metadata.mapGamma, metadata.offsetSdr, metadata.offsetHdr,
    metadata.hdrCapacityMin, metadata.hdrCapacityMax
)

// result in an Uint8Array of a jpeg file (sdr rendition) with the gainMap "appended" (XMP metadata + MPF Binary gainmap)
```

further investigation is needed in order to integrate more bindings, especially given the library contains `libjpeg-turbo` which is completely unused at the moment.

It has the potential to handle the complete encoding of a gainmap jpeg given raw YUV440 HDR Data (as per the upstream repository intended usage).

## Testing Locally

After building, start a server with `python -m http.server` in the root of the project and open `http://localhost:8000`, you will have an HTML page which can be used to load the files expected by the `appendGainMap` function.

Once all the files are provided the page will download a JPEG with appended gainmap.

## Building Requirements

* Requires **emscripten**: follow [this guide](https://emscripten.org/docs/getting_started/downloads.html) to download it and setup it.
* Requires [python 3](https://www.python.org/downloads/) 
* Requires python module **meson**: install it with `pip3 install --user meson` 
* Requires python module  **ninja**: install it with `pip3 install --user ninja`

make sure the globally installed python scripts folder is included in your OS Path.

## Building

Create a meson "cross compile config" named `em.txt` and place the following content inside:

```ini
[binaries]
c = 'emcc'
cpp = 'em++'
ar = 'emar'
nm = 'emnm'

[host_machine]
system = 'emscripten'
cpu_family = 'wasm32'
cpu = 'wasm32'
endian = 'little'
```

this assumes the path to `em++`, `emcc` etc is globally accessible (i.e. added to PATH).

on Windows you may need to specify the full path for binaries like this 

```ini
[binaries]
c = 'C:\path\to\emsdk\upstream\emscripten\emnm.bat'
cpp = 'C:\path\to\emsdk\upstream\emscripten\emnm.bat'
ar = 'C:\path\to\emsdk\upstream\emscripten\emnm.bat'
nm = 'C:\path\to\emsdk\upstream\emscripten\emnm.bat'
```

### Build commands

```bash
$ meson setup build --cross-file=em.txt
$ meson compile -C build
```

the `build` folder will contain the compiled wasm files in multiple versions

1. `libultrahdr-debug.js` is the ESM debug version with source maps enabled
2. `libultrahdr-esm.js` is the release version compiled in ESM
3. `libultrahdr.js` is the release version without ESM
