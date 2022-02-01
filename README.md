<div style="text-align:center">
  <img src="/other/XTETRIS.png" style="display:block; margin:auto; width:100%; max-width:100%"/></a>
</div>


By
[Jacopo Costantini](https://github.com/MacPapo),
[Alvise Silvestri](https://github.com/mastrodeimastri),
[Matteo Zambon](https://github.com/Blast291).

### Advanced Version of the original 1984 game TETRIS 🕹


### Some Picturs of the Project 📺

<div style="text-align:center">
<h4>👾 Game Menu 👾</h4>
  <img src="/other/xmenu.gif" style="display:block; margin:auto; width:60%; max-width:100%"/></a>
</div>

* * *

The game is 100% [ncurses](https://github.com/mirror/ncurses) based

### Installation 💾

For insatalling the program you need to install first ncurses on your computer.

**macOS **
```shell
brew install ncurses
```
**Linux 🐧**

Debian based :
```shell
sudo apt-get install libncurses5-dev libncursesw5-dev
```

### What Now? 🪄

Simply clone the repository on your desire directory
```shell
git clone https://github.com/mastrodeimastri/XTETRIS.git
```
Now enter inside the XTETRIS directory
```shell
cd xtetris
```
type `make setup` to build the project directory
```shell
make setup
```
Than simply `make` to build the project
```shell
make
```
And now you are good to go 🎉

The last command is `make fly`
```shell
make fly
```

Enjoy 👾
* * *

### Todo List [ALL DONE  💪🏻]

- [x] Game Menu
  - [x] Add background & primitive boxes
  - [x] Add dynamic windows
  - [x] Add text highlighting
  - [x] Sync selected text with the description
  - [x] Optimize windows refresh
  - [x] Add music
  
- [x] Single Player Mod 
  - [x] Add background & static windows
  - [x] Add dynamic windows
  - [x] Add game matrix
  - [x] Add falling function
  - [x] Add row deletion function
  - [x] Add scoring function
  - [x] Add help box
  - [x] Add quit & game over banner with score
  
- [x] Multi Player Mod
  - [x] Add background & static windows
  - [x] Add dynamic windows
  - [x] Add game matrixs
  - [x] Add turn methods for players interaction
  - [x] Add falling function
  - [x] Add row deletion function
  - [x] Add field inversion for the other player
  - [x] Add scoring function
  - [x] Add help box
  - [x] Add quit & game over banner with score
  
- [x] Player vs CPU
  - [x] Add background & static windows
  - [x] Add dynamic windows
  - [x] Add game matrixs
  - [x] Add falling function for the player
  - [x] Add Big Brain function for the CPU to interact 
  - [x] Add row deletion function
  - [x] Add field inversion for the other player
  - [x] Add scoring function
  - [x] Add help box
  - [x] Add quit & game over banner with score
  
## Documentation

DOXYGEN DOC 
work in progress ⚠️
