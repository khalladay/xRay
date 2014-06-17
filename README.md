#xRay

A toy ray tracer. 

###Recent Changes:
* Blender COLLADA file parsing
* Support for Directional lights only
* Support for Lambert Materials only

![v0.01 output](http://kylehalladay.com/images/xray/xray_v0.01.png)


###Usage - CLI

Currently there is no GUI front end for xRay. The project will build a command line tool. To use this tool:

> _xRay [path of input collada file] [path to output image]_

The project has only been tested with collada files generated from Blender 2.69. All meshes must be triangulated.

###Build and Run
Building and running from xCode will pass the command line arguments automatically. To change the default paths (which are unique to my system at the moment), go to the arguments panel in the active scheme. 

###Dependencies
* Boost Multi_Array
* Boost Threads
* GLM (Included in Repo)
* RapidXML (Included in Repo)


