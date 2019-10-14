# show.cpp
This presentation software was built using a snapshot of io2d, the C++ standard graphics proposal which you can find at [https://github.com/cpp-io2d/P0267_RefImpl](https://github.com/cpp-io2d/P0267_RefImpl).

This presentation was first delivered at C++Russia, St Petersburg, on October 31st 2019. If you would like to build and run the presentation yourself you will need Visual Studio 2019 and you will need to have vcpkg installed, which you can find at [https://github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg). Use vcpkg to install Cairo and GraphicsMagick before attempting to build the presentation: this process is described at the io2d repository in [https://github.com/cpp-io2d/P0267_RefImpl/blob/master/BUILDING.md](https://github.com/cpp-io2d/P0267_RefImpl/blob/master/BUILDING.md).

show.cpp is a static library that you link against your presentation. When you have installed all the components, build the solution and run StPetersburg.exe. Note that you will need to modify StPetersburg.h to point to the resource directory.

If you just want to run the presentation, the navigation keys are Z and X. On slide 79 you will also need to use Q, W, U, I and P.

This is the first draft of the library. It is exceedingly rough around the edges and limited by the current state of the io2d implementation.

To Do:

 - Make the St Petersburg presentation resolution independent

Thanks to the io2d team for keeping things going, and to Hana Dusikova for prompting me to create a piece of open source C++ presentation software.