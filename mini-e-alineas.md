# Alíneas secretas

## Paralelização pacman

Cada grupo recebe *apenas uma* das variáveis seguintes.

A parte pública do exercício mantém-se inalterada.

---

## Variantes de contadores e estatísticas

### CNT1 - Contar comandos por tipo

*Enunciado*

No final de cada nível, apresentar o número total de comandos de cada tipo executados pelo Pacman e pelos monstros.

Exemplo:

```bash
T:2,W:4,A:9,S:0,D:5,C:8,G:2
```

*Apoio*
A struct `command_t` tem um campo `command` onde facilmente é obtido o comando executado por cada entidade.

Manter um contador independente para cada tipo de comando.

Cada vez que um comando é processado, atualizar o contador correspondente.

### MOVE1 - Monstro com mais movimentos

*Enunciado*

No final do nível, indicar qual foi o fantasma que realizou mais movimentos.

Exemplo:

```bash
Ghost 2 made the most moves: 31
```

Em caso de empate, indicar o de menor índice.

*Apoio*
Manter o número de movimentos de cada fantasma separadamente.

Depois de todas as _threads_ terminarem, percorrer esses valores para determinar o maior.

### MOVE2 - Média de movimentos dos monstros`

*Enunciado*

No final do nível, apresentar o número médio de movimentos realizados pelos monstros.

```bash
Average ghost moves: 27.3
```

*Apoio*
Manter o número de movimentos de cada fantasma separadamente.

Depois de todas as _threads_ terminarem, percorrer esses valores e determinar a média

## Variantes de processos.

### PID1 - Identificar processo

*Enunciado*

Cada processo filho criado para a monitorização da thread deverá apresentar o seu PID.

Exemplo:

```bash
Monitor process: 23145
```

*Apoio*

O PID do processo filho é obtido através da função `getpid()

### CNTP1 - Contar processos criados

*Enunciado*
No final de cada nível, apresentar o número total de processos filhos criados pela thread de monitorização

Exemplo

```bash
Monitor processes created: 5
```

*Apoio*
Criar contador global  ou criar contador local, que é depois devolvido pela _thread_, e incrementar a cada `fork()` executado com sucesso.

Após join de todas as _threads_, apresentar o valor no contador.

### MAXP1 - Limite de processos

*Enunciado*

O programa deverá defenir, através de uma constante, o número máximo de processos de monitorização que podem ser criados num nível.

Quando esse limite for atingido, não deverão ser criados mais processos e deverá ser registada uma mensagem no ficheiro de debug.

*Apoio*

Criar um contador local na _thread_ de monitorização e a cada `fork()` executado com sucesso, aumentar o contador.

Se contador exceder limite definido na constante, imprimir a mensagem de limite atingido.

### ESTP1 - Estado do Pacman no filho

*Enunciado*

O processo filho deverá apresentar também a posição atual do Pacman no momento do `fork()`.

Exemplo:

```bash
Pacman position: (4, 7)
```

*Apoio*

Através do `game_board` conseguimos aceder ao Pacman e, por sua vez, aos campos `pos_x` e `pos_y` do mesmo.

## Variantes de comportamento do jogo

### INV1 - Inverter controlos

*Enunciado*
Quando o programa é executado com a opção `-i`, os comandos `A` e `D` deverão trocar de significado, tal como `W` e `S`.

Sem a opção `-i`, o comportamento deverá permanecer inalterado.

*Apoio*

Receber a opção por command line arguments e ter variável que define se modo está ativo ou não.

Dependendo do valor da variável inverter, ou não, os controlos no momento em que é feito parse aos ficheiros em `parser.c`

### RES1 - Pacman sobrevive a um choque

*Enunciado*
O Pacman deverá sobreviver ao primeiro choque com um monstro. Apenas o segundo choque deverá terminar o jogo.

*Apoio*
Manter um contador do número de colisões entre o Pacman e os monstros.

Incrementar contador a cada colisão.

Quando o contador for igual a 2, terminar o jogo.

## Variantes de parsing de ficheiros

### COL1 - Contar a ocorrência do conteúdo do tabuleiro

*Enunciado*
No final da leitura de cada ficheiro .lvl, deve ser apresentado o número de ocorrências de cada símbolo presente no tabuleiro.

Exemplo:

```bash
X:14,o:18,@:1
```

*Apoio*

Criar contadores para cada símbolo na função `read_level()` e incrementar o mesmo cada vez que o respetivo símbolo é lido.

### COL2 - Contar o número de comentários

*Enunciado*
No final da leitura de todos os ficheiros, deve ser apresentado o número de comentários que existiam em todos os ficheiros

Exemplo:

```bash
Found 5 comments
```

*Apoio*

Criar contador para número de comentários vistos na função `load_level` e passar esse contador a cada função `read_XXXX()`, sendo incrementado cada vez que um comentário é lido.