opengames
=========

Pequenos jogos de exemplo com código aberto.

CBreakout
---------
Clone de Breakout feito em OpenGL.
Status atual:
- Melhorar sistema de FPS;
- Inserir objetos quebrÃ¡veis;
- Inserir sons simples.

Reversi
-------
Clone de Reversi feito em C e OpenGL.
Status atual:
- Inserir regras do jogo;
- Inserir sistema de contagem de pontos e saÃ­da.

CPlatformer
-----------
Platformer rápido feito em C, SDL e OpenGL.
Instruções de compilação utilizando GCC / MinGW:

Linux:
gcc level.c character.c main.c -o platformer -lSDL -lGL

Windows:
gcc level.c character.c main.c -o platformer.exe -lmingw32 -lSDLmain -lSDL -lopengl32

Em ambos os casos, você precisará do SDL instalado em seu computador.
No Linux, você pode instalar os pacotes libsdl1.2 e libsdl1.2-dev, e TALVEZ precisará do pacote freeglut3. Instale-os com os seu gerenciador de pacotes preferido.

No Windows, além das bibliotecas de SDL e OpenGL instaladas no seu compilador, você precisará utilizar o arquivo SDL.dll incluso na pasta.

CSonic
------

Engine de platformer baseada no Mega Drive/Genesis.

Atualmente incluído:
- Sistema de paletas em bitmask e conversão para byte.
