# 🎮 Pokémon Mini Game (Weedle vs Minun)

## 📌 Overview

This is a **Computer Graphics (OpenGL) based 3D mini game** featuring two Pokémon-inspired characters: **Minun** and **Weedle**. The project demonstrates core computer graphics concepts such as 3D modeling, hierarchical transformations, lighting, texture mapping, and animation using OpenGL and GLUT.

The scene includes a fully designed battle arena surrounded by a village environment and forest, creating an immersive Pokémon-style battle experience.

---

## ✨ Features

### 🧍 3D Character Modeling

* Fully modeled Minun and Weedle characters
* Built using OpenGL primitives (spheres, cylinders, cubes, quads)
* Hierarchical structure (head, body, limbs, tail, etc.)

### 🌍 Environment Design

* Circular village with multiple houses
* Forest environment with trees and grass
* Detailed battle arena with walls and floor
* Central Poké Ball floor design
* Lamp posts with colored lighting effects

### ⚔️ Battle & Animation Effects

* Minun attack: blue “minus sign” projectiles
* Weedle attack: purple poison droplet projectiles
* Smooth projectile movement animations

### 💡 Graphics & Rendering Features

* OpenGL lighting system (white, red, blue lights)
* Texture mapping support
* Wireframe / shaded mode toggle
* Camera movement and world rotation
* HUD text display overlay

---

## 🧠 Tech Stack

* C / C++
* OpenGL (GLUT)
* GLU
* FreeGLUT
* JPEG texture libraries

---

## 🎮 Controls

| Key        | Action                         |
| ---------- | ------------------------------ |
| W A S D    | Move camera/world              |
| Q / E      | Move up/down                   |
| Arrow Keys | Rotate view                    |
| Mouse Drag | Rotate scene                   |
| HOME       | Reset view                     |
| ESC        | Exit program                   |
| F1         | Toggle wireframe / shading     |
| F2         | Toggle axis display            |
| F3         | Toggle textures                |
| F4         | Auto-rotate scene              |
| F5         | Toggle lighting                |
| F6         | Toggle white light             |
| F7         | Toggle red light (Weedle lamp) |
| F8         | Toggle blue light (Minun lamp) |
| F9         | Trigger Minun attack skill     |
| F10        | Trigger Weedle attack skill    |

---

## 📁 Project Structure

```
project/
├── ProjectMain.cpp
├── ProjectMain.hpp
├── RationalMain.cpp
├── RationalMain.hpp
├── CGLab11Utils/
├── image/
├── glut32.dll
├── jpeg62.dll
├── libjpeg.dll.a
└── Rational_1E.cbp
```

---


## 📚 Key Concepts Used

* 3D hierarchical modeling
* Transformation (translate, rotate, scale)
* Primitive-based modeling
* Lighting and shading
* Texture mapping
* Animation system using projectile logic
* OpenGL fixed-function pipeline

---

## 📌 Conclusion

This project demonstrates how OpenGL can be used to build a complete interactive 3D environment with animated characters, lighting effects, and structured scene design, simulating a Pokémon-style battle game experience.
