﻿TP 02 - Synthèse d'images
Activite: lumiere; refraction; plans; lampe spherique;
Etudiant: Edwyn Luis dos Santos Batista

Ma submission consiste d'un projet en CodeBlocks, en utilisant la language C++;
Les fichiers .cpp sont trouvables sur "S_TP01";
Le fichier principal est nomé "main.cpp". Il contient les tests et executions des algorithmes.

Des images sont groupés dans le dossier "\Raytracer\images":
=== TP01
- 01-image.ppm: 	produit de l'algorithme de la Partie 01 (fonction "saveImage");
- 02-sphere.ppm:	produit de l'algorithme de la Partie 02 (rendu de l'une sphere - sans couleur);
- 03-sphere2.ppm:	produit de l'algorithme de la Partie 02 (rendu de l'une sphere - avec couleur);
- 04-sphere3.ppm:	produit de l'algorithme de la Partie 03 (rendu de plusieurs objets - axe y inversé);
- 05-sphere4.ppm:	produit de l'algorithme de la Partie 03 (rendu de plusieurs objets - axe y régulier);
=== TP02
- 06-sphereMirroir.ppm:				rendu d'une sphere comme mirroir;
- 07-sphereMirroir - Lumiere - sansNormal.ppm:	rendu d'une sphere comme mirroir et lumiere (lumiere simple);
- 08-sphereMirroir - Lumiere - gris.ppm:	rendu d'une sphere comme mirroir et lumiere (lumiere considerant le normal; gris);
- 09-sphereMirroir - Lumiere - albedo.ppm:	rendu d'une sphere comme mirroir et lumiere (lumiere considerant le normal; albedo);
- 10-sphereMirroir - Lumiere - avecNormal.ppm:	rendu d'une sphere comme mirroir et lumiere (lumiere considerant le normal; albedo);
- 11-spherePlans.ppm:				rendu des plans au lieu des murs;
- 12-sphereTransparence - sansRefraction - LumiereDirecte.ppm:	rendu d'une sphere transparente (considerant que la lumiere n'arrive pas au cote sombre de la sphere : probleme);
- 13-sphereTransparence - sansRefraction - LumiereObjet.ppm:	rendu d'une sphere transparente (considerant que la lumiere est la meme aue celle de l'objet derriere la sphere);
- 14-sphereTransparence - refraction - sansLumiereAmbiance.ppm:rendu d'une sphere transparente avec formule de Refraction (les objets n'ont pas de lumiere propre; tout depende de la lampe);
===
- 15-sphereLumiere : 				la lampe est represente par une sphere (l'ombre est plus douce);
- 16-indirecte - LumierePoint-0rayons-1it :	eclairage indirecte (la lampe est considere comme un point; 0 rayons de lumiere incidents considere; 1 iteration de eclairage => meme que l'eclairage directe)
- 17-indirecte - LumierePoint-0rayons-3it :	eclairage indirecte (la lampe est considere comme un point; 0 rayons de lumiere incidents considere; 3 iteration de eclairage => meme que l'eclairage directe)