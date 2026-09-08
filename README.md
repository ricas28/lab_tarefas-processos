# Guião sobre programação com tarefas e processos

![IST](img/IST_DEI.png)  

## Objetivos

No final deste guião, deverá ser capaz de:

- compreender os conceitos programação paralela com tarefas (que é um dos requisitos essenciais do 1º exercício do projeto de SO) e também com processos;
- utilizar primitivas para a gestão de tarefas como o `pthread_create` e `pthread_join`;
- utilizar primitivas para a gestão de processos, como o `fork` e `wait`.

## Requisitos

- Sistema operativo Linux Ubuntu 20.04 LTS (se não o tiverem disponível no vosso computador pessoal, podem utilizar os computadores do laboratório).

## 1. Tarefas

Clone este repositório, usando: `git clone https://github.com/tecnico-so/lab_tarefas-processos.git`.

Aceda à diretoria com o comando: `cd lab_tarefas-processos`.

Na pasta `simple-example/src` estude, compile e execute a aplicação.  
O programa cria diferentes tarefas (*threads*) a executar em paralelo.

### 1.1 Interpretação dos resultados

Consulte o seguinte diagrama e interprete o resultado do programa.

![THREAD](img/thread_image_800px.png)

Porque razão diferentes execuções do programa podem resultar em diferentes valores para variável global `g_value`?

### 1.2 Exercício

Monte uma experiência que prove a afirmação anterior (ponto 1.1).  
Sugestão: utilize a chamada sistema [`sleep`](https://man7.org/linux/man-pages/man3/sleep.3.html).

### 1.3 Uso de sanitizador

Como iremos lidar com programas concorrentes aconselha-se a utilização do `ThreadSanitizer` para ajudar a detetar problemas de acessos concorrentes.
Para isto, note a ativação deste sanitizador com a opção `-fsanitize` na *Makefile*.

Revisite o guião sobre ferramentas de deteção de erros, onde pode encontrar mais detalhes sobre o uso de sanitizadores de código.

----

## 2. Processos

Componha um programa paralelo alternativo, `process.c`, em que as chamadas a `pthread_create` e `pthread_join` são substituídas por chamadas a `fork` e `wait` (tenha o cuidado de assegurar que a função `wait` só é chamada pelo processo pai).

Neste caso, o paralelismo acontecerá com processos distintos, e não com tarefas dentro do mesmo processo.
O que muda no resultado que este novo programa imprime no *stdout*?

----

## Mini-exercício

Neste exercício iremos transformar uma implementação estática do **pacman**, numa versão paralelizada, onde cada entidade é controlada por uma thread diferente.

Sempre que forem criadas _threads_, utilize a biblioteca POSIX `pthread`.

Sempre que for pedido para algo ser apresentado, utilize a função `debug`.

Este exercíco utiliza a biblioteca `ncurses`. Para instalar a mesma, será necessário um dos seguintes comandos, dependendo do seu ambiente:

- **Linux - Debian/Ubuntu**

```bash
sudo apt update 
sudo apt-get install libncurses-dev
```

- **macOS**

Caso utilize **Homebrew**, execute:
```bash
brew install ncurses
```

- **Computador do Laboratório da RNL**

Caso esteja a fazer este exercício num computador de um laboratório da RNL, pode criar um ambiente temporário, sem instalar permanentemente a biblioteca no sistema.

Execute na diretoria do projeto:
```bash
nix shell nixpkgs#ncurses
```

Depois de entrar na shell, poderá executar o exercício sem problema nenhum.

----

### Implementação original  

Começar por ver a implementação original do **pacmanIST** na pasta `pacmanIST-original`.

Para experimentar o código: Seguir as instruções no **README**.

Analise o código de forma a perceber como o jogo é implementado e experimente o mesmo. Nesta versão, o jogo continua infinitamente, repetindo o nível inicial, até o pacman morrer, ou o utilizador pressionar a tecla '**Q**'.


### 1. Paralelização 

Pretende-se que o programa passe a ser paralelizado de forma a que os monstros se  movimentem sem terem de esperar pelo input do pacman.

Começar pela implementação do pacmanIST adaptada a este exercício, na pasta `pacmanIST-adapted`. Nesta versão, os inputs do nível, e de cada entidade, são fornecidos por ficheiros, de maneira a termos um comportamento determinístico. Analise os mesmos, na pasta `levels_auto`, e perceba qual a sua sintaxe.

Analise o código do ficheiro `game.c` e veja como as threads para o pacman e para a interação com a biblioteca ncurses são geridas.

**a)** Threads Monstros

Altere o programa de forma a que cada monstro seja executado por uma thread independente.

Quando o jogo terminar, todas as threads dos monstros devem terminar corretamente.

Confirme, através da execução do programa, que os monstros continuam a movimentar-se enquanto o pacman está parado (sem receber input).

**Sugestão**: Altere o conteúdo do ficheiro `1.p` ou `1.lvl` na pasta `levels_auto`.

### 2. Detetor de movimentos

Pretende-se agora contabilizar o número total de movimentos realizados durante o jogo.

Crie uma variável global:

```C
int moves = 0;
```

**b)** Incrementar contador Global

Sempre que o Pacman ou um monstro realizar um movimento, o programa deverá executar:

```C
moves++;
```

No final do jogo, o número total de movimentos deverá ser apresentado, por exemplo:

```bash
Total moves: 67
```

Execute o programa várias vezes e observe os valores obtidos.

Considere os seguintes pontos:
- Quem pode alterar a variável `moves`?
- Podem existir duas _threads_ a executar `moves++` simultaneamente?
- O que poderá acontecer se duas _threads_ tentarem alterar a variável ao mesmo tempo?
- O valor apresentado no final corresponde necessariamente ao número esperado de movimentos?

**Nesta alínea não é necessário corrigir o problema.**

O objetivo é identificar e observar uma possível _**race condition**_ provocada pelo acesso concorrente à variável global.

### 3. Contabilizar os movimentos de cada monstro

Pretende-se agora saber quantos movimentos realizou cada monstro.

**c)** Contador local e valor de retorno da _thread_

Altere a função das _threads_ dos monstros de forma a que cada um mantenha o seu próprio contador de movimentos.

O contador deverá ser uma variável local da função da _thread_:

```C
int ghost_moves = 0;
```

Sempre que o respetivo monstro realizar um movimento, o contador deverá ser incrementado.

Quando o jogo terminar, cada _thread_ deverá devolver o número de movimentos realizado pelo respetivo monstro.

O programa principal deverá obter o valor devolvido por cada _thread_ e apresentar, por exemplo:

```bash
Monster 0: 37 moves
Monster 1: 42 moves
Monster 2: 35 moves
```

Utilize o mecanismo POSIX adequado para obter o valor devolvido por uma _thread_.

Considere:
- Como pode uma _thread_ devolver um valor?
- Como pode a função que criou a _thread_ obter esse valor?
- Qual é a diferença entre o contador local desta alínea e a variável global `moves` da alínea anterior? 
- Porque razão este contador não apresenta o mesmo problema da variável global `moves`?

### 4. Thread de monitorização

Pretende-se agora criar uma _thread_ adicional que seja responsável por obter periodicamente informações sobre o estado do jogo.

**d)** Criar _thread_ de monitorização

Crie uma nova _thread_ no início do jogo.

Esta _thread_ terá de receber como um dos seus argumentos um tempo de espera, `wait_time`, que será o número de segundos entre duas consultas ao estado do jogo.

A _thread_ deverá executar, repetidamente, o seguinte procedimento:

1. Esperar o número de segundos indicado pelo argumento de espera;
2. Criar um processo filho através de `fork()`;
3. O processo filho deverá apresentar o número de movimentos realizados e o conteúdo do `game_board` utilizando a função `print_board`;
4. O processo filho deverá terminar;
5. A _thread_ deverá continuar a executar;

Por exemplo:

```bash
--- Current Game State ---
Current total moves: 42

=== [110412] LEVEL INFO ===
Dimensions: 10 x 10
Tempo: 300
Pacman file: levels_auto/1.p
Monster files (2):
  - levels_auto/4.m
  - levels_auto/1.m

=== BOARD ===
WWWWWWWWWW
W        W
W        W
W        W
WWWW  WWWW
WM       W
W   M    W
W        W
W    P   W
WWWWWWWWWW
==================

--- Current Game State ---
```

A _thread_ de monitorização deverá continuar a criar processos de `wait_time` em `wait_time` segundos até o jogo terminar.

Considere os seguintes pontos:
- Se o Pacman se mover depois do `fork()`, o processo filho vê essa alteração?
- O valor de `moves` no processo filho continua a ser alterado enquanto o jogo decorre?
- Qual a diferença, neste aspeto, entre uma _thread_ e um processo?
- O que acontece à memória do processo filho quando este termina?

## Conclusão

As tarefas (_threads_) permitem ter paralelismo dentro de um processo, com partilha de memória.
Os processos permitem ter paralelismo dentro de um sistema, permitindo ter várias aplicações a executar-se separadamente, com isolamento da memória.
Neste laboratório vimos os dois tipos de paralelismo em ação.

----

Contactos para sugestões/correções: [LEIC-Alameda](mailto:leic-so-alameda@disciplinas.tecnico.ulisboa.pt), [LEIC-Tagus](mailto:leic-so-tagus@disciplinas.tecnico.ulisboa.pt), [LETI](mailto:leti-so-tagus@disciplinas.tecnico.ulisboa.pt)
