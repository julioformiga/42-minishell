# Analisi del progetto Minishell

## Contenuto del progetto

Il progetto è suddiviso in più file e cartelle. Di seguito vengono analizzati i principali file e funzioni, con una spiegazione dettagliata di ogni componente.

---

## Cartella `command`

### File: `cmd_exec.c`

#### Funzionalità principali

1. **`t_builtin_fn get_builtin(char *cmd_name)`**

   - Cerca una funzione built-in associata a un comando. Se il comando è supportato, restituisce un puntatore alla funzione corrispondente; altrimenti, restituisce `NULL`.

2. **`static char **env_to_array(t_env *env)`**

   - Converte una lista collegata di variabili di ambiente (`t_env`) in un array di stringhe. Ogni stringa ha il formato `KEY=VALUE`.

3. **`static int cmd_create_pipe(int pipefd[2])`**

   - Crea una pipe per la comunicazione tra processi. Restituisce `1` in caso di errore.

4. **`static int is_interactive_program(void)`**

   - Determina se il programma è in esecuzione in modalità interattiva controllando il terminale.

5. **`static int cmd_fork(char *full_path, char **args, int pipefd[2], t_env *env)`**

   - Crea un processo figlio con `fork()` e lo esegue usando `execve`. Configura le variabili di ambiente e le pipe se necessario.

6. **`void cmd_exec_inline(int argc, char **argv, t_env **env, t_cmd *cmd)`**

   - Esegue un comando inline. Gestisce la terminazione e il cleanup delle risorse.

7. **`int cmd_exec(t_cmd *cmd, t_env *env)`**

   - Esegue un comando. Se il comando è built-in, lo esegue direttamente; altrimenti cerca il percorso eseguibile e crea un processo figlio per l'esecuzione.

---

### File: `cmd_init.c` (precedentemente `command.c`)

#### Funzionalità principali

1. **`char *cmd_check(t_cmd *cmd, t_env *env)`**

   - Verifica la presenza di un comando nel `PATH`. Se trovato, restituisce il percorso completo; altrimenti stampa un errore e restituisce `NULL`.

2. **`int cmd_setup(t_cmd *cmd, t_env *env, char ***args, char **full_path)`**

   - Configura un comando, inizializzando gli argomenti e il percorso completo.

3. **`t_cmdblock *create_cmdblock(char *cmd_part)`**

   - Crea un blocco di comando (`t_cmdblock`) separando l'eseguibile e i suoi argomenti.

4. **`void cmd_init(char *readline, t_cmd *cmd)`**

   - Analizza una riga di comando e la divide in blocchi (`t_cmdblock`), gestendo separatori come `|` o `&&`.

5. **`void cmd_print(t_cmd *command)`**

   - Stampa i dettagli di un comando, inclusi eseguibile, argomenti e separatori.

---

### File: `cmd_parser.c`

#### Funzionalità principali

1. **`void cmd_parser(char *readline, t_cmd *command)`**
   - Inizializza una struttura `t_cmd` a partire da una riga di input.

---

### File: `utils.c`

#### Funzionalità principali

1. **`int cmd_count_args(char *cmd)`**

   - Conta il numero di argomenti in un comando.

2. **`char **cmd_get_args(char *cmd)`**

   - Estrae gli argomenti di un comando e li restituisce come array di stringhe.

---

## Cartella `env`

### File: `env.c`

#### Funzionalità principali

1. **`t_env *env_init(char **envp)`**

   - Inizializza una lista collegata di variabili di ambiente (`t_env`) a partire da `envp`.

2. **`int env_set(t_env *env, char *key, char *value)`**

   - Modifica o aggiunge una variabile di ambiente. Restituisce `0` se l'operazione ha successo, `1` altrimenti.

3. **`char *env_get(t_env *env, char *key)`**

   - Restituisce il valore di una variabile di ambiente specificata da `key`, oppure `NULL` se non esiste.

4. **`void env_print(t_env *env)`**

   - Stampa tutte le variabili di ambiente in formato `KEY=VALUE`.

5. **`void env_free(t_env *env)`**

   - Libera la memoria associata alla lista collegata delle variabili di ambiente.

---

## Strutture principali

### Struttura `t_env`

Utilizzata per rappresentare una variabile di ambiente:

- **`char *key`**: Nome della variabile.
- **`char *value`**: Valore della variabile.
- **`t_env *next`**: Puntatore alla variabile successiva nella lista.

### Struttura `t_cmd`

Utilizzata per rappresentare un comando:

- **`char *cmd_line`**: La riga di comando completa.
- **`t_cmdblock *cmd`**: Lista collegata di blocchi di comando.

### Struttura `t_cmdblock`

Un blocco di comando contiene:

- **`char *exec`**: L'eseguibile.
- **`char **args`**: Gli argomenti.
- **`char *sep`**: Un separatore opzionale (es. `|`, `&&`).
- **`t_cmdblock *next`**: Il prossimo blocco di comando.

---

Questo documento fornisce una panoramica del progetto. Se hai bisogno di ulteriori dettagli su un file o una funzione specifica, fammi sapere!


