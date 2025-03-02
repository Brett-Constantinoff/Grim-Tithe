# Grim-Tithe

Grim Tithe is a work in progress Quake like first person shooter set in a Warhammer 40k inspired universe.

The goal of the project is to be as verbose as possible and to not over-engineer things. The project is meant to be a learning experience for me and others who are interested in game development.
Even though the language is C++, you'll notice that I dont make use of many OOP principles, this is mainly to avoid over the top, complex abstractions that are not needed for a project of this size.

You may ask yourself "Why are you using C++ then?" the answer to that is because I enjoy the language, duh. What am I gonna use isntead? Rust? I dont think so.

# Cloning
This project uses submodules, so when cloning you'll need to use the `--recurse-submodules` flag.

# Building
You're going to need python installed and added to your PATH, also install using pip the 'requests' package.

Once thats installed run the setup.bat file, this will fetch the Vulkan SDK for you and set the necessary environment variables.

After that you can run the build.bat file to build the project.