Convention morse :

. = 1 tick de son
- = 3 ticks de son
changement de lettre = 3 ticks de blanc
espace = 7 ticks de blanc (5 ticks de blanc sont acceptés au décodage d'un fichier)

Pour les fichiers wav codé avec ce projet, le tick est choisi à 0.1 seconde.

Pour décoder un fichier (.wav mono), il est nécessaire de connaître le temps en seconde d'un tick et de l'indiquer (demandé par le programme lors de l'éxécution)
Il faut que le fichier encodé par l'autre programme respecte son tick à la création du fichier audio (j'ai constaté en essayant de décoder des fichiers wav de certains camarades que leur fichier audio ne respectait pas le tick qu'il pensait utiliser)
Fichiers audio wav à placer dans le même dossier que morse.exe (dossier build créé après compilation)
