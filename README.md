# MC-Cartographer
A console based application which converts images into Minecraft maps.

## Previews

| Source | Ordered | Floyd-Steinberg |
| --- | --- | --- |
| ![Hollow Knight 01](/assets/samples/source01.jpg "Source Image") | ![Hollow Knight 02](/assets/samples/ordered01.jpg "Ordered Dithering") | ![Hollow Knight 03](/assets/samples/steinberg01.jpg "Floyd-Steinberg Dithering") |

## Usage
### Prerequisites
* Download the latest version of [NBTExplorer](https://github.com/jaquadro/NBTExplorer/releases/tag/v2.8.0-win).

### Converting a map into an image
**Using drag and drop**
1.  Open ```NBTExplorer``` and go to ```savegame > data > map_x.dat > data```.
2.  Double click on ```colors``` and export to a suitable location.
3.  Drag and drop the map file into ```cartographer.exe```.
4.  A PNG file should be created in the same directory as the map file.

**Using console**
1.  Open ```NBTExplorer``` and go to ```savegame > data > map_x.dat > data```.
2.  Double click on ```colors``` and export to a suitable location.
3.  Open ```cartographer.exe``` and enter the full path to the map (e.g. C://directory/map).
4.  A PNG file should be created in the same folder as cartographer.exe.

### Converting an image into a map
**Using drag and drop**
1.  Drag and drop the desired image into ```cartographer.exe```.
2.  A file should be created in the same directory as the image file ```imagename_map```.
3.  Open ```NBTExplorer``` and got to ```savegame > data > map_x.dat > data```.
4.  Double click on ```colors``` and import ```imagename_map```.
5.  Set ```locked``` to ```1``` and ```trackingPosition``` to ```0```.
6.  Save changes.

**Using console**
1.  Open ```cartographer.exe``` and enter the full path to the image (e.g. C://directory/image.png).
2.  You will be asked to select a dithering mode. Enter ```0``` for ```ordered``` or ```1``` for ```Floyd-Steinberg```.
3.  A PNG file should be created in the same directory as the map titled ```imagename_map```.
4.  Open ```NBTExplorer``` and got to ```savegame > data > map_x.dat > data```.
5.  Double click on ```colors``` and import ```imagename_map```.
6.  Set ```locked``` to ```1``` and ```trackingPosition``` to ```0```.
7.  Save changes.
