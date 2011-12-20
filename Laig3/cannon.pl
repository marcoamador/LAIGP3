
:- use_module(library(lists)).
:- use_module(library(chr)).
:- use_module(library(random)).
:- use_module(library(sockets)).
:- use_module(library(clpfd)).
:- use_module(library(between)).
:-use_module(library(sockets)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% SOCKETS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


port(60001).

server:-
        port(Port),
        socket_server_open(Port,Socket),
        socket_server_accept(Socket, _Client, Stream, [type(text)]),
        server_loop(Stream),
        socket_server_close(Socket),!,
        write('Server Exit'),nl.

server_loop(Stream) :-
        repeat,
                read(Stream, ClientRequest),
                write('Received: '), write(ClientRequest), nl, 
                server_input(ClientRequest, ServerReply),
                format(Stream, '~q.~n', [ServerReply]),
                write('Send: '), write(ServerReply), nl, 
                flush_output(Stream),
        (ClientRequest == bye; ClientRequest == end_of_file), !.


%server_input(ai([Level,Jogador])).
server_input(ai([1,1],Board),ok(NewBoard)):-
        aiLevel1Jog1(Board, NewBoard),!.
server_input(ai([2,1],Board),ok(NewBoard)):-
        aiLevel2Jog1(Board, NewBoard),!.
server_input(ai([1,2],Board),ok(NewBoard)):-
        aiLevel1Jog2(Board, NewBoard),!.
server_input(ai([2,2],Board),ok(NewBoard)):-
        aiLevel2Jog2(Board, NewBoard),!.

%posiciona_cidade(1,Tab, 1, NewTab,T1):-
server_input(execute([Jogador,Modo,Col],Board),ok(NewBoard)):-
        %Mov=[Jogador,Modo,Col],
        write('city pos'),nl,
        posiciona_cidade(Jogador,Board, Modo, NewBoard,Col),!.
server_input(initialize, ok(Board)):- 
        tabuleiro_inicial(Board), !.
server_input(execute([Jogador,Opcao,L_ini,C_ini,L_fin,C_fin], Board), ok(NewBoard)):-
        (executa_jogada(Jogador,Opcao,L_ini,C_ini,L_fin,C_fin,Board,New2Board, 0)->NewBoard=New2Board;NewBoard=Board), !.
server_input(calculate(Level, J, Board), ok(Mov, NewBoard)):-  %Not implemented
        calculate_move(Level, J, Board, Mov),
        execute_move(Mov, Board, NewBoard), !.
server_input(game_end(Board), ok(Winner)):- 
        verificaFim(Board,Winner), !.
server_input(bye, ok):-!.
server_input(end_of_file, ok):-!.
server_input(_, invalid) :- !.



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% UTILITARIOS E PERSONALIZAVEIS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

rep_p1(r).
rep_p2(b).
rep_branco(x).
rep_cidade1(t).
rep_cidade2(y).

separador(' ').
p1_peca('R').
p2_peca('B').
cidade('T').

%Numero de pecas de cada coluna
linhas_jogador(3).
%Numero de linhas entre as pecas de cada jogador
linhas_brancas(2).

%X e o tamanho do tabuleiro
tamanho_tabuleiro(X) :-
        linhas_jogador(H),
        linhas_brancas(M),
        X is H*2 + M + 2.

inserir_em(X,L,K,R):-
        remover_em(X,R,K,L).

remover_em(X,[X|Xs],1,Xs).

remover_em(X,[Y|Xs],K,[Y|Ys]) :-
        K > 1,
        K1 is K - 1,
        remover_em(X,Xs,K1,Ys), !.

%Substitui Antigo por Novo
substituir(Novo, Antigo, L, Index, F):-
        remover_em(Antigo, L, Index, R),
        inserir_em(Novo,R,Index, F), !.

%substituir_alt(OldList, InitialIndex, NewList, 1):-
%        rep_branco(T),
 %       substituir(T, _ , OldList, InitialIndex,NewList).
        
substituir_alt(OldList, InitialIndex, NewList, N):-
                (N =:= 1 ->  rep_branco(T),
        substituir(T, _ , OldList, InitialIndex,NewList), !;
                (rep_branco(T), substituir(T, _, OldList, InitialIndex,IntList),
        InitialIndex1 is InitialIndex+2,
        N1 is N - 1,
        substituir_alt(IntList, InitialIndex1, NewList, N1))).
                                          
gerar_lista(Element, NElements, [Element|Result]) :-
        NElements > 0,
        NewNElements is NElements - 1,
        gerar_lista(Element, NewNElements, Result).
  
gerar_lista(_, 0, []).

concatenar([], List, List).

concatenar([Head|Tail1], List, [Head|Tail2]) :-
        concatenar(Tail1, List, Tail2).
              
%Recebe input e valida-o caso esteja no intervalo pretendido                      
le_e_valida_numero(LimInf, LimSup, Resposta):- repeat,
        read(Resposta), between(LimInf, LimSup, Resposta).
        
elemento([Head|_], 0, Head).

elemento([_|Tail], Index, Element) :-
        Index \= 0,
        NewIndex is Index - 1,
        elemento(Tail, NewIndex, Element).

posicao_valida(X, Y):-
        tamanho_tabuleiro(L),
        X >= 0,
        X < L,
        Y >= 0,
        Y < L.
  
%Devolve em Elemento a peca de coordenadas X, Y  
elemento_matriz(Tab, X, Y, Elemento) :-
        %posicao_valida(X, Y),
        elemento(Tab, Y, Linha),
        elemento(Linha, X, Elemento).
  
membro(X, [X|_]).

membro(X, [_|Y]):-
        membro(X, Y), !.
        
altera_elemento_lista([Head|Tail], Index, Element, Result) :-
        Index > 0,
        NewIndex is Index - 1,
        altera_elemento_lista(Tail, NewIndex, Element, NewList),
        concatenar([Head], NewList, Result).
  
altera_elemento_lista([_|Tail], 0, Element, Result) :-
        concatenar([Element], Tail, Result).  
  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% IMPRESSAO E VISUALIZACAO TABULEIRO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

%Cria o tabuleiro inicial
tabuleiro_inicial(Tabuleiro) :-
        rep_p1(P1),
        rep_p2(P2),
        rep_branco(B),
        tamanho_tabuleiro(Tam),
        gerar_lista(P1, Tam, P1Linha),
        gerar_lista(P2, Tam, P2Linha),
        substituir_alt(P1Linha, 1, P1Linha2, Tam/2),
        substituir_alt(P2Linha, 2, P2Linha2, Tam/2),
        gerar_lista(B, Tam, LinhaBranca),
        linhas_jogador(NLinhas),
        linhas_brancas(NLinhasBrancas),
        gerar_lista(P1Linha2, NLinhas, LinhasP1),
        gerar_lista(P2Linha2, NLinhas, LinhasP2),
        gerar_lista(LinhaBranca, 1, LinhaCidade),
        gerar_lista(LinhaBranca, NLinhasBrancas, LinhasBrancas),
        concatenar(LinhaCidade, LinhasP1, TempBoard),
        concatenar(TempBoard, LinhasBrancas, TempBoard2),
        concatenar(TempBoard2, LinhasP2, TempBoard3),
        concatenar(TempBoard3, LinhaCidade, Tabuleiro).

%Imprime coordenadas horizontais
imprime_coordenadas(Valor):-
            tamanho_tabuleiro(Tamanho),
                Tamanho =< 10,
        Valor < Tamanho,
        NovoValor is Valor + 1,
        write(NovoValor),
        write(' '),
        imprime_coordenadas(Valor + 1).
                
imprime_coordenadas(Valor):-
            tamanho_tabuleiro(Tamanho),
                Tamanho > 10,
                imprime_linha_1(1), nl,
                write('    '),
                imprime_linha_2(1).
 
imprime_linha_1(Valor):-
                tamanho_tabuleiro(Tamanho),
                Valor =< Tamanho,
                Valor < 10,
                write('  '),
                NovoValor is Valor + 1,
                imprime_linha_1(NovoValor).

imprime_linha_1(Valor):-
                tamanho_tabuleiro(Tamanho),
                Valor > Tamanho.

imprime_linha_1(Valor):-
                tamanho_tabuleiro(Tamanho),
                Valor =< Tamanho,
                Valor >= 10,
                V is Valor // 10,
                write(V),
                write(' '),
                NovoValor is Valor + 1,
                imprime_linha_1(NovoValor).
                
imprime_linha_2(Valor):-
                tamanho_tabuleiro(Tamanho),
                Valor =< Tamanho,
                Valor < 10,
                write(Valor),
                write(' '),
                NovoValor is Valor + 1,
                imprime_linha_2(NovoValor).
                
imprime_linha_2(Valor):-
                tamanho_tabuleiro(Tamanho),
                Valor =< Tamanho,
                Valor >= 10,
                V0 is Valor mod 10,
                V is V0,
                write(V),
                write(' '),
                NovoValor is Valor + 1,
                imprime_linha_2(NovoValor).
                
imprime_linha_2(Valor):-
                tamanho_tabuleiro(Tamanho),
                Valor > Tamanho.
                
imprime_coordenadas(Valor):-
        tamanho_tabuleiro(Tamanho),
        Valor >= Tamanho.
 
%Imprime o tabuleiro 
imprime_tabuleiro([]).

imprime_tabuleiro(Tab):-
        write('    '),
        imprime_coordenadas(0), nl,
        imprime_tabuleiro(Tab, 1).

imprime_tabuleiro([],_).

imprime_tabuleiro([X|R], Pos):-
        Pos < 10,
        tamanho_tabuleiro(Tamanho),
        Pos < Tamanho,
        write(Pos),
        write('  |'),
        imprime_linha(X),
        write('|'),     nl,
        Pos1 is Pos+1,
        imprime_tabuleiro(R, Pos1), !.
                
imprime_tabuleiro([X|R], Pos):-
        Pos >= 10,
        write(Pos),
        write(' |'),
        imprime_linha(X),
        write('|'),     nl,
        Pos1 is Pos+1,
        imprime_tabuleiro(R, Pos1), !.

imprime_linha([]).

imprime_linha([Y|Z]):-
        imprime(Y),
        write(' '),
        imprime_linha(Z).

imprime('t1'):- cidade(T), write(T).
imprime('t2'):- cidade(T), write(T).
imprime('x'):- separador(X), write(X).
imprime('r'):- p1_peca(R), write(R).
imprime('b'):- p2_peca(B), write(B).

vermelho(1).

preto(2).

adjacente(1).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INICIALIZACAO DO JOGO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Base do jogo
inicio:- 
        write(' __________________________________________________________________ '), nl,
        write('|                                                                  |'), nl,
        write('|                    Cannon: The Emperors Game                     |'), nl,
        write('|__________________________________________________________________|'), nl,
        write('|                                                                  |'), nl,
        write('|         Ana Carraca(100509001) | Marco Amador(090509006)         |'), nl,
        write('|              FEUP - Programacao em Logica - 2011/12              |'), nl,
        write('|__________________________________________________________________|'), nl,
        nl,
        tipo_jogo(X, Dif, Dif2),
        jogadorInicial(Jogador),
        tabuleiro_inicial(Tabuleiro),
        imprime_tabuleiro(Tabuleiro), nl,
        posiciona_cidade(Tabuleiro, X, Tab2),
        jogo(X, Dif, Dif2, Jogador, Tab2).


%Posiciona as cidades para o modo 1 de jogo (hum - hum)  sockets          
posiciona_cidade(1,Tab, 1, NewTab,T1):-
        tamanho_tabuleiro(Tamanho),
        write('Jogador 1, indique coluna para colocacao da sua cidade'), nl,
        %S1 is Tamanho-1,
        %le_e_valida_numero(2, S1, T1),
        Col is T1-1,
        rep_cidade1(X1),
        %rep_cidade2(X2),
        elemento(Tab, 0, Linha1),
        altera_elemento_lista(Linha1, Col, X1, Linha1Alt),
        altera_elemento_lista(Tab, 0, Linha1Alt,NewTab).
        %Temp = TabTemp,
        %write('Jogador 2: indique coluna para colocaÃ§Ã£o da sua cidade'), nl,
       % le_e_valida_numero(2, S1, T2),
        %Col2 is T2-1,
        %elemento(Temp, S1, Linha2),
        %altera_elemento_lista(Linha2, Col2, X2, Linha2Alt),
        %altera_elemento_lista(Temp, Tamanho-1, Linha2Alt, NewTab).

posiciona_cidade(2,Tab, 1, NewTab,T2):-
        tamanho_tabuleiro(Tamanho),
        rep_cidade2(X2),
        S1 is Tamanho-1,
        Col2 is T2-1,
        elemento(Tab, S1, Linha2),
        altera_elemento_lista(Linha2, Col2, X2, Linha2Alt),
        altera_elemento_lista(Tab, Tamanho-1, Linha2Alt, NewTab).

%Posiciona as cidades para o modo 1 de jogo (hum - hum)            
posiciona_cidade(Tab, 1, NewTab):-
        tamanho_tabuleiro(Tamanho),
        write('Jogador 1, indique coluna para colocao da sua cidade'), nl,
        S1 is Tamanho-1,
        le_e_valida_numero(2, S1, T1),
        Col is T1-1,
        rep_cidade1(X1),
        rep_cidade2(X2),
        elemento(Tab, 0, Linha1),
        altera_elemento_lista(Linha1, Col, X1, Linha1Alt),
        altera_elemento_lista(Tab, 0, Linha1Alt, TabTemp),
        Temp = TabTemp,
        write('Jogador 2: indique coluna para colocao da sua cidade'), nl,
        le_e_valida_numero(2, S1, T2),
        Col2 is T2-1,
        elemento(Temp, S1, Linha2),
        altera_elemento_lista(Linha2, Col2, X2, Linha2Alt),
        altera_elemento_lista(Temp, Tamanho-1, Linha2Alt, NewTab).
    
%Posiciona as cidades para o modo 2 de jogo (hum - comp)                
posiciona_cidade(Tab, 2, NewTab):- tamanho_tabuleiro(Tamanho),
        write('Jogador 1, indique coluna para colocao da sua cidade'), nl,
        S1 is Tamanho-1,
        le_e_valida_numero(2, S1, T1),
        Col is T1-1,
        rep_cidade1(X1),
        rep_cidade2(X2),
        elemento(Tab, 0, Linha1),
        altera_elemento_lista(Linha1, Col, X1, Linha1Alt),
        altera_elemento_lista(Tab, 0, Linha1Alt, TabTemp),
        Temp = TabTemp,
        random(2, Tamanho, T2),
        Col2 is T2-1,
        elemento(Temp, S1, Linha2),
        altera_elemento_lista(Linha2, Col2, X2, Linha2Alt),
        altera_elemento_lista(Temp, Tamanho-1, Linha2Alt, NewTab).
        
%Posiciona as cidades para o modo 3 de jogo (comp - comp)               
posiciona_cidade(Tab, 3, NewTab):- tamanho_tabuleiro(Tamanho),
        random(2, Tamanho, T1),
        S1 is Tamanho-1,
        Col is T1-1,
        rep_cidade1(X1),
        rep_cidade2(X2),
        elemento(Tab, 0, Linha1),
        altera_elemento_lista(Linha1, Col, X1, Linha1Alt),
        altera_elemento_lista(Tab, 0, Linha1Alt, TabTemp),
        Temp = TabTemp,
        random(2, Tamanho, T2),
        Col2 is T2-1,
        elemento(Temp, S1, Linha2),
        altera_elemento_lista(Linha2, Col2, X2, Linha2Alt),
        altera_elemento_lista(Temp, Tamanho-1, Linha2Alt, NewTab).

%Modo de jogo 1
jogo(1, Dif, Dif2, P , Tabuleiro):-
        imprime_tabuleiro(Tabuleiro),
        processa_tipo_jogada(P, Tabuleiro, Tab2),
        mudaJogador(P, Next),
        verificaFim(1, Dif, Dif2, Next, Tab2).

%Modo de jogo 2 
jogo(2, Dif, Dif2, P , Tabuleiro):-
        imprime_tabuleiro(Tabuleiro),
        (P = 1 -> processa_tipo_jogada(P , Tabuleiro, Tab2);
        (P = 2, Dif2 = 2 -> aiLevel2Jog2(Tabuleiro,Tab2);
        (P = 2, Dif2 = 1 -> aiLevel1Jog2(Tabuleiro,Tab2)))),
        mudaJogador(P, Next),
        verificaFim(2, Dif, Dif2, Next, Tab2).
        
%Modo de jogo 3
jogo(3, Dif, Dif2, P, Tabuleiro):-
        imprime_tabuleiro(Tabuleiro),
        (P = 1, Dif = 1 -> aiLevel1Jog1(Tabuleiro,Tab2);
        (P = 1, Dif = 2 -> aiLevel2Jog1(Tabuleiro,Tab2);
        (P = 2, Dif2 = 1 -> aiLevel1Jog2(Tabuleiro,Tab2);
        (P = 2, Dif2 = 2 -> aiLevel2Jog2(Tabuleiro,Tab2))))),
        mudaJogador(P, Next),
        verificaFim(3, Dif, Dif2, Next, Tab2).
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INTELIGENCIA ARTIFICIAL %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%IA para o nivel aleatorio, para o jogador 1 (vermelho)
aiLevel1Jog1(Tab,TabFinal):-
                (adjacente_cidade(1, Tab, TabFinal)
        ;
        (repeat,
                tamanho_tabuleiro(Tamanho),
        random(0, Tamanho, Col),
                random(0, Tamanho, Lin),
                elemento_matriz(Tab, Lin, Col, PecaAleatoria),
        PecaAleatoria = 'r',
        random(1, 6, Mov),
        desloca(1, Mov, Lin + 1, Col + 1, Tab, TabFinal))).
  
%IA para o nivel aleatorio, para o jogador 2 (preto)  
aiLevel1Jog2(Tab,TabFinal):-
        (adjacente_cidade(2, Tab, TabFinal)
        ;
        (repeat,
                tamanho_tabuleiro(Tamanho),
        random(0, Tamanho, Col),
        random(0, Tamanho, Lin),
        elemento_matriz(Tab, Col, Lin, PecaAleatoria),
        PecaAleatoria = 'b',
                random(1, 6, Mov),
        desloca(2, Mov, Lin + 1, Col + 1, Tab, TabFinal))).

%Verifica se hÃ¡ alguma peÃ§a numa posiÃ§Ã£o adjacente a cidade inimiga para o jogador1
adjacente_cidade(1, Tab, NovoTab):-
        tamanho_tabuleiro(Tamanho),
        elemento_matriz(Tab, Tamanho - 1, Cidade, 't1'),
        (   (Lin is Tamanho - 1, Col is Cidade - 1, elemento_matriz(Tab, Lin, Col, 'r'));
                (Lin is Tamanho - 1, Col is Cidade + 1, elemento_matriz(Tab, Lin, Col, 'r'));
                (Lin is Tamanho - 2, Col is Cidade - 1, elemento_matriz(Tab, Lin, Col, 'r'));
                (Lin is Tamanho - 2, Col is Cidade, elemento_matriz(Tab, Lin, Col, 'r'));
                (Lin is Tamanho - 2, Col is Cidade + 1, elemento_matriz(Tab, Lin, Col, 'r'))
        ),
        executa_jogada(1, 2, Lin, Col, Cidade, Tamanho - 1, Tab, NovoTab, 1).

%Verifica se hÃ¡ alguma peÃ§a numa posiÃ§Ã£o adjacente a cidade inimiga para o jogador2     
adjacente_cidade(2, Tab, NovoTab):- %jogador2
        elemento_matriz(Tab, 0, Cidade, 't2'),
        (   (Lin is 0, Col is Cidade - 1, elemento_matriz(Tab, Lin, Col, 'b'));
                (Lin is 0, Col is Cidade + 1, elemento_matriz(Tab, Lin, Col, 'b'));
                (Lin is 1, Col is Cidade - 1, elemento_matriz(Tab, Lin, Col, 'b'));
                (Lin is 1, Col is Cidade, elemento_matriz(Tab, Lin, Col, 'b'));
                (Lin is 1, Col is Cidade + 1, elemento_matriz(Tab, Lin, Col, 'b'))
        ),
        executa_jogada(2, 2, Lin, Col, Cidade, 0, Tab, NovoTab, 1). 

desloca(2, 1, Lin, Col, Tab, TabFinal):- %esquerda jogador2
                Lin > 1,
        NovaCol is Col - 1,
        executa_jogada(2,1,Lin,Col,Lin,NovaCol,Tab,TabFinal, 1).

desloca(2, 2, Lin, Col, Tab, TabFinal):- %frente esquerda jogador2
                Lin > 1,
                Col > 1,
        NovaCol is Col - 1,
        NovaLin is Lin - 1,
        executa_jogada(2,1,Lin,Col,NovaLin,NovaCol,Tab,TabFinal, 1).
        
desloca(2, 3, Lin, Col, Tab, TabFinal):- %frente jogador2
                Col > 1,
        NovaLin is Lin - 1,
        executa_jogada(2,1,Lin,Col,NovaLin,Col,Tab,TabFinal, 1).
        
desloca(2, 4, Lin, Col, Tab, TabFinal):- %frente direita jogador2
                tamanho_tabuleiro(Tamanho),
                Col > 1,
                Lin < Tamanho,
        NovaCol is Col + 1,
        NovaLin is Lin - 1,
        executa_jogada(2,1,Lin,Col,NovaLin,NovaCol,Tab,TabFinal, 1).
        
desloca(2, 5, Lin, Col, Tab, TabFinal):- %direita jogador2
                tamanho_tabuleiro(Tamanho),
                Lin < Tamanho,
        NovaCol is Col + 1,
        executa_jogada(2,1,Lin,Col,Lin,NovaCol,Tab,TabFinal, 1).

desloca(1, 1, Lin, Col, Tab, TabFinal):- %esquerda jogador1
                tamanho_tabuleiro(Tamanho),
                Lin < Tamanho,
        NovaCol is Col + 1,
        executa_jogada(1,1,Lin,Col,Lin,NovaCol,Tab,TabFinal, 1).

desloca(1, 2, Lin, Col, Tab, TabFinal):- %frente esquerda jogador1
                tamanho_tabuleiro(Tamanho),
                Lin < Tamanho,
                Col < Tamanho,
        NovaCol is Col + 1,
        NovaLin is Lin + 1,
        executa_jogada(1,1,Lin,Col,NovaLin,NovaCol,Tab,TabFinal, 1).
        
desloca(1, 3, Lin, Col, Tab, TabFinal):- %frente jogador1
                tamanho_tabuleiro(Tamanho),
                Lin < Tamanho,
        NovaLin is Lin + 1,
        executa_jogada(1,1,Lin,Col,NovaLin,Col,Tab,TabFinal, 1).
        
desloca(1, 4, Lin, Col, Tab, TabFinal):- %frente direita jogador1
                tamanho_tabuleiro(Tamanho),
                Lin < Tamanho,
                Col > 1,
        NovaCol is Col - 1,
        NovaLin is Lin + 1,
        executa_jogada(1,1,Lin,Col,NovaLin,NovaCol,Tab,TabFinal, 1).
        
desloca(1, 5, Lin, Col, Tab, TabFinal):- %direita jogador1
                Col > 1,
        NovaCol is Col - 1,
        executa_jogada(1,1,Lin,Col,Lin,NovaCol,Tab,TabFinal, 1).
        
%Nivel de teste
aiLevel0Jog2(Tab,TabFinal):- repeat, tamanho_tabuleiro(Tamanho), S is Tamanho-1 ,
        random(0, S, Col), random(0, S, Lin), LinhaSeguinte is Lin-1, elemento_matriz(Tab, Lin, Col, PecaAleatoria), PecaAleatoria = 'r',
        elemento_matriz(Tab, LinhaSeguinte, Col, PecaFrente),
        (PecaFrente = 'x' -> elemento(Tab, Linha, L), altera_elemento_lista(L, Col, 'x', L2),
        altera_elemento_lista(Tab, Linha, L2, Tab2), elemento(Tab2, LinhaSeguinte, LI), altera_elemento_lista(LI, Col, 'b', L2),
        altera_elemento_lista(Tab2, LinhaSeguinte, L2, TabFinal)).

%IA para o nivel 2, para o jogador 1 (vermelho)  
aiLevel2Jog1(Tab,TabFinal):-
        (adjacente_cidade(1, Tab, TabFinal);
        (repeat, tamanho_tabuleiro(Tamanho), T is Tamanho+1, %set_random(seed(100000)),
        random(1, T, Col), random(1, T, Lin),
        LT is Lin-1, CT is Col-1, elemento_matriz(Tab, CT, LT, Peca),
        Peca\= 'b', Peca \= 'x', Peca = 'r',
        LinhaSeguinte is LT+1, ColSeguinte is CT+1, ColAnterior is CT-1,
        elemento_matriz(Tab, CT, LinhaSeguinte, PecaFrente),
        elemento_matriz(Tab, ColAnterior, LinhaSeguinte, PecaFrenteDir),
        elemento_matriz(Tab, ColSeguinte, LinhaSeguinte, PecaFrenteEsq),
        elemento_matriz(Tab, ColSeguinte, LT, PecaEsq),
        elemento_matriz(Tab, ColAnterior, LT, PecaDir),
        (PecaFrente = 't2' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, CT, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (PecaFrenteDir = 't2' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, ColAnterior, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (PecaFrenteEsq = 't2' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, ColSeguinte, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (PecaEsq = 't2' -> elemento(Tab, LT, LI),
        altera_elemento_lista(LI, ColSeguinte, 'x', L2), altera_elemento_lista(Tab, LT, L2, TabFinal);
        (PecaDir = 't2' -> elemento(Tab, LT, LI),
        altera_elemento_lista(LI, ColAnterior, 'x', L2), altera_elemento_lista(Tab, LT, L2, TabFinal);
        random(1,9,TipoMov),
        (TipoMov = 1, PecaFrente = 'b' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, CT, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (TipoMov = 2,PecaFrenteDir = 'b' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, ColAnterior, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (TipoMov = 3,PecaFrenteEsq = 'b' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, ColSeguinte, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (TipoMov = 4,PecaEsq = 'b' -> elemento(Tab, LT, LI),
        altera_elemento_lista(LI, ColSeguinte, 'x', L2), altera_elemento_lista(Tab, LT, L2, TabFinal);
        (TipoMov = 5,PecaDir = 'b' -> elemento(Tab, LT, LI),
        altera_elemento_lista(LI, ColAnterior, 'x', L2), altera_elemento_lista(Tab, LT, L2, TabFinal);
        (TipoMov = 6, PecaFrente = 'x' -> elemento(Tab, LT, L), altera_elemento_lista(L, CT, 'x', L2),
        altera_elemento_lista(Tab, LT, L2, Tab2), elemento(Tab2, LinhaSeguinte, LI), altera_elemento_lista(LI, CT, 'r', L3),
        altera_elemento_lista(Tab2, LinhaSeguinte, L3, TabFinal);
        (TipoMov = 7, PecaFrenteDir = 'x' -> elemento(Tab, LT, L), altera_elemento_lista(L, CT, 'x', L2),
        altera_elemento_lista(Tab, LT, L2, Tab2),elemento(Tab2, LinhaSeguinte, LI),altera_elemento_lista(LI, ColAnterior, 'r', L3),
        altera_elemento_lista(Tab2, LinhaSeguinte, L3, TabFinal);
        (TipoMov = 8, PecaFrenteEsq = 'x' -> elemento(Tab, LT, L), altera_elemento_lista(L, CT, 'x', L2),
        altera_elemento_lista(Tab, LT, L2, Tab2), elemento(Tab2, LinhaSeguinte, LI),altera_elemento_lista(LI, ColSeguinte, 'r', L3),
        altera_elemento_lista(Tab2, LinhaSeguinte, L3, TabFinal))))))))))))); fail))).
 
%IA para o nivel 2, para o jogador 2 (preto)   
aiLevel2Jog2(Tab,TabFinal):-
        (adjacente_cidade(2, Tab, TabFinal);
        (repeat, tamanho_tabuleiro(Tamanho), T is Tamanho+1, %set_random(seed(100000)),
        random(1, T, Col), random(1, T, Lin),
        LT is Lin-1, CT is Col-1, elemento_matriz(Tab, CT, LT, Peca),
        Peca\= 'r', Peca \= 'x', Peca = 'b',
        LinhaSeguinte is LT-1, ColSeguinte is CT+1, ColAnterior is CT-1,
        elemento_matriz(Tab, CT, LinhaSeguinte, PecaFrente),
        elemento_matriz(Tab, ColAnterior, LinhaSeguinte, PecaFrenteEsq),
        elemento_matriz(Tab, ColSeguinte, LinhaSeguinte, PecaFrenteDir),
        elemento_matriz(Tab, ColSeguinte, LT, PecaDir),
        elemento_matriz(Tab, ColAnterior, LT, PecaEsq),
        (PecaFrente = 't1' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, CT, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (PecaFrenteEsq = 't1' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, ColAnterior, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (PecaFrenteDir = 't1' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, ColSeguinte, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (PecaDir = 't1' -> elemento(Tab, LT, LI),
        altera_elemento_lista(LI, ColSeguinte, 'x', L2), altera_elemento_lista(Tab, LT, L2, TabFinal);
        (PecaEsq = 't1' -> elemento(Tab, LT, LI),
        altera_elemento_lista(LI, ColAnterior, 'x', L2), altera_elemento_lista(Tab, LT, L2, TabFinal);
        random(1,9,TipoMov),
        (TipoMov = 1, PecaFrente = 'r' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, CT, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (TipoMov = 2,PecaFrenteEsq = 'r' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, ColAnterior, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (TipoMov = 3,PecaFrenteDir = 'r' -> elemento(Tab, LinhaSeguinte, LI),
        altera_elemento_lista(LI, ColSeguinte, 'x', L2), altera_elemento_lista(Tab, LinhaSeguinte, L2, TabFinal);
        (TipoMov = 4,PecaDir = 'r' -> elemento(Tab, LT, LI),
        altera_elemento_lista(LI, ColSeguinte, 'x', L2), altera_elemento_lista(Tab, LT, L2, TabFinal);
        (TipoMov = 5,PecaEsq = 'r' -> elemento(Tab, LT, LI),
        altera_elemento_lista(LI, ColAnterior, 'x', L2), altera_elemento_lista(Tab, LT, L2, TabFinal);
        (TipoMov = 6, PecaFrente = 'x' -> elemento(Tab, LT, L), altera_elemento_lista(L, CT, 'x', L2),
        altera_elemento_lista(Tab, LT, L2, Tab2), elemento(Tab2, LinhaSeguinte, LI), altera_elemento_lista(LI, CT, 'b', L3),
        altera_elemento_lista(Tab2, LinhaSeguinte, L3, TabFinal);
        (TipoMov = 7, PecaFrenteEsq = 'x' -> elemento(Tab, LT, L), altera_elemento_lista(L, CT, 'x', L2),
        altera_elemento_lista(Tab, LT, L2, Tab2),elemento(Tab2, LinhaSeguinte, LI),altera_elemento_lista(LI, ColAnterior, 'b', L3),
        altera_elemento_lista(Tab2, LinhaSeguinte, L3, TabFinal);
        (TipoMov = 8, PecaFrenteDir = 'x' -> elemento(Tab, LT, L), altera_elemento_lista(L, CT, 'x', L2),
        altera_elemento_lista(Tab, LT, L2, Tab2), elemento(Tab2, LinhaSeguinte, LI),altera_elemento_lista(LI, ColSeguinte, 'b', L3),
        altera_elemento_lista(Tab2, LinhaSeguinte, L3, TabFinal))))))))))))); fail))).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INPUTS E OUTPUTS DO TIPO DE JOGO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
tipo_jogo(X, Dif, Dif2):-
        write('1 - Humano vs Humano'), nl,
        write('2 - Humano vs Computador'), nl, 
        write('3 - Computador vs Computador'), nl,
        le_e_valida_numero(1,3,N),
        define_tipo_jogo(N, Jogador1, Jogador2),
        assert(jogador(1, Jogador1)),
        assert(jogador(2, Jogador2)),
        X = N,
                
        (N = 3 -> write('Escolha nivel de dificuldade do computador: '), nl,
        write('1 - Aleatorio'), nl, write('2 - Captura se possivel'), nl,  write('Jogador A:'), nl,
        le_e_valida_numero(1,2,Dif), write('Jogador B:'), nl,
        le_e_valida_numero(1,2,Dif2);
                        (N = 2 -> write('Escolha nivel de dificuldade do computador: '), nl,
        write('1 - Aleatorio'), nl, write('2 - Captura se possivel'), nl, write('Jogador B:'), nl,
        le_e_valida_numero(1,2,Dif2), Dif is 0;
                        Dif is 0, Dif2 is 0)).

define_tipo_jogo(1, humano, humano).
define_tipo_jogo(2, humano, computador).
define_tipo_jogo(3, computador, computador).

jogadorInicial(1).
mudaJogador(1,Proximo):- Proximo is 2.
mudaJogador(2,Proximo):- Proximo is 1.
        
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INPUTS E OUTPUTS DE UMA JOGADA %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

processa_tipo_jogada(Jogador,Tabuleiro,NovoTab):-
        Tabuleiro=NovoTab.
        %tamanho_tabuleiro(T),
       % write('Mover peÃ§a - 1'), nl,
      %  write('Capturar peÃ§a - 2'), nl,
       % le_e_valida_numero(1, 2, N), nl,
       % write('PeÃ§a inicial'), nl,
        %write('Linha: '),
        %le_e_valida_numero(1, T, X),
        %write('Coluna: '),
        %le_e_valida_numero(1, T, Y),
        %write('Linha Alvo:'),nl,
        % le_e_valida_numero(1, T, XFinal),
        %write('Coluna Alvo: '),nl,
        %le_e_valida_numero(1, T, YFinal),
        %executa_jogada(Jogador,N,X,Y,XFinal,YFinal,Tabuleiro,NovoTab, 0).

processa_tipo_jogada_ai(1,Tab,TabFinal):- 
        aiLevel1Jog1(Tab,TabFinal).
        
processa_tipo_jogada_ai(2,Tab,TabFinal):- 
        aiLevel1Jog2(Tab,TabFinal).
                
tipo_invalido(Jogador, Tabuleiro, NovoTab):-
        write('Tipo de jogada incorrecto.'), nl,
        processa_tipo_jogada_ai(Jogador, Tabuleiro, NovoTab).
                
jogada_invalida(Jogador,Tabuleiro,NovoTab, 0):-
        write('Jogada invalida.'), nl,
        processa_tipo_jogada(Jogador,Tabuleiro,NovoTab).
                
jogada_invalida(Jogador,Tabuleiro,NovoTab, 1):-
        processa_tipo_jogada_ai(Jogador,Tabuleiro,NovoTab).
        
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LOGICA DE JOGO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%executa_jogada(Jogador, TipoJogada, Linha, Coluna, NovaLinha, NovaColuna, Tabuleiro, TabuleiroFInal, AI.

executa_jogada(1,1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        write('jogador 1 - '),
        LinhaCanhao is Linha+1,
        LinhaRetirada is Linha-1,
        LinhaRetirada2 is Linha-2,
        LinhaX is Linha+1, LinhaX2 is Linha-2, LinhaX3 is Linha-3, Col1 is Coluna-1,
        elemento_matriz(Tab, Col1, LinhaRetirada, Peca),
        write(Peca),nl,
        (Peca \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (elemento_matriz(Tab, Col1, LinhaX2, E1), elemento_matriz(Tab, Col1,LinhaX3, E2),
        E1 = 'r', E2='r',NovaLinha = LinhaX, Coluna = NovaColuna, Linha>2 -> moveCanhaoFrenteJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (LinhaX = NovaLinha -> moveSoldadoFrenteJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (LinhaRetirada2 = NovaLinha -> retiradaSoldadoJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (NovaLinha = LinhaRetirada -> retiradaCanhaoJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        %jogada_invalida(1,Tab,TabFinal, AI)
         TabFinal=Tab
        ))))).
        
executa_jogada(1,2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        LinhaCanhao is Linha+3,
        LinhaCanhao2 is Linha+2,
        (NovaLinha = LinhaCanhao -> capturaCanhaoJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (NovaLinha = LinhaCanhao2 -> capturaCanhaoJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        capturaSoldadoJog1(Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI))).
        
executa_jogada(2,1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        LinhaCanhao is Linha-1,
        LinhaRetirada is Linha+1, Col1 is Coluna-1,
        LinhaRetirada2 is Linha+2,
        elemento_matriz(Tab, Col1, LinhaCanhao, Peca),
        (Peca \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (elemento_matriz(Tab, Col1, Linha, E2), elemento_matriz(Tab, Col1, LinhaRetirada, E3),
        E3 = 'b', E2 = 'b',NovaLinha = LinhaCanhao, Coluna=NovaColuna -> moveCanhaoFrenteJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (NovaLinha = LinhaCanhao -> moveSoldadoFrenteJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (LinhaRetirada2 = NovaLinha -> retiradaSoldadoJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (NovaLinha = LinhaRetirada -> retiradaCanhaoJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        %jogada_invalida(2,Tab,TabFinal, AI)
         TabFinal=Tab
        ))))).
        
executa_jogada(2,2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        LinhaCanhao is Linha-3,
        LinhaCanhao2 is Linha-2,
        (NovaLinha = LinhaCanhao -> capturaCanhaoJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (NovaLinha = LinhaCanhao2 -> capturaCanhaoJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        capturaSoldadoJog2(Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI))).

moveSoldadoFrenteJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        (NovaColuna > Coluna+1 -> jogada_invalida(1,Tab,TabFinal, AI);
        (NovaColuna < Coluna-1 -> jogada_invalida(1,Tab,TabFinal, AI);
        (NovaLinha > Linha+1 -> jogada_invalida(1,Tab,TabFinal, AI);
        trocaPeca(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI)))).

retiradaSoldadoJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        TempLinha is NovaLinha+2,
        LinhaAnterior is Linha-1,
        LinhaTras is Linha-2,
        Col1 is Coluna -1,  ColEsq is Coluna-2,
        elemento_matriz(Tab, ColEsq, LinhaAnterior, PecaEsquerda),
        elemento_matriz(Tab, Coluna, LinhaAnterior, PecaDireita),
        elemento_matriz(Tab, ColEsq, Linha, PecaEsqFrente),
        elemento_matriz(Tab, Coluna, Linha, PecaDirFrente),
        elemento_matriz(Tab, Col1, Linha, PecaFrente),
        (TempLinha \= Linha; PecaEsquerda \= 'b', PecaDireita \= 'b', 
        PecaEsqFrente \= 'b', PecaDirFrente \= 'b',
        PecaFrente \= 'b' -> jogada_invalida(1,Tab,TabFinal, AI);
        (NovaColuna > Coluna -> retiradaSoldadoJog1Dir(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (NovaColuna < Coluna -> retiradaSoldadoJog1Esq(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        elemento_matriz(Tab, LinhaTras, Col1, PecaInt),
        (PecaInt \= 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        trocaPeca(1,Linha,Coluna,NovaLinha,Coluna,Tab,TabFinal, AI))))).
        
retiradaSoldadoJog1Dir(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        TempColuna is NovaColuna-2,
        (TempColuna \= Coluna -> jogada_invalida(1,Tab,TabFinal, AI);
        Linha1 is Linha-2,
        ColVer is Coluna-2,
        elemento_matriz(Tab, ColVer, Linha1, PecaInt),
        (PecaInt \= 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        trocaPeca(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI))).
        
retiradaSoldadoJog1Esq(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        TempColuna is NovaColuna+2,
        (TempColuna \= Coluna -> jogada_invalida(1,Tab,TabFinal, AI);
        Linha1 is Linha-2,
        elemento_matriz(Tab, Coluna, Linha1, PecaInt),
        (PecaInt \= 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        trocaPeca(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI))).
      
moveSoldadoFrenteJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        (NovaColuna > Coluna+1 -> jogada_invalida(2,Tab,TabFinal, AI);
        (NovaColuna < Coluna-1 -> jogada_invalida(2,Tab,TabFinal, AI);
        (NovaLinha < Linha-1 -> jogada_invalida(2,Tab,TabFinal, AI);
        trocaPeca(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI)))).

retiradaSoldadoJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        TempLinha is NovaLinha-2,
        LinhaSeguinte is Linha+1,
        Linha2 is Linha-1,
        LinhaFrente is Linha-2,
        ColEsq is Coluna-2, Col1 is Coluna-1,
        elemento_matriz(Tab, ColEsq, Linha2,PecaEsq),
        elemento_matriz(Tab, Coluna,Linha2, PecaDir),
        elemento_matriz(Tab, ColEsq, LinhaFrente, PecaFrenteEsq),
        elemento_matriz(Tab, Col1, LinhaFrente, PecaFrente),
        elemento_matriz(Tab, Coluna, LinhaFrente, PecaFrenteDir),
        (TempLinha \= Linha; PecaEsq \= 'r', PecaDir \= 'r', PecaFrenteEsq \= 'r', PecaFrenteDir \= 'r',
         PecaFrente \= 'r'  -> jogada_invalida(2,Tab,TabFinal, AI);
        (NovaColuna > Coluna -> retiradaSoldadoJog2Dir(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        (NovaColuna < Coluna -> retiradaSoldadoJog2Esq(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI);
        elemento_matriz(Tab,Col1,Linha,PecaInt),
        (PecaInt \= 'x' -> jogada_invalida(2,Tab,TabFinal, AI);
        trocaPeca(2,Linha,Coluna,NovaLinha,Coluna,Tab,TabFinal, AI))))).
        
retiradaSoldadoJog2Dir(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        TempColuna is NovaColuna-2,
        (TempColuna \= Coluna -> jogada_invalida(2,Tab,TabFinal, AI);
        elemento_matriz(Tab, Coluna, Linha, PecaInt),
        (PecaInt \= 'x' -> jogada_invalida(2,Tab,TabFinal, AI);
        trocaPeca(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI))).
        
retiradaSoldadoJog2Esq(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        TempColuna is NovaColuna+2,
        (TempColuna \= Coluna -> jogada_invalida(2,Tab,TabFinal, AI);
        Col is Coluna-2,
        elemento_matriz(Tab, Col, Linha, PecaInt),
        (PecaInt \= 'x' -> jogada_invalida(2,Tab,TabFinal, AI);
        trocaPeca(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI))).

capturaSoldadoJog1(Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        NCol1 is NovaColuna-1, NLinha is NovaLinha-1,
        Col is Coluna-1, Lin is Linha-1,
        elemento_matriz(Tab, NCol1, NLinha, Alvo),
        elemento_matriz(Tab, Col, Lin,Origem),
        MaxLinha is Linha+1,
        MaxColuna is Coluna+1,
        MinColuna is Coluna-1,
        (NovaLinha > MaxLinha -> jogada_invalida(1,Tab,TabFinal, AI);
        (NovaLinha < Linha -> jogada_invalida(1,Tab,TabFinal, AI);
        (NovaColuna > MaxColuna -> jogada_invalida(1,Tab,TabFinal, AI);
        (NovaColuna < MinColuna -> jogada_invalida(1,Tab,TabFinal, AI);
        (Origem \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (Alvo = 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        (Alvo = 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (Alvo = 't1' -> jogada_invalida(1,Tab,TabFinal, AI);
        elemento(Tab, NLinha, L),
        altera_elemento_lista(L, NCol1, 'x', L2),
        TabTemp = Tab,
        altera_elemento_lista(TabTemp, NLinha, L2, TabFinal))))))))).
        
capturaSoldadoJog2(Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        NCol1 is NovaColuna-1, NLin is NovaLinha-1,
        Col1 is Coluna-1, Lin1 is Linha-1,
        elemento_matriz(Tab, NCol1, NLin, Alvo),
        elemento_matriz(Tab, Col1, Lin1,Origem),
        MinLinha is Linha-1,
        MaxColuna is Coluna+1,
        MinColuna is Coluna-1,
        (NovaLinha < MinLinha -> jogada_invalida(2,Tab,TabFinal, AI);
        (NovaLinha > Linha -> jogada_invalida(2,Tab,TabFinal, AI);
        (NovaColuna > MaxColuna -> jogada_invalida(2,Tab,TabFinal, AI);
        (NovaColuna < MinColuna -> jogada_invalida(2,Tab,TabFinal, AI);
        (Origem \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (Alvo = 'x' -> jogada_invalida(2,Tab,TabFinal, AI);
        (Alvo = 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (Alvo = 't2' -> jogada_invalida(2,Tab,TabFinal, AI);
        elemento(Tab,NLin,L),
        altera_elemento_lista(L, NCol1,'x',L2),
        TabTemp=Tab,
        altera_elemento_lista(TabTemp, NLin, L2, TabFinal))))))))).

trocaPeca(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        Col1 is Coluna-1, Linha1 is Linha-1, NCol1 is NovaColuna-1,
        NLin is NovaLinha-1,
        elemento_matriz(Tab, Col1, Linha1, PecaOriginal),
        elemento_matriz(Tab,NCol1,NLin,PecaDestino),
        (PecaOriginal \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (PecaDestino \= 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        elemento(Tab, Linha1, L), altera_elemento_lista(L, Col1, 'x', L2),
        TabTemp = Tab,
        altera_elemento_lista(TabTemp, Linha1, L2, TabNovo),
        elemento(TabNovo, NLin, L3), altera_elemento_lista(L3, NCol1, 'r', L4),
        TabTemp2 = TabNovo,
        altera_elemento_lista(TabTemp2, NLin, L4, TabFinal))).
        
trocaPeca(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        Col1 is Coluna-1, Linha1 is Linha-1,
        NCol is NovaColuna-1, NLin is NovaLinha-1,
        elemento_matriz(Tab, Col1, Linha1, PecaOriginal),
        elemento_matriz(Tab,NCol,NLin,PecaDestino),
        (PecaOriginal \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (PecaDestino \= 'x' -> jogada_invalida(2,Tab,TabFinal, AI);
        elemento(Tab, Linha1, L), altera_elemento_lista(L, Col1, 'x', L2),
        TabTemp=Tab, altera_elemento_lista(TabTemp, Linha1, L2, TabTemp2),
        elemento(TabTemp2, NLin, L3), altera_elemento_lista(L3, NCol,'b', L4),
        TabNovo = TabTemp2, altera_elemento_lista(TabNovo, NLin, L4, TabFinal))).
   
capturaCanhaoJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        Dif is NovaLinha - Linha,
        LinhaAnterior is Linha-1,
        UltimaLinha is Linha-2,
        NCol is NovaColuna-1, NLin is NovaLinha-1,
        Col is Coluna-1, Lin is Linha-1,
        IntLin is NovaLinha-3,
        IntLin2 is NovaLinha-2,
        elemento_matriz(Tab, NCol, IntLin, IntPeca1),
        elemento_matriz(Tab, NCol, IntLin2, IntPeca2),
        elemento_matriz(Tab, NCol, NLin, Alvo),
        elemento_matriz(Tab, Col, Lin,Origem),
        LinhaAnt1 is LinhaAnterior-1, UltLin1 is UltimaLinha-1,
        elemento_matriz(Tab,Col,LinhaAnt1,PecaAnterior),
        elemento_matriz(Tab,Col,UltLin1,UltimaPeca),
        (Dif = 3, IntPeca1 \= 'x' -> jogada_invalida(1, Tab, TabFinal, AI);
        (IntPeca2 \= 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        (NovaColuna \= Coluna -> jogada_invalida(1,Tab,TabFinal, AI);
        (Origem \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (Alvo = 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        (Alvo = 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (Alvo = 't1' -> jogada_invalida(1,Tab,TabFinal, AI);
        (PecaAnterior \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (UltimaPeca \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        elemento(Tab, NLin,L), altera_elemento_lista(L, NCol,'x', L2),
        TabTemp=Tab, altera_elemento_lista(TabTemp, NLin, L2, TabFinal)))))))))).
        
capturaCanhaoJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        Dif is Linha-NovaLinha,
        LinhaAnterior is Linha+1,
        UltimaLinha is Linha+2,
        NCol is NovaColuna-1, NLin is NovaLinha-1,
        Col is Coluna-1, Lin is Linha-1,
        Dist1 is Linha-2, Dist2 is Linha-3,
        LinAnt1 is LinhaAnterior-1, UltLin1 is UltimaLinha-1,
        elemento_matriz(Tab, NCol, NLin, Alvo),
        elemento_matriz(Tab, Col, Lin,Origem),
        elemento_matriz(Tab,Col,LinAnt1,PecaAnterior),
        elemento_matriz(Tab,Col,UltLin1,UltimaPeca),
        elemento_matriz(Tab,Col,Dist1, IntPeca2),
        elemento_matriz(Tab,Col,Dist2, IntPeca1),
        (Dif = 3, IntPeca1 \= 'x' -> jogada_invalida(1, Tab, TabFinal, AI);
        (IntPeca2 \= 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        (NovaColuna \= Coluna -> jogada_invalida(2,Tab,TabFinal, AI);
        (Origem \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (Alvo = 'x' -> jogada_invalida(2,Tab,TabFinal, AI);
        (Alvo = 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (Alvo = 't2' -> jogada_invalida(2,Tab,TabFinal, AI);
        (PecaAnterior \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (UltimaPeca \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        elemento(Tab, NLin, L), altera_elemento_lista(L, NCol, 'x', L2),
        TabTemp = Tab, altera_elemento_lista(TabTemp, NLin, L2, TabFinal)))))))))).
                
moveCanhaoFrenteJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        LinhaAnterior is Linha-1,
        UltimaLinha is Linha-2,
        ProxLinha is Linha+1,
        LA is LinhaAnterior-1, UltL is UltimaLinha-1,
        Lin is Linha-1, Col is Coluna-1, NLin is NovaLinha-1,
        elemento_matriz(Tab,Col, NLin, Alvo),
        elemento_matriz(Tab, Col, Lin,Origem),
        elemento_matriz(Tab,Col,LA,PecaAnterior),
        elemento_matriz(Tab,Col,UltL,UltimaPeca),
        (NovaColuna \= Coluna -> jogada_invalida(1,Tab,TabFinal, AI);
        (Origem \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (Alvo \= 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        (PecaAnterior \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (UltimaPeca \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (ProxLinha \= NovaLinha -> jogada_invalida(1,Tab,TabFinal, AI);
        elemento(Tab, UltL, L), altera_elemento_lista(L, Col, 'x', L2),
        TabTemp=Tab, altera_elemento_lista(TabTemp, UltL, L2, Tab2),
        elemento(Tab2, NLin, L3), altera_elemento_lista(L3, Col, 'r', L4),
        Tab3 = Tab2, altera_elemento_lista(Tab3, NLin, L4, TabFinal))))))).
             
moveCanhaoFrenteJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        LinhaAnterior is Linha+1,
        UltimaLinha is Linha+2,
        ProxLinha is Linha-1,
        LA is LinhaAnterior-1, UL is UltimaLinha-1,
        Col is Coluna-1, Lin is Linha-1, NLin is NovaLinha-1,
        elemento_matriz(Tab,Col, NLin, Alvo),
        elemento_matriz(Tab, Col, Lin,Origem),
        elemento_matriz(Tab,Col,LA,PecaAnterior),
        elemento_matriz(Tab,Col,UL,UltimaPeca),
        (ProxLinha \= NovaLinha ->jogada_invalida(2,Tab,TabFinal, AI);
        (NovaColuna \= Coluna -> jogada_invalida(2,Tab,TabFinal, AI);
        (Origem \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (Alvo \= 'x' -> jogada_invalida(2,Tab,TabFinal, AI);
        (PecaAnterior \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (UltimaPeca \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        elemento(Tab, UL, L), altera_elemento_lista(L, Col, 'x', L2),
        TabTemp = Tab, altera_elemento_lista(TabTemp, UL, L2, Tab2),
        elemento(Tab2, NLin, L3), altera_elemento_lista(L3, Col, 'b', L4),
        Tab3 = Tab2, altera_elemento_lista(Tab3, NLin, L4, TabFinal))))))).
        
retiradaCanhaoJog1(1,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-
        ProxLinha is Linha-3,
        LinhaSeguinte is Linha-2,
        PL is ProxLinha-1, LS is LinhaSeguinte-1,
        Lin is Linha-1, NLin is NovaLinha-1,
        Col is Coluna-1,
        elemento_matriz(Tab,Col,PL,ProxCel),
        elemento_matriz(Tab,Col,LS,CelSeguinte),
        elemento_matriz(Tab,Col, NLin, Alvo),
        elemento_matriz(Tab, Col, Lin,Origem),
        (NovaColuna \= Coluna -> jogada_invalida(2,Tab,TabFinal, AI);
        (Origem \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (Alvo \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        (ProxCel \= 'x' -> jogada_invalida(1,Tab,TabFinal, AI);
        (CelSeguinte \= 'r' -> jogada_invalida(1,Tab,TabFinal, AI);
        elemento(Tab, Lin, L1), altera_elemento_lista(L1, Col, 'x', L2),
        TabTemp = Tab, altera_elemento_lista(TabTemp, Lin, L2, Tab2),
        elemento(Tab2, PL, L3), altera_elemento_lista(L3, Col, 'r', L4),
        Tab3 = Tab2, altera_elemento_lista(Tab3, PL, L4, TabFinal)))))).
           
retiradaCanhaoJog2(2,Linha,Coluna,NovaLinha,NovaColuna,Tab,TabFinal, AI):-       
        ProxLinha is Linha+3,
        LinhaSeguinte is Linha+2,
        PL is ProxLinha-1, LS is LinhaSeguinte-1, Col is Coluna-1,
        NLin is NovaLinha-1, Lin is Linha-1,
        elemento_matriz(Tab,Col,PL,ProxCel),
        elemento_matriz(Tab,Col,LS,CelSeguinte),
        elemento_matriz(Tab,Col, NLin, Alvo),
        elemento_matriz(Tab, Col, Linha,Origem),
        (NovaColuna \= Coluna -> jogada_invalida(2,Tab,TabFinal, AI);
        (Origem \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (Alvo \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        (ProxCel \= 'x' -> jogada_invalida(2,Tab,TabFinal, AI);
        (CelSeguinte \= 'b' -> jogada_invalida(2,Tab,TabFinal, AI);
        elemento(Tab, Lin, L1), altera_elemento_lista(L1, Col, 'x', L2),
        Tab2 = Tab, altera_elemento_lista(Tab2, Lin, L2, Tab3),
        elemento(Tab3, PL, L3), altera_elemento_lista(L3, Col, 'b', L4),
        Tab4 = Tab3, altera_elemento_lista(Tab4, PL, L4, TabFinal)))))).
                
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% VERIFICACAO DE FIM DE JOGO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


verificaFim(Tab,Res):-
 (verificaJog2Vit(Tab,ResultadoA), ResultadoA = 1 -> Res=2;
        (verificaJog1Vit(Tab,ResultadoB),ResultadoB = 1 -> Res=1; Res=(-1))).

%Verifica se o jogo terminou
verificaFim(Tipo, Dif, Dif2, Jogador, Tab):-
        (verificaJog2Vit(Tab,ResultadoA), ResultadoA = 1 -> write('O Jogador B venceu o jogo!');
        (verificaJog1Vit(Tab,ResultadoB),ResultadoB = 1 -> write('O Jogador A venceu o jogo!');
        jogo(Tipo, Dif, Dif2, Jogador,Tab))).

%Verifica se o jogador 2 ganhou
verificaJog2Vit(Tab,ResultadoA):-
        rep_cidade1(T),
        elemento(Tab, 0, Linha1),!,
        write('search: '),write(T),write(' on line: '),write(Linha1),nl,
        (membro(T, Linha1) -> ResultadoA is 0;
        ResultadoA is 1).

%Verifica se o jogador 1 ganhou
verificaJog1Vit(Tab,ResultadoB):- tamanho_tabuleiro(Tamanho),
        rep_cidade2(T),
        S1 is Tamanho-1,
        elemento(Tab, S1, UltLinha),!,
        write('search: '),write(T),write(' on line: '),write(UltLinha),nl,
        (membro(T, UltLinha) -> ResultadoB is 0;
        ResultadoB is 1).
