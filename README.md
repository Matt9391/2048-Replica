# 2048-Replica
---
## C++ remake of the iconic sliding tile puzzle

**2048-Replica** is a simple but complete recreation of the classic 2048 puzzle game.  
It implements the original mechanics: slide, merge, and try to reach the **2048 tile**!

---

## Features

- Fully working **4×4 grid** puzzle logic
- Sliding + merging mechanic like the original game
- Random tile spawning (`2` or `4`) after each valid move
- Win detection when reaching **2048**
- Basic defeat condition when no moves remain
- Visual grid drawn with tile sprites
- Grid also printed to console for debugging

---

## Gameplay
Every move merges tiles with the same value while shifting everything in the chosen direction.

### Controls

- **W** → Slide Up  
- **S** → Slide Down  
- **A** → Slide Left  
- **D** → Slide Right  

---

## Objective

Combine tiles together to create increasingly larger numbers:  
`2 → 4 → 8 → 16 → 32 → ... → 2048`

Reach **2048** to win!

If the board fills up and no tiles can merge anymore → **Game Over**.

---

## Project Notes

This project was created to:
- Practice matrix/grid manipulation  
- Learn tile merging logic and input handling  
- Experiment with simple game rendering in C++  
- Improve understanding of clean function-based logic

There is **a known flaw** in tile generation where invalid loss detection may trigger very rarely — intentionally left in as part of the learning process

---

## Future Enhancements

- Restart screen  
- Score tracking  
- Fix the `generateNewCell` reliability edge case  


