# Pocket Potato - Software repository

This is the repository for the software of the **Pocket Potato Project**. For more information, see [here](https://github.com/spuschmann-tgm/PocketPotato).

## Usage

The Software, right now, only contains the logo (as the splash screen) and the main menu. When finished, it should include several games, some settings and perhaps a dev menu.

## Compilation

You'll need `arduino-cli` and `make` to compile the project.

- `make prepare`: installs needed libraries per arduino-cli
- `make` or `make all`: builds, flashes and then displays the Serial output
- `make build`: compiles the project and puts the binaries into ./build/
- `make flash`: flashes the built binaries (it likes to rebuild them first though… TODO: fix that)
- `make debug`: opens the serial port for you to look at what the board has to say
- `make clean`: cleans the build directory
- `make detect-hardware`: prints information about the detected board, if any.

## Contributing

### Textures

- Draw a texture with a pixel depth of one (either fully black or fully white pixels). I can recommend [Piskel](https://www.piskelapp.com).
- Export it as a png and feed it through `texture_fmt.py`, our texture formatting script, to get it into the format C++ wants. We currently only support non-compressed textures; compressed ones will be implemented soon!
- Add the resulting array to the code, and format it the same way the other textures are formatted. Don't forget the comment!

### Code

Ask me if any questions arise. We don't really have any hard rules around here, but that doesn't mean you can contribute bad code.
Also ask me before starting to work on your PR, to avoid unnecessary work in case of my disapproval.



## Troubleshooting

### The board isn't recognized.

Make sure the board is connected and that you have the appropriate drivers. Then try rebooting, that worked for me.
If the Makefile is detecting your board incorrectly, you can try setting the environment variable `BOARDLIST` to `"/port/path arduino:board:name"`.

### I don't have the necessary permissions. Should I flash with sudo?

<u>NO!</u> On Linux, you need to add yourself to the `uucp` group. See [here](https://wiki.archlinux.org/title/Arduino#Configuration).

### How do I compile this on Windows?

Install Linux. We accept Pull requests to support Windows, but won't make the effort to do it ourselves.

### How do I compile this on MacOS?

Our Mac expert [@MasterMarcoHD](https://github.com/MasterMarcoHD) will eventually get around to write a How-to. Until then, tough luck.
The Makefile should make this fairly easy though.

### The screen and/or buttons don't work and/or are glitchy.

Make sure your pin configuration is the same as ours. Have a look into `config.h`.

### I wanted to add a texture, but it doesn't render properly

- Make sure your texture is the right size and that you're drawing it at the right size. We're not doing any boundary checking.
- The helper script is a bit finicky at times. Open an issue on github or talk to me directly if you have any troubles.

### The games are slow and the controls are finicky

yep
