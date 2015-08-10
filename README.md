opengames
=========

Pequenos jogos de exemplo com c�digo aberto.

CBreakout
---------
Clone de Breakout feito em OpenGL/GLUT.
Ainda injog�vel. N�o sei se o continuarei, j� fiz um clone
melhor usando Processing, at� ent�o.
Status atual:
- Melhorar sistema de FPS;
- Inserir objetos quebr�veis;
- Inserir sons simples.

Reversi
-------

![CReversi](screenshots/creversi.png)

Clone de Reversi feito em C e OpenGL.
Status atual:
- Inserir regras do jogo;
- Inserir sistema de contagem de pontos e sa�da.

CPlatformer
-----------

![CPlatformer](screenshots/cplatformer.png)

Platformer r�pido feito em C, SDL 1.2 e OpenGL/GLUT.

CSonic
------

![CSonic](screenshots/csonic.png)

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
- Menu de clique direito, conciso e organizado;
- Inser��o e sele��o de c�lulas com bot�es esquerdo e m�dio do mouse, respectivamente.

A fazer:
- Mais formas comuns, especialmente formas de crescimento indefinido;
- Encontrar uma forma de sair do jogo de forma segura.
