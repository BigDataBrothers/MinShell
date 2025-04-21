# MinShell

[![École 42](https://img.shields.io/badge/École-42-0055FF.svg)](https://42.fr)
[![Status](https://img.shields.io/badge/Status-Completed-green.svg)]()
![License](https://img.shields.io/badge/License-42--School-lightblue)
![Shell](https://img.shields.io/badge/Shell-Bash-lightgrey.svg)
![C](https://img.shields.io/badge/C-Language-00599C.svg)
![Readline](https://img.shields.io/badge/Readline-lib-lightgrey.svg)
![Threads](https://img.shields.io/badge/Pthreads-supported-blue.svg)

MinShell est un shell minimaliste implémentant les fonctionnalités de base d'un shell Unix.

## Fonctionnalités

- **Exécution de commandes** :
  - Commandes simples ou complexes
  - Gestion des redirections (`>`, `<`, `>>`, `<<`)
  - Support des pipes (`|`)
- **Gestion des variables d'environnement** :
  - Expansion avec `$`
  - Commandes intégrées pour manipuler les variables (`export`, `unset`, etc.)
- **Commandes intégrées** :
  - `cd` : Changer de répertoire
  - `echo` : Afficher du texte
  - `env` : Afficher les variables d'environnement
  - `exit` : Quitter le shell
  - `export` : Définir des variables d'environnement
  - `pwd` : Afficher le répertoire courant
  - `unset` : Supprimer des variables d'environnement
- **Gestion des signaux** :
  - Support de `Ctrl-C`, `Ctrl-D`, `Ctrl-\`

## Installation

Clonez le dépôt et compilez le projet :

```bash
git clone https://github.com/BigDataBrothers/MinShell
cd MinShell
make
```

## Utilisation

Lancez le shell avec la commande suivante :

```bash
./minishell
```

## Exemples

### Commande simple
```bash
> ls -la
```

### Redirection de sortie
```bash
> ls > fichier.txt
```

### Pipe
```bash
> ls -la | grep .c
```

### Utilisation de variables
```bash
> echo $HOME
```

## Dépendances

MinShell utilise les bibliothèques suivantes :
- `readline` pour la gestion des entrées utilisateur
- `pthread` pour la gestion des threads

Assurez-vous que ces bibliothèques sont installées sur votre système avant de compiler le projet.

## Auteur

- **Mehdi Yassine**

## Licence

> ℹ️ Ce projet est réalisé dans le cadre du cursus de l'école 42. Il est soumis à ses politiques académiques et ne peut être utilisé à des fins commerciales ou éducatives sans autorisation.
