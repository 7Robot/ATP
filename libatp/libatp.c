/* Fonctions d’aide liées au protocole ATP.
 */

/* Liste des types valides.
 * Ceux-ci sont détaillé dans la RFC.
 * Les types spéciaux 129 et 128 désignant le début et la fin d’un paquet
 * n’apparraissent pas ici.
 */
static int atptypes[] = { 1, 2, 4, 17, 18, 20, 36, 132, 148, -1 };

/* Renvoit vrai (1) si le type spécifié en argument est un type ATP valide.
 * Renvoit faux (0) sinon.
 */
int atp_is_valid_type(int type)
{
    int i = 0;
    int t;
    while (t = atptypes[i]) {
        if (t == type)
            return 1;
        i++;
    }
    return 0;
}
