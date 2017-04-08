#!/bin/bash
#####################################################################################
# UQAM - Département d'informatique
# INF3105 - Structures de données et algorithmes
# TP3 - Calcul d'itinéraires à pied et en transport en commun
# http://ericbeaudry.ca/INF3105/tp3/
#
# Script d'évaluation
#
# Instructions:
# 1. Déposer ce script avec les fichiers problèmes dans un répertoire 
#    distinct (ex: tests).
# 2. Étudiants : Se placer dans le répertoire contenant votre programme
#    Correcteur : Se placer dans répertoire contenant les soumissions Oto (*.tp_oto).
# 3. Lancer ce script (ex: ../tests/evaluer.sh).
#####################################################################################

echo "Évaluation du TP3 d'INF3105..."

# Répertoire contenant les fichiers tests
repertoire_tests=`dirname $0`
# Villes à tester
villes="uqam stejulie halifax montreal"

########### Détection du valideur de résultats #######
# Le valideur à utiliser
valideur="${repertoire_tests}/valideur"
if [ -x "${valideur}-`uname`-`uname -p`" ]
then
    valideur="${valideur}-`uname`-`uname -p`"
else
    if [ -x "${valideur}-`uname`" ]
    then
        valideur="${valideur}-`uname`"
    fi
fi
######################################################


# Détection si l'utilitaire time sous Linux est disponible (peut afficher 0.00)
echo "Détection de /usr/bin/time..."
/usr/bin/time -f %U echo 2>&1 > /dev/null
souslinux=$?

# Limiter le temps d'exécution et la quantite de mémoire
ulimit -t 3600 -v 2097152 -f 65536
echo "Limite de temps par test : `ulimit -t` secondes."
echo "Limite de mémoire par test : `ulimit -v` KiO."
echo "Limite de taille de fichier : `ulimit -f` KiO."


# Détection du CPU
if [ -e /proc/cpuinfo ] ; then
    cpuinfo=`grep "model name" /proc/cpuinfo | sort -u | cut -d ":" -f 2`
else
    cpuinfo="?"
fi

function Nettoyer
{
	echo "Nettoyage"
    # Au cas où le Makefile des étudiants ne ferait pas un «make clean» correctement.
    make clean
   
    rm -f *.o* *.gch tp[1-3][ab] *.resultat *.validation *.png *.eps *.pdf
    # Au besoin, nettoyer les précédents fichiers logs
    rm -f log*.txt
}


##### Fonction d'évaluation d'un TP ######
function EvaluerTP
{
    ## Forcer la recompilation.
    #rm $programme *.o
    #make clean
    # Recompiler
    #make tp3a tp3b
    
    if [ ! -x tp3a ]
	    then
	    echo " Erreur : le fichier tp3a n'existe pas ou n'est pas exécutable!"
	    return
    fi
    if [ ! -x tp3b ]
	    then
	    echo " Erreur : le fichier tp3b n'existe pas ou n'est pas exécutable!"
	    return
    fi

    logfile="log-`date +%Y%m%d_%H%M%S`.txt"

    echo "Évaluation des temps d'exécution de ${programmeA} et ${programmeB} sur les jeux de tests."
    echo "Les résultats sont déposés dans $logfile."

	echo "Machine : " `hostname`
	echo "#Machine : " `hostname`  > $logfile
	echo "CPU :$cpuinfo" 
	echo "#CPU :$cpuinfo"  >> $logfile
	echo "Date début : $date"
	echo "#Date début : $date"  >> $logfile
	echo "Limite de `ulimit -t` secondes par test"
	echo "#Limite de `ulimit -t` secondes par test"  >> $logfile
    echo 
    echo "#" >> $logfile

    # Pour statistiques : nombre de lignes de code...
    # echo "Taille des fichiers source :" >> ${logfile}
    # wc *.{c*,h*}  >> ${logfile}

    taille=`wc *.{c*,h*} | grep total`
    sommaire="$taille"

    echo -e "#Test\tRequêtes\tCPU_A\tMém_A\tValA\tOptA\tCPU_B\tMém_B\tValB\tOptB" >> $logfile
    echo -e "Test\tRequêtes\tCPU_A\tMém_A\tValA\tOptA\tCPU_B\tMém_B\tValB\tOptB"

    # Itérer sur tous les réseaux
    for ville in $villes;
    do
        fcarte="${repertoire_tests}/${ville}-carte.txt"
        freseau="${repertoire_tests}/${ville}-reseau.txt"

        # Lister les fichiers tests pour le réseau actuel
        tests="`cd $repertoire_tests && ls ${ville}-req?.txt`"

        # Itérer sur tous les fichiers tests
        for test in $tests; do
            nbreqs=`cat $repertoire_tests/$test | wc -l`

            if [ $souslinux -eq 0 ]; then
                tA=`(/usr/bin/time -f "%U\t%M" ./tp3a $fcarte $repertoire_tests/$test > ${test%.txt}.resultatA) 2>&1 | tail -n 1`
                tB=`(/usr/bin/time -f "%U\t%M" ./tp3b $fcarte $freseau $repertoire_tests/$test > ${test%.txt}.resultatB) 2>&1 | tail -n 1`
            else
                tA=`(time -p ./$tp3a $fcarte $repertoire_tests/$test > ${test%.txt}.resultatA) 2>&1 | egrep user | cut -f 2 -d " "` 
                tB=`(time -p ./$tp3b $fcarte $freseau $repertoire_tests/$test > ${test%.txt}.resultatB) 2>&1 | egrep user | cut -f 2 -d " "` 
            fi

            if ( [ -x ${valideur} ] ); then
                verif=`$valideur $fcarte $freseau ${repertoire_tests}/${test} ${test%.txt}.resultatA ${test%.txt}.resultatB`
                verifAChVal=`echo $verif | cut -d " " -f 1`
                verifAInsVal=`echo $verif | cut -d " " -f 2`
                verifAOpt=`echo $verif | cut -d " " -f 3`
                verifBItVal=`echo $verif | cut -d " " -f 4`
                verifBOpt=`echo $verif | cut -d " " -f 5`
            else
                verifAChVal=""
                verifAInsVal=""
                verifAOpt=""
                verifBItVal=""
                verifBOpt=""
            	if( [ -e ${repertoire_tests}/${test%.txt}.resultatA ] ); then
                	diff -tibw ${test%.txt}.resultatA ${repertoire_tests}/${test%.txt}.resultatA 2>&1 > /dev/null
            		if [ $? -eq 0 ]; then
            	    	verifAChVal="OK"
             		else
             	    	verifAChVal="Different"
            		fi
            	fi
            	if( [ -e ${repertoire_tests}/${test%.txt}.resultatB ] ); then
                	diff -tibw ${test%.txt}.resultatB ${repertoire_tests}/${test%.txt}.resultatB 2>&1 > /dev/null
            		if [ $? -eq 0 ]; then
            	    	verifBItVal="OK"
             		else
             	    	verifBOpt="Different"
            		fi
            	fi
            fi

            echo -e "$test\t$nbreqs\t${tA}\t${verifAChVal}\t${verifAOpt}\t${tB}\t${verifBItVal}\t${verifBOpt}"
            echo -e "$test\t$nbreqs\t${tA}\t${verifAChVal}\t${verifAOpt}\t${tB}\t${verifBItVal}\t${verifBOpt}" >> $logfile
            sommaire="${sommaire}\t\t${tA}\t${verifAChVal}\t${verifAOpt}\t${tB}\t${verifBItVal}\t${verifBOpt}"
        done

    done
}

# Lister les soumissions Oto (répertoires terminant par .tp_oto)
tps=`ls -1 | grep .tp_oto`
# Si aucun répertoire .tp_oto n'existe, essayer le répertoire courant (auto-évaluation)
if [ ! -n "$tps" ]; then
    tps="."
fi

# Génération de l'entête du rapport
echo "#Rapport de correction INF3105 / $programme" > "rapport-${date}.txt"
echo -e "#Date:\t${date2}" >> "rapport-${date}.txt"
echo -e "#Machine :\t" `hostname` >> "rapport-${date}.txt"
echo -e "#CPU :\t$cpuinfo" >> "rapport-${date}.txt"
echo >> "rapport-${date}.txt"

# Génération des titres des colonnes
echo -e -n "#\t\t\t" >> "rapport-${date}.txt"
for ville in ${villes}; 
do
    ville=${ville%-ville.txt}
    tests="`cd $repertoire_tests && ls ${ville}-req*.txt`"
    for test in $tests;
    do
        echo -e -n "$test\t\t\t\t\t\t\t\t\t" >> "rapport-${date}.txt"
    done        
done
echo >> "rapport-${date}.txt"

echo -e -n "#Soumission\tTaille sources" >> "rapport-${date}.txt"
for ville in ${villes}; 
do
    ville=${ville%-ville.txt}
    tests="`cd $repertoire_tests && ls ${ville}-req*-*.txt`"
    for test in $tests;
    do
        echo -e -n "\t\tCPU_A\tMém_A\tValA\tOptA\tCPU_B\tMém_B\tValB\tOptB" >> "rapport-${date}.txt"
    done        
done
echo >> "rapport-${date}.txt"

# Itération sur chaque TP
for tp in $tps; do
    sommaire=""
    echo "## CORRECTION : $tp"
    pushd $tp
    	EvaluerTP
	#Nettoyer
    popd
    #echo -e ">> ${sommaire}"
    echo -e "${tp}\t${sommaire}" >> "rapport-${date}.txt"
done

