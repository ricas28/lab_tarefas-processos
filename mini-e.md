## Mini-exercício

**Implementação original**

...

### 1. Paralelização

**a)** Threads Monstros

...

### 2. Detetor de Movimentos

**a)** Incrementar contador global

Sim, é possivel que duas threads executem `moves++` simultaneamente

As duas podem ler o mesmo valor de `moves` antes de alguma delas escrever o novo valor.
Nesse caso, uma atualização pode perder-se.

Não. Como existe acesso concorrente à variável, alguns incrementos podem perder-se. O resultado pode ser inferior ao número efetivo de movimentos

**b)** Contador local e valor de retorno da _thread_

A thread pode terminar através de `pthread_exit()` indicando um ponteiro para o valor que pretende devolver, ou pode simplesmente fazer `return` desse ponteiro.

Através de `pthread_join()`. O segundo argumento permite receber o valor devolvido pela thread.

O contador local pertence apenas à _thread_ que o criou. Cada _thread_ possui a sua própria variável `ghost_moves` e, portanto, cada monstro mantém um contador independente. A variável global `moves`, por outro lado, pertence ao processo e é partilhada por todas as _threads_.

Porque cada _thread_ acede apenas ao seu próprio contador local. Não existem várias _threads_ a modificar simultaneamente a mesma variável, pelo que não ocorre uma _race condition_ sobre este contador.

### 3. Thread de Monitorização

**a)** _Thread_ de monitorização e `fork()`

Não. O processo filgo recebe uma cópia do estado de memória do processo pai no momento do `fork()`. As alterações posteriores feitas pelo processo pai não são refletidas no processo filho.

As _threads_ do mesmo processo partilham o espaço de memória, pelo que uma alteração a uma variável global pode ser observada pelas restantes _threads_. Um processo criado com `fork()` possui o seu próprio espaço de memória, incialmente com uma cópia do estado do processo pau.

A memória pertencente ao processo filgo é libertada pelo sistema operativo quando o processo termina. Essa memória deixa de estar disponível para o processo filho e não altera a memória do processo pai.

## Alíneas secretas

[ver alíneas](mini-e-alineas.md)

## Avaliação em aula

...