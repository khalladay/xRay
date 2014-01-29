#xRay

An extensible and customizable ray tracing renderer.

### Philosophy

xRay first and foremost is intended to be a high quality renderer in its own right. More importantly however, the intention is to structure this project in a way that is highly extensible and customizable, in order to provide a base from which to build more stylized or custom renderers for individual projects.

###Usage

Currently there is no GUI front end for xRay. The project will build a command line tool. To use this tool:

> _xRay [path of input collada file] [path to output image]_

The project has only been tested with collada files generated from Blender 2.69. All meshes must be triangulated.

###Current Version: 0.01
* Blender COLLADA file parsing
* Support for Directional lights only
* Support for Lambert Materials only

![v0.01 output](http://kylehalladay.com/images/xray/xray_v0.01.png)


###Dependencies
* Boost Multi_Array
* Boost Threads
* GLM (Included in Repo)
* RapidXML (Included in Repo)


