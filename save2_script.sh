#!/bin/bash

# Fichier contenant la liste des commandes à tester
COMMANDS_FILE="cmd.txt"

# Exécutable de ton minishell
MINISHELL_EXEC="./minishell"

# Dossier temporaire pour stocker les sorties
TMP_DIR="tmp_test_output"
mkdir -p $TMP_DIR

# Fichiers temporaires pour les sorties
BASH_OUTPUT="$TMP_DIR/bash_output.txt"
MINISHELL_OUTPUT="$TMP_DIR/minishell_output.txt"
MINISHELL_OUTPUT_FILTERED="$TMP_DIR/minishell_output_filtered.txt"
VALGRIND_OUTPUT="$TMP_DIR/valgrind_output.txt"

# Fonction pour découper les lignes longues
wrap_lines() {
	local file=$1
	local width=$2
	fold -sw $width "$file"
}

# Lire le fichier de commandes et exécuter chaque commande
while IFS= read -r command || [[ -n "$command" ]]; do
	# Exécuter la commande avec bash et sauvegarder la sortie
	bash -c "$command" > "$BASH_OUTPUT" 2>&1

	# Exécuter la commande avec ou sans Valgrind sur minishell et sauvegarder la sortie
	# La ligne ci-dessous exécute le shell sans Valgrind
	echo "$command" | valgrind --leak-check=full --show-leak-kinds=all --suppressions=mask_readline_leaks.supp --log-file="$VALGRIND_OUTPUT" $MINISHELL_EXEC > "$MINISHELL_OUTPUT" 2>&1
	#echo "$command" | $MINISHELL_EXEC > "$MINISHELL_OUTPUT" 2>&1

	# Filtrer la première et dernière ligne de la sortie du minishell
	sed '1d;$d' "$MINISHELL_OUTPUT" > "$MINISHELL_OUTPUT_FILTERED"
	tail -n 1 "$MINISHELL_OUTPUT" >> "$MINISHELL_OUTPUT_FILTERED"

	# Découper les lignes longues pour une meilleure comparaison
	wrap_lines "$BASH_OUTPUT" 40 > "$TMP_DIR/bash_output_wrapped.txt"
	wrap_lines "$MINISHELL_OUTPUT_FILTERED" 40 > "$TMP_DIR/minishell_output_wrapped.txt"

	# Comparer les sorties
	echo "Commande: '$command'"
	printf "%-40s + %-40s\n" "Bash output" "Minishell output"
	printf "%-40s + %-40s\n" "----------------------------------------" "----------------------------------------"

	# Affichage aligné avec gestion des lignes de différentes longueurs
	paste <(cat "$TMP_DIR/bash_output_wrapped.txt") <(cat "$TMP_DIR/minishell_output_wrapped.txt") | while IFS=$'\t' read -r bash_line minishell_line; do
		printf "%-40s + %-40s\n" "${bash_line:- }" "${minishell_line:- }"
	done

	# Ligne de séparation entre les commandes
	printf "%-40s + %-40s\n" "----------------------------------------" "----------------------------------------"
	echo " "

	# Afficher le rapport de Valgrind s'il existe
	if [[ -f "$VALGRIND_OUTPUT" && -s "$VALGRIND_OUTPUT" ]]; then
		echo "Valgrind report for command: '$command'"
		cat "$VALGRIND_OUTPUT"
	else
		echo "No Valgrind report generated for command: '$command'"
	fi
	echo " "
	printf "%-80s\n" "================================================================================"
done < "$COMMANDS_FILE"

# Nettoyer le dossier temporaire
rm -r $TMP_DIR
