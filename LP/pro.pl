% Eduardo Morgado IST190061


:- consult(codigo_comum).

%----------------------------------------------------------------------%
%                                                                      %
%       Predicados desenvolvidos para a implementacao do projeto.      %
%                                                                      %
%----------------------------------------------------------------------%

%----------------------------------------------------------------------%
% conta_variaveis(L,Cont):
% Cont corresponde ao numero de variaveis na linha L.
%----------------------------------------------------------------------%


conta_variaveis(L, Cont) :-
    include(var, L, L_vars),
    length(L_vars, Cont).

%----------------------------------------------------------------------%
% aplica_R1_triplo_aux(([A,B,C],N_Triplo)):
% N_Triplo corresponde a' aplicacao da regra 1 a tres posicoes 
% seguidas,A,B,C, sendo que apenas uma dela e variavel.
%----------------------------------------------------------------------%

aplica_R1_triplo_aux([A,B,C],N_Triplo):-
    var(A),B==C, Res is (B+1)mod(2),N_Triplo=[Res,B,C],!;
    var(B),A==C, Res is (C+1)mod(2),N_Triplo=[A,Res,C],!;
    var(C),A==B, Res is (A+1)mod(2),N_Triplo=[A,B,Res],!;
    N_Triplo=[A,B,C].

%----------------------------------------------------------------------%
% aplica_R1_triplo([A,B,C],N_Triplo):
% N_Triplo corresponde a' aplicacao da regra 1 a tres posicoes
% seguidas,A,B,C, sendo que podem haver entre 0 a 3 variaveis
%----------------------------------------------------------------------%

aplica_R1_triplo([A,B,C],N_Triplo):-
    conta_variaveis([A,B,C],Nelem),
    ((Nelem==3 , N_Triplo=[A,B,C]),!;
    (Nelem==2,  N_Triplo=[A,B,C]),!;
    (Nelem==1,  aplica_R1_triplo_aux([A,B,C],N_Triplo)),!;
    (Nelem==0, (A\==B; B\==C) , N_Triplo=[A,B,C])).


%----------------------------------------------------------------------%
% aplica_R1_fila_aux([A,B,C], Res):
% Res corresponde ao resultado de aplicar a regra R1 a todos 
% trios de numeros [A,B,C].
%----------------------------------------------------------------------%

aplica_R1_fila_aux([A,B,C], Res) :-
    aplica_R1_triplo([A,B,C], Res).

aplica_R1_fila_aux([A, B, C | R], Res) :-
    aplica_R1_triplo([A, B, C], [NX, NY, NZ]),
    aplica_R1_fila_aux([NY, NZ | R], Res_Temp),
    Res = [NX | Res_Temp].

%----------------------------------------------------------------------%
% aplica_R1_fila(Fila,Res):
% Res corresponde a' linha apos aplicar a regra R1 na Fila ate nao
% existirem mais alteracoes possiveis.
%----------------------------------------------------------------------%


aplica_R1_fila(Fila, Res) :-
    aplica_R1_fila_aux(Fila, Res_Temp),
    (Fila == Res_Temp, Res = Res_Temp,!;
    Fila\==Res_Temp, aplica_R1_fila(Res_Temp, Res)).



%----------------------------------------------------------------------%
% muda_R2_fila_aux([H|R],Aux,Ac,Res) :
% Res corresponde a' linha apos adicionar 1's e 0's
%
%----------------------------------------------------------------------%

muda_R2_fila_aux([H|R],Aux,Ac,Res) :-
    var(H),
    append(Ac,[Aux],Ac2),
    muda_R2_fila_aux(R,Aux,Ac2,Res);
    append(Ac,[H],Ac2),
    muda_R2_fila_aux(R,Aux,Ac2,Res).

muda_R2_fila_aux([],_,Ac,Res) :-
    Res=Ac.

%----------------------------------------------------------------------%
% aplica_R2_fila(Fila, N_Fila) :
% N_Fila corresponde a' linha apos aplicar R2 a Fila.
%----------------------------------------------------------------------%

aplica_R2_fila(Fila, N_Fila) :-
    length(Fila,N), HalfElem is N/2, 
    (findall(Zero,(member(Zero,Fila), Zero==0),Zeros),
    findall(One, (member(One,Fila), One==1), Ones)),
    (length(Zeros,X),
    length(Ones,Y)),
    (X==HalfElem, X\==Y, muda_R2_fila_aux(Fila,1,[],N_Fila),!;
    Y==HalfElem, X\==Y,  muda_R2_fila_aux(Fila,0,[],N_Fila),!;
    X=<HalfElem, Y=<HalfElem, N_Fila = Fila).

%----------------------------------------------------------------------%
% aplica_R1_R2_fila(Fila,N_Fila) :
% N_Fila corresponde a' linha correspondente a aplicar
% R1 e R2 a Fila.
%----------------------------------------------------------------------%

aplica_R1_R2_fila(Fila,N_Fila) :-
    aplica_R1_fila(Fila,Temp),
    aplica_R2_fila(Temp,N_Fila).
%----------------------------------------------------------------------%
% aplica_R1_R2_puzzle(Puz,N_Puz):
% N_Puz corresponde ao puzzle depois de aplicar R1 e R2
% a todas as linhas e colunas de Puz.
%----------------------------------------------------------------------%

aplica_R1_R2_puzzle(Puz,N_Puz) :-
    maplist(aplica_R1_R2_fila,Puz,Aux),
    mat_transposta(Aux,Aux2),
    maplist(aplica_R1_R2_fila,Aux2,Temp),
    mat_transposta(Temp,N_Puz).

%----------------------------------------------------------------------%
% inicializa(Puz,N_Puz):
% N_Puz resulta de aplicar R1 e R2 a todo o puzzle
% ate nao exisitirem mais mudancas
%----------------------------------------------------------------------%

inicializa_aux(Puz,N_Puz) :-
    aplica_R1_R2_puzzle(Puz,Aux),
    (Puz==Aux , N_Puz=Aux;
    inicializa_aux(Aux,N_Puz)).

inicializa(Puz,N_Puz):-
    inicializa_aux(Puz,N_Puz).

%----------------------------------------------------------------------%
% verifica_R3([A,B|Res]):
% Verifica se existe algum par de linhas que seja igual entre eles.
%----------------------------------------------------------------------%

verifica_R3([_]).

verifica_R3([A,B|Res]):-
    A==B, fail;
    A\==B, verifica_R3([B|Res]).


%----------------------------------------------------------------------%
% compara_linhas([A|L1],[B|L2],Linha,Contador,ListaAux,ListaFinal):-
% ListaFinal corresponde a uniao da ListaAux com todos os pontos
% que sejam diferentes nas duas linhas introduzidas.
%----------------------------------------------------------------------%

compara_linhas([],[],_,_,ListaAux,ListaFinal):-
    ListaFinal=ListaAux.

compara_linhas([A|L1],[B|L2],Linha,Contador,ListaAux,ListaFinal):-
    NovoContador is Contador+1,
    (A==B ,
    compara_linhas(L1,L2,Linha,NovoContador,ListaAux,ListaFinal),!;
    (append([(Linha,Contador)],ListaAux,NovaLista),
    (compara_linhas(L1,L2,Linha,NovoContador,NovaLista,ListaFinal)))).

%----------------------------------------------------------------------%
% compara_colunas([A|L1],[B|L2],Coluna,Contador,ListaAux,ListaFinal):-
% ListaFinal corresponde a uniao da ListaAux com todos os pontos
% que sejam diferentes nas duas linhas introduzidas.
%----------------------------------------------------------------------%

compara_colunas([],[],_,_,ListaAux,ListaFinal):-
    ListaFinal=ListaAux.

compara_colunas([A|L1],[B|L2],Coluna,Contador,ListaAux,ListaFinal):-
    NovoContador is Contador+1,
    (A==B ,
    compara_colunas(L1,L2,Coluna,NovoContador,ListaAux,ListaFinal),!;
    (append([(Contador,Coluna)],ListaAux,NovaLista),
    (compara_colunas(L1,L2,Coluna,NovoContador,NovaLista,ListaFinal)))).


%----------------------------------------------------------------------%
% propaga_posicoes_aux([(L,C)|Res], Puz,NovoPuz):
% NovoPuz corresponde a' aplicacao sucessiva de R1 e R2
% na lista de pontos introduzida ate nao existirem mais 
% alteracoes possiveis.
%----------------------------------------------------------------------%

propaga_posicoes_aux([],Puz,NovoPuz):-
    NovoPuz=Puz.

propaga_posicoes_aux([(L,C)|Res], Puz,NovoPuz) :-
    mat_transposta(Puz,Aux),
    mat_elementos_coluna(Aux,L,Linha),
    aplica_R1_R2_fila(Linha,LinhaRes),
    mat_muda_linha(Puz,L,LinhaRes,PuzAux),
    mat_elementos_coluna(PuzAux,C,Coluna),
    aplica_R1_R2_fila(Coluna,ColunaRes),
    mat_muda_coluna(PuzAux,C,ColunaRes,PuzTemp), 
    compara_linhas(Linha,LinhaRes,L,1,Res,NovaLista), 
    compara_colunas(Coluna,ColunaRes,C,1,NovaLista,NovaNovaLista),
    propaga_posicoes_aux(NovaNovaLista,PuzTemp,NovoPuz).   


propaga_posicoes([(L , C)|Res], Puz, N_Puz):-
    propaga_posicoes_aux([(L,C)|Res],Puz,N_Puz).

%----------------------------------------------------------------------%
% resolve(Puz,Sol) :
% Sol e' (um)a solucao do puz Puz.
%----------------------------------------------------------------------%

% resolve_final(Puz,(L,C),PuzAlterado):-
%    var(Cont),
%    (mat_muda_posicao(Puz, (L,C), 0, Res), propaga_posicoes((L,C),Res,Res2), escreve_Puzzle(PuzAlterado) ,verifica_R3(Res2), PuzAlterado=Res2 ,! ;
%    mat_muda_posicao(Puz, (L,C), 1, Res), propaga_posicoes((L,C),Res,Res2), escreve_Puzzle(PuzAlterado) ,verifica_R3(Res2) ,PuzAlterado=Res2,)   .
%    
%resolve_aux(_,(Tamanho,Tamanho),Tamanho,Aux,N_Puz):-
%    N_Puz=Aux.
%    
%
%resolve_aux(Puz,(L,C),Tamanho,Aux,N_Puz):-
%    mat_ref(Puz, (L, C), Cont),
%    resolve_final(Puz,(L,C),Aux);
%    (L<Tamanho,NovoL is L+1, resolve_aux(Puz,(NovoL,C),Tamanho,Aux,N_Puz),!;
%    C<Tamanho,NovoC is C+1, resolve_aux(Puz,(1,NovoC),Tamanho,Aux,N_Puz)).

%resolve(Puz,N_Puz):-
%    inicializa(Puz,Temp),
%    mat_dimensoes(Puz, Num_Lins, Num_Cols),
%    resolve_aux(Temp,(1,1),Num_Lins,Aux,N_Puz),
%    escreve_Puzzle(N_Puz).
