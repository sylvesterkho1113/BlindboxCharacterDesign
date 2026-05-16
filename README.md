# 📦 Blindbox Character Design: Pokémon Battle Arena

[![C++](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-Fixed--Function-orange.svg)](https://www.opengl.org/)
[![Status](https://img.shields.io/badge/Status-Completed-green.svg)]()

A high-fidelity **Computer Graphics (OpenGL)** project featuring stylized "Blindbox" versions of **Minun** and **Weedle**. This project demonstrates advanced 3D modeling, hierarchical transformations, dynamic lighting, and interactive animations within a rich village environment.

---

## 🌟 Overview

Developed as a group project for the **TCG6223 Computer Graphics** course at Multimedia University (MMU), this application brings the Pokémon world to life through a custom-built 3D engine. The "Blindbox" aesthetic is achieved through clean, primitive-based modeling that emphasizes the collectible figurine look.

### 🎥 Key Highlights
*   **Dynamic Battle System**: Interactive skills with unique 3D projectiles.
*   **Immersive Environment**: A circular village architecture with procedural forests and grass.
*   **Advanced Rendering**: Multi-source lighting, texture mapping, and real-time animations.

---

## ✨ Features

### 🧊 3D Character Modeling (Hierarchical)
*   **Minun**: Crafted using specialized profile-point surface generation for organic shapes. Includes detailed hand, ear, and expression modeling.
*   **Weedle**: A complex multi-segmented body structure utilizing hierarchical transformations to simulate a worm-like appearance.
*   **Primitive Foundation**: Built entirely from spheres, cylinders, disks, and custom-generated surfaces.

### 🏘️ Environment & Scene Design
*   **Village Arc**: 12 modularly designed houses arranged in a perfect circle, all facing the central arena.
*   **Battle Arena**: A high-tech arena featuring a central **Poké Ball** floor design and HUD screens.
*   **Procedural Nature**: Randomized forest generation and grass distribution for a natural feel.
*   **Architecture**: Houses featuring balconies, windows (circle/square), and textured roofs.

### ⚔️ Combat & Animation
*   **Minun's "Minus" Strike**: Fires glowing blue 3D minus-sign projectiles.
*   **Weedle's "Poison Drop"**: Launches purple poison droplet projectiles.
*   **Smooth Motion**: Time-based animations ensuring consistent speed across different hardware.

### 💡 Graphics Engine Details
*   **Lighting System**: 
    *   `Light 0`: Ambient white light (The Sun).
    *   `Light 1`: Red spotlight (Weedle's Side).
    *   `Light 2`: Blue spotlight (Minun's Side).
*   **Texture Mapping**: High-quality textures for bricks, wood, and roofing.
*   **Rendering Modes**: Toggle between full shading and wireframe mode for architectural analysis.

---

## 🎮 Controls

| Category | Key | Action |
| :--- | :---: | :--- |
| **Movement** | `W` `A` `S` `D` | Move Camera/World |
| | `Q` / `E` | Fly Up / Down |
| **Rotation** | `Arrow Keys` | Rotate View |
| | `Mouse Drag` | Orbit Scene |
| **Actions** | `F9` | **Minun Skill** (Blue Side) |
| | `F10` | **Weedle Skill** (Red Side) |
| **Toggles** | `F1` | Toggle Shading / Wireframe |
| | `F2` | Toggle Coordinate Axes |
| | `F3` | Toggle Textures |
| | `F4` | Toggle Auto-Rotation |
| | `F5` | Master Lighting Toggle |
| | `F6` - `F8` | Individual Light Toggles |
| **System** | `HOME` | Reset Viewport |
| | `ESC` | Exit Application |

---

## 🛠️ Tech Stack & Dependencies

*   **Language**: C++
*   **Graphics API**: OpenGL (Fixed-Function Pipeline)
*   **Utility Libraries**: 
    *   **GLUT / FreeGLUT**: Windowing and input management.
    *   **GLU**: Quadric objects and perspective calculations.
    *   **JPEG Library**: Handling custom textures (`.jpg`).

---

## 📁 Project Structure

```text
BlindboxCharacterDesign/
├── CGLab11Utils/       # Core utility functions & image loaders
├── image/              # Texture assets (brick, wood, roof)
├── ProjectMain.cpp     # Main logic, modeling, and rendering
├── ProjectMain.hpp     # Class definitions & scene hierarchy
├── RationalMain.cpp    # OpenGL initialization & input callbacks
├── RationalMain.hpp    # Window & world settings
├── glut32.dll          # Runtime dependency
└── Rational_1E.cbp     # Code::Blocks Project File
```

---

## 👥 Contributors (Group Rational - Section 1E)

*   **See Chwan Kai** (242UT2449P)
*   **Teo Jing An** (242UT24490)
*   **Kho Wei Cong** (242UT2449Z)
*   **Tee Kian Hao** (242UT244B2)

---

## 📝 License

This project was developed for educational purposes at **Multimedia University (MMU)**.
