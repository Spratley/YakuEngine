# YakuEngine
YakuEngine or *YakuEn* for short is a custom built C++ game engine, made from scratch by me.
This project started as a way for me to keep my engine development skills sharp (Since I don't do much core development in my current job), but has since grown into a passion project that reminds me why I love game development and C++.

YakuEn is structured as a series of static libraries that link together like a pyramid to form the final game.
- YakuCore (YK) holds the most basic core concepts that make the foundation of then engine
- CG is the computer graphics layer that manages the rendering loop of the game
- PPhys (PP) is a wrapper around [Jolt Physics](https://github.com/jrouwe/joltphysics), named after the first physics system I made in University
- YakuEngine (EN) is the top layer that sits between the end user's gameplay code and the engine. It is also responsible for connecting together the other layers of the engine into a unified interface

YakuEn also leverages:
- [HIDra](https://github.com/Spratley/HIDra) for HID polling and user input
- [Zen](https://github.com/Spratley/Zen) for the Entity Component System backbone
(Both of which were custom made for the engine, but designed to stand alone for use in other projects)

As mentioned, the physics solver for YakuEn is [Jolt](https://github.com/jrouwe/joltphysics) an open source and Web Assembly compatible codebase that has been used in AAA projects.

---
The name YakuEn pays homage to the original renderer codebase I made for a simple Riichi Mahjong game, that eventually got recycled into the first version of my engine.