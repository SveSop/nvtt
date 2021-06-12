# nvTT
nvAPI Tiny Test  
  
This aims to give some testfunctions for various nvAPI functions run from wine.  
  
The focus being to test functions added to projects like dxvk-nvapi and the likes  
[https://github.com/jp7677/dxvk-nvapi](https://github.com/jp7677/dxvk-nvapi)  
  
It will also run on windows for reference, so values and functions can be somewhat verified for expected outcome when using an nVidia GPU  

## Requirements  
This project requires meson, ninja and MinGW to compile.  

## Compile  
Compile the project by running `./package-release.sh foldername`  
  
## Usage  
Use the compiled nvtt.exe by either doubleclick in Windows, or by running `wine ./nvtt.exe`  
