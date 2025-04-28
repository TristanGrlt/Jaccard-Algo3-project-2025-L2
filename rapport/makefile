# Makefile : compile automatiquement toutes les images en images utilisable dans un `.tex`
# 	- drawio doit être installé sur la machine : `sudo snap install drawio`.
#	- faire un `make` pour recompiler les images si besoin
#	- compiler le `.tex`

# Fonctionement :
#	-1 Créer `nom_image.drawio`
#	-2 inclure `nom_image.pdf` dans `.tex`
#	-3 compiler pour créer l'image (avec make) (clean pour toutes les suprimer)
# 	-4 compiler le `.tex`


# Trouver automatiquement tous les fichiers .drawio
DRAWIO_SRC = $(wildcard *.drawio)

# Générer la liste des fichiers .pdf correspondants
DRAWIO_PDFS = $(DRAWIO_SRC:.drawio=.pdf)

all: $(DRAWIO_PDFS)

# Règle pour convertir .drawio -> .pdf
%.pdf: %.drawio
	drawio -x --crop --format pdf --output "$@" "$<"

clean:
	rm -f $(DRAWIO_PDFS)

.PHONY: all clean
