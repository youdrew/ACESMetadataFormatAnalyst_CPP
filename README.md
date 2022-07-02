# About the project and the developer
I'm Hsuan, the project is my graduate work. This project was originally written in Python (original project [here](https://github.com/youdrew/ACES_Metadata_Format_Analyst)), but given the use of OCIO and some image processing features, I had to migrate the whole project to C++. I just learned C++ not long ago and I don't use it very well, so please pardon me if some of the code is not very efficient.

&nbsp; 

&nbsp; 

&nbsp; 


# ACES_Metadata_Format_Analyst
[ACES](https://acescentral.com/) Metadata Format Analysis tool GUI. A tool for color artist who use .amf file. It can import or batch import ACES Metadata Files, parse the text and color pipeline information in a more readable way, and display LUT information in a graphical way. It converts the information you read into various formats LUTs, OCIO Config, or Word，EXCEL, or individual CDL files...

Related Projects：

[`amf-util`](https://github.com/pomfort/amf-util)
[`amf-implementation`](https://github.com/dtatut/amf-implementation)
[`aces-dev`](https://github.com/ampas/aces-dev/tree/master)（Need build & setting）
[`Color Transformation Language`](https://github.com/ampas/CTL)
[`OpenColroIO`](https://github.com/AcademySoftwareFoundation/OpenColorIO)（Need build & setting）
[`QT6`](https://www.qt.io/)
&nbsp; 

&nbsp; 

&nbsp; 


## ACES's background knowledge
[ACES](https://acescentral.com/) is a global standard for interchanging digital image files, managing color workflows and creating masters for delivery and archiving.

It is a combination of [SMPTE](https://www.smpte.org/) standards, best practices, and sophisticated color science developed by hundreds of professional filmmakers and color scientists under the auspices of the Science and Technology Council of the [Academy of Motion Picture Arts and Sciences](https://www.oscars.org/).

[ACES](https://acescentral.com/) can be used on any type of production from features to television, commercials, AR/VR and more.
&nbsp; 

&nbsp; 

&nbsp; 



## ACES Metadata Format's background knowledge
[ACES](https://acescentral.com/) does not specify [ACES](https://acescentral.com/)‘s configuration points directly or associate them with actual imagesor shots during production, and this is the very reason why AMF exists.

AMF is the configuration file that allows a precise setup for an [ACES](https://acescentral.com/) engine. Besides this basicgoal, AMF is also the tool of choice to transmit and exchange configuration parameters in orderto ensure consistency within a workflow and across the entire ecosystem of tools that are usedwithin that workflow.

AMF is an XML specification that describes the configuration of an [ACES](https://acescentral.com/) color pipeline together with the various input transforms, look transforms and output transforms.AMF is a "sidecar' element that accompanies some visual material such as a video file or a sequence of frames or a whole composition (i.e timeline). In the case of a composition, more than one AMF file can be used if the composition requires different configurations of the [ACES](https://acescentral.com/) pipeline. It is also worth mentioning that several AMF files can reference the same visual material.

If you are an artist, you can download [**AMF Handbook**](https://community.acescentral.com/uploads/short-url/35NJKAucKujBY9g2lZjr1gNwk20.pdf).

If you are a developer, you can download [**S-2019-001**](https://community.acescentral.com/uploads/short-url/e4v6I9CuoMv5wauxqwnUkLvWjw6.pdf).

for more infomation.
&nbsp; 

&nbsp; 

&nbsp; 




## ACES Metadata Format Analyst 
ACES Metadata Format Analyst is an open source project designed for color artists. We want to create a simple AMF analysis and processing GUI tool, with lightweight, and run on Mac and Win systems.
![Demo](Demo.gif)
It has the following functions **under development**:

+ **Analyze the content of the AMF file** and display in a tabular, highly readable manner.

+ **Batch import and process** AMF files.

+ **Input an AMF file and render a result** in the way of ACES color pipeline.

+ **Display LMT information** in the form of Cube and Cruve.

+ **Output AMF file  literal information in various forms**(i.e EXCEL, WORD , etc).

+ **Output transforms in AMF individually or in bundles**, in multiple LUT formats. (And also individually display them in the form of Cube and Cruve form).
&nbsp; 

&nbsp; 

&nbsp; 


## Problems with the current version
My ability is limited, I welcome developers to see here to improve this software with me. My best regards.

+ I am currently using the CTLrender command line, which results in an inefficient ACES pipeline.
+ There are elements that may occur multiple times and the software does not read them well. For example, `aces:author` is allowed to appear more than once, but the software currently only read the first one.
+ The `archivedPipeline` element is not currently used very well. I think it is a very useful module, but none of the software vendors seem to use it very well at this moment.
+ The module of `scatter.cpp` for displaying 3D LUT uses the QT tool `Q3DScatter` module, but it canot display colored 3D points. My current idea is to instantiate each point and give each its own color (this might make the software less efficient).
+ [CTLrender is not yet able to handle LMT](https://github.com/pomfort/amf-util/issues/5), so this problem exists in this software as well.
+ The image rendering pipeline uses the [amf-util](https://github.com/pomfort/amf-util) project of [ptrpfn](https://github.com/ptrpfn), which requires a mix of C++ and python, and modifies a bit the original [ptrpfn](https://github.com/ptrpfn) source code by removing the “click" header part. I also did a parse way for rendering pipeline, but that parse way may not be very reliable, as you can see in the source code `std::string usingctlrender(ParseAMF);` which is my own rendering pipeline.
+ Considering that `amf-util` is not stable, so I wrote a parsing method by myself. so when importing a file of .amf, it actually did two renderings (4 times in total) which improves the stability in disguise, i.e. if one pipeline is does not working, there has another one, if you want to close one of them, you can search ` parseamf.cpp` to delate`RenderPic_Whenconstruct(Path);` or search `amfanalyst.cpp` to delate `c1->usingctlrender(amf_file1); `. amfutil(python) uses the `system()` way So when it runs, the system needs to wait. My pipeline uses QT to run in the background without waiting, which can be used as a remedy.
