
# Pathfinder

A fourth semester project on Data Structures And Algorithms made by Computer Engineering students of IOE Pulchowk Campus. Made using C++ and SFML.

## Contributors

- [Bishal Lamichhane](https://www.github.com/bishallamichhane44) (078BCT035)
- [Roshan Karki](https://github.com/roshankc00) (078BCT098)
- [Bipin Bashyal](https://www.github.com/bipinbashyal) (078BCT033)
- [Ayush KC](https://www.github.com/AyusK4) (078BCT025)












## Build

### Requirements:
To setup and use the project, you will need to download the following:
- [SFML](https://www.sfml-dev.org/files/SFML-2.6.0-windows-gcc-13.1.0-mingw-32-bit.zip)
- [MINGW](https://github.com/brechtsanders/winlibs_mingw/releases/download/13.1.0-16.0.5-11.0.0-msvcrt-r5/winlibs-i686-posix-dwarf-gcc-13.1.0-mingw-w64msvcrt-11.0.0-r5.7z)

Cut MinGW and SFML-2.6.0 folder and paste in your C: drive.
Add path to environment variables for:
- **bin** from MinGW folder
- **bin** form SFML-2.6.0 folder


### Installation:

- Use the following build commnads.
```bash
  $ g++ -IC:\SFML-2.6.0\include -c main.cpp -o main.o
```
```bash
  $ g++ -LC:\SFML-2.6.0\lib .\main.o -o main.exe -lmingw32 -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lsfml-main -mwindows
```
```bash
  $ main.exe
```
  + Double click o.bat.
  + Double click exe.bat.
  + Main.exe will appear.(Open it to Run the program).


### Note:
- You must use above mentioned MinGW and SFML versions (Download from the link). Remove your already existing versions.






