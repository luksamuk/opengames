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
Feito em C, utiliza SDL, OpenGL e GLUT.

Atualmente inclu�do:
- Sistema completo de paletas de cores em bitmask;
- Fun��es devidas de manipula��o de paletas;
- Fun��o para sele��o de cor a partir da paleta, similar ao OpenGL;
- Sistema de renderiza��o de fases dentro de um certo limite, para evitar sobrecarga;
- Tiles de teste feitos a partir de c�digo.
- Suporte a resolu��es 1x, 2x, 3x, 4x, 720p, 1080p - fullscreen e windowed;
- Suporte a 25, 20, 30 e 60 FPS;
- C�mera 2D para cada level;
- Suporte total � cor transparente.

A fazer:
- Substituir bloco zero como bloco comum, e completamente transparente por default. Rever esta decis�o, por�m, pois se o bloco zero for tratado de forma especial, a renderiza��o ser� um pouco otimizada.
- Armazenar modo da paleta na representa��o da cor;
- Fun��o para resgatar modo da paleta na representa��o da cor;
- Colis�o para cada piece;
- Implementar motion blur;
- Escrever programa de cria��o e edi��o de fases;
- Terminar fun��es de carregar/exportar paletas e fases;
- Inserir personagens, inimigos, itens e etc que podem ser configurados atrav�s de script.

CGOL
-----

![CGOL](screenshots/cgol.png)

Vers�o pr�pria do Jogo da Vida de Conway.
Escrito em C++, utiliza OpenGL/GLUT.

Atualmente conclu�do:
- Implementa��o da grade e das regras do jogo;
- Implementa��o do redimensionamento da tela;
- Atalhos de teclado;
- Formas comuns para gera��o atrav�s de menu com clique direito;
- Menu de clique direito;

A fazer:
- Mais formas comuns, especialmente formas de crescimento indefinido;
- Criar um menu mais conciso;
- Criar um menu melhor organizado em termos de cria��o de formas, por categoria.
