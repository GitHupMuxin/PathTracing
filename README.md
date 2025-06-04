# PathTracing

## How to run the code.<br>
In Windows system you can use msvc conpiler.<br> In directory **./DawnTracerMux/**.<br>Enter the code in terminal.<br>
```
cmake -S . -B build/msvc(the directroy you like) -G ""(your compiler, often used default) -DCMAKE_BUILD_TYPE=Debug(or Release)
```
Then build the project, enter like that.
```
cmake --build build/msvc(the directroy you like) --config Debug(or Release)
```
In Linux system you can use GCC.
```
#cmake -S . -B build/gcc(the directroy you like) -G ""(your compiler, often used default) -DCMAKE_BUILD_TYPE=Debug(or Release)
```
Then build the project.
```
#cmake --build build/gcc(the directroy you like)
```
In directory **./DawnTracerMux/app/assets/scene.json**, you can change the value about spp to generate different results, but all of those are the .pmm picture which you should have a way to realistic pictures in Windows system.(you can donwload some plugin in vscode to show the picture);



