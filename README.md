# PathTracing

How to run the code
In windows you can use msvc 
In directory DawnTracerMux/
Enter the code in terminal
```
cmake -S . -B build/msvc(the directroy you like) -G ""(your compiler, often used default) 
```
Then build the project, enter like that
```
cmake --build build/msvc(the directroy you like) --config Debug(or Release)
```

In linux can use GCC
```
#cmake -S . -B build/gcc(the directroy you like) -G ""(your compiler, often used default) -DMAKE_BUILD_TYPE=Debug(or Release)
```

Then build the project
```
#cmake --build build/gcc(the directroy you like)
```

In directory DawnTracerMux/app/assets/scene.json, you can change the value about spp to generate different results, but all of those are the .pmm picture which you should have a way to realistic pictures in windows system.(you can donwload some plugin in vscode to show the picture);



