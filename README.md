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
Feito em C, utiliza SDL, OpenGL e GLUT.

Atualmente incluído:
- Sistema completo de paletas de cores em bitmask;
- Funções devidas de manipulação de paletas;
- Função para seleção de cor a partir da paleta, similar ao OpenGL;
- Sistema de renderização de fases dentro de um certo limite, para evitar sobrecarga;
- Tiles de teste feitos a partir de código.
- Suporte a resoluções 1x, 2x, 3x, 4x, 720p, 1080p - fullscreen e windowed;
- Suporte a 25, 20, 30 e 60 FPS.

A fazer:
- Escrever câmera;
- Implementar motion blur;
- Escrever programa de criação e edição de fases.
