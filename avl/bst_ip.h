//  bst_ip.h : précisions sur l'implantation du module bst.

//  bst_empty : temps constant ; espace constant

//  bst_dispose : temps linéaire en la taille de l'arbre associé à *tptr ;
// espace linéaire en la taille de l'arbre associé à *tptr

//  bst_add_endofpath : temps au plus linéaire en la hauteur de l'arbre associé
// à *t ; espace  au plus linéaire en la hauteur de l'arbre associé à *t

//  bst_remove_climbup_left : temps au plus linéaire en la hauteur de l'arbre
// associé à t ; espace  au plus linéaire en la hauteur de l'arbre associé à *t

//  bst_search : temps au plus linéaire en la hauteur de l'arbre associé à t ;
// espace  au plus linéaire en la hauteur de l'arbre associé à *t

//  bst_size : temps constant ; espace constant

//  bst_height : temps constant ; espace constant

//  bst_distance : temps logarithmique en la taille de l'arbre associé
// à t ; espace linéaire en la taille de l'arbre associé à t

//  bst_number : temps au plus linéaire en la hauteur de l'arbre associé à t ;
// espace  au plus linéaire en la hauteur de l'arbre associé à *t

//  bst_rank : temps linéaire en la taille de l'arbre associé à t ; espace
// linéaire en la taille de l'arbre associé à t

//  bst_repr_graphic : temps linéaire en la taille de l'arbre associé à t ;
// espace linéaire en la taille de l'arbre associé à t

//  bst_dft_infix_apply_context : temps ... ; espace ...
