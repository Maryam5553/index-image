# Image indexée

Ce programme convertit une image jpeg, png ou gif en format indexé.

## Utilisation

- convertir votre image jpeg, png ou gif appelée "image.jpeg" en format lisible par le programme en exécutant `./writergba image.jpeg image.png` (où image.png est le nouveau nom que vous voulez lui donner)
- indexez votre image en exécutant `./index image.png new_image.png` (image.png le nom de votre fichier converti et new_image.png est le nouveau nom). Vous pouvez choisir nouvelle palette de couleurs avec une option parmi -s (saturée), -b (noir et blanc), -g (niveaux de gris), -4 (4-4-4). Par défaut la palette utilisée est 4-4-4.

## Qu'est-ce qu'une image indexée ?

C'est un format plus efficace pour les images en png dans lequel les couleurs sont stockées dans une palette, et chaque pixel de l'image est indexé par sa couleur dans la palette.

Or, les images indexées ne supportent que des palettes de 256 couleurs.
Donc si l'image comporte plus de 256 couleurs, nous allons proposer de réduire le nombre de couleurs en choisissant une palette parmi les suivantes :

- saturée
- noir et blanc (binaire)
- niveaux de gris
- 4-4-4 (à 64 couleurs {(51i, 51j, 51k) | i,j,k = 0,...,5})
