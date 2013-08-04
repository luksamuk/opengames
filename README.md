opengames
=========

Pequenos jogos de exemplo com c�digo aberto.

CBreakout
---------
Clone de Breakout feito em OpenGL.
Status atual:
- Melhorar sistema de FPS;
- Inserir objetos quebráveis;
- Inserir sons simples.

Reversi
-------
Clone de Reversi feito em C e OpenGL.
Status atual:
- Inserir regras do jogo;
- Inserir sistema de contagem de pontos e saída.

CPlatformer
-----------
Platformer r�pido feito em C, SDL e OpenGL.
Instru��es de compila��o utilizando GCC / MinGW:

Linux:
gcc level.c character.c main.c -o platformer -lSDL -lGL

Windows:
gcc level.c character.c main.c -o platformer.exe -lmingw32 -lSDLmain -lSDL -lopengl32

Em ambos os casos, voc� precisar� do SDL instalado em seu computador.
No Linux, voc� pode instalar os pacotes libsdl1.2 e libsdl1.2-dev, e TALVEZ precisar� do pacote freeglut3. Instale-os com os seu gerenciador de pacotes preferido.

No Windows, al�m das bibliotecas de SDL e OpenGL instaladas no seu compilador, voc� precisar� utilizar o arquivo SDL.dll incluso na pasta.

CSonic
------

Engine de platformer baseada no Mega Drive/Genesis.

Atualmente inclu�do:
- Sistema de paletas em bitmask e convers�o para byte.
