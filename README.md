# Projet Termites

## Compilation

1. Clonez le projet et placez-vous dans le dossier `Projet`
2. Si vous avez nix :
  - Entrez dans le shell (assurez vous d'avoir les flakes activés) :
  ```bash
  nix develop --impure
  ```
  - Sinon, il vous suffit d'avoir `clang` et `raylib` d'installés sur votre machine.
3. Compilez le projet :
```bash
make main
```
4. Exécutez le projet :
```bash
./main
```

## Tests 
Pour lancer les tests, utilisez la commande suivante :
```bash
make tests
``` 
puis pour exécuter les tests :
```bash
./tests
```
