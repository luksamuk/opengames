opengames
=========

Pequenos jogos de exemplo com c�digo aberto.

cl-2048
-------

![cl-2048](screenshots/cl-2048.png)

Clone de 2048 em Common Lisp.

Usa SDL2/OpenGL via Quicklisp.
Otimizado para Emacs + SLIME com SBCL.

A fazer:
- Telas r�pidas de Win/Lose com score

cl-snake
--------

![cl-snake](screenshots/cl-snake.png)

Mais um clone de Snake, por�m feito em Common Lisp.

Usa SDL2 e OpenGL via Quicklisp.
Otimizado para Emacs + SLIME com SBCL.

A fazer:
- Corrigir bug de spawn da fruta
- Adicionar menus
- Adicionar tela de debriefing ao morrer

Para uma vers�o execut�vel, veja [a p�gina do jogo no itch.io](https://luksamuk.itch.io/cl-snake).

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

dagarden
-----------

![cl-dagarden](screenshots/cl-dagarden.png)

Clone do modo D.A. Garden, do jogo Sonic CD, em C++ e Common Lisp,
utilizando SDL2, SDL2\_Image e OpenGL.

Vers�o em C++ usa tamb�m OficinaFramework 1.3, OpenAL, Ogg, Vorbis, Vorbisfile e PhysicsFS.

Ainda em fase de testes e incompleto.

A fazer:
- Sprites e objetos variados passando pela tela
- M�sica de fundo
- Menu com alguma coisa?

NOTA: Tamb�m inclusa nesta pasta est� uma vers�o 100% em Scheme (dagarden.scm),
compat�vel com Chicken Scheme, por�m inacabada; o suporte a texturas n�o
parece funcionar bem nos bindings de OpenGL para Scheme.







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
