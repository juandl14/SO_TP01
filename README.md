# Sistemas Operativos

## Trabajo Práctico N°1 - Inter Process Communication

### Autores:
* Borracci, Ángeles - 56648
* De Luca, Juan Manuel - 60103
* Kim, Azul Candelaria - 60264

### Contenidos:
1. Prerrequisitos
2. Compilación
3. Ejecución

---

#### 1. Prerrequisitos:
1. Clonar este repositorio
2. Contar con los archivos que se quieran analizar [ en donde? ]

#### 2. Compilación:
Para compilar ingrese, en una terminal, el siguiente comando en el mismo directorio donde se encuentra el archivo llamado *Makefile*:
```
$ make all
```

#### 3. Ejecución
El programa App guardará los resultados en un archivo mediante la siguiente ejecución:
```
$ ./App <files_path>
```
Esto devolverá un entero <MEMORY_SIZE>, el cual se usa como argumento en la ejecución del View.
Para imprimir en pantalla estos resultados, ejecutar View de la siguiente forma:
```
$ ./View <MEMORY_SIZE>
```

También se pueden ejecutar al mismo tiempo View y App con el siguiente comando:
```
$ ./App <files_path> | ./View
```
---

Finalmente y una vez terminada la ejecución, si se quieren remover los archivos creados en la compilación, debe utilizarse el siguiente comando en la terminal:
```
$ make clean
```
