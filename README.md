# MC-Cartographer
A console based Minecraft map item data converter written in c++.

## Usage
### Converting a map into an image
1.  Open NBTExplorer and go to savegame > data > map_x.dat.
2.  Double click on 'colours' and export to a suitable location.

**Using drag and drop**
3.  Drag and drop the map file into cartographer.exe.
4.  A PNG file should be created in the same directory as the map file.

**Using console**
3.  Open cartographer.exe and enter the full path to the map (e.g. C://directory/map).
4.  A PNG file should be created in the same folder as cartographer.exe.

### Converting an image into a map
**Using drag and drop**
1.  Drag and drop the desired image into cartographer.exe.
2.  A file should be created in the same directory as the image title imagename_map.

**Using console**
1.  Open cartographer.exe and enter the full path to the image (e.g. C://directory/image.png).
2.  A PNG file should be created in the same directory as the map titled imagename_map.

3.  Open NBTExplorer and got to savegame > data > map_x.dat.
4.  Double click on 'colours' and import imagename_map.
