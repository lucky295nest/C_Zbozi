#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "zbozi.txt"
#define OUTPUT "vypis_zbozi.html"
#define BUFFER 150

struct ZBOZI
{
    char dodavatel[5];
    char nazevZbozi[20];
    int hmotnost;
    int pocet;
    float cena;
};

void convertGtoKg(int gramy)
{
    int kilogramy = gramy / 1000;
    int zbyvajiciGramy = gramy % 1000;

    printf("Celková váha zboží: %d kg %d g\n", kilogramy, zbyvajiciGramy);
}

void UndrScrToSpace(char *str)
{
    int lenght = strlen(str);

    for (int i = 0; i < lenght; i++)
    {
        if (str[i] == '_')
        {
            str[i] = ' ';
        }
    }
}

int main()
{
    // otevreni souboru
    FILE *fr;
    fr = fopen(INPUT, "r");
    if (fr == NULL)
    {
        printf("Error opening file %s.\n", INPUT);
        return 1;
    }

    char header[BUFFER];
    if(fgets(header, sizeof(header), fr) == NULL)
    {
        perror("Error reading first line");
    }

    struct ZBOZI zbozi[100];
    int pocetZbozi = 0;

    char line[BUFFER];
    while (fgets(line, sizeof(line), fr))
    {
        char nazevZbozi[20];

        sscanf(line, "%[^;];%[^;];%d;%d;%f",
        zbozi[pocetZbozi].dodavatel,
        nazevZbozi,
        &zbozi[pocetZbozi].hmotnost,
        &zbozi[pocetZbozi].pocet,
        &zbozi[pocetZbozi].cena);

        UndrScrToSpace(nazevZbozi);
        strcpy(zbozi[pocetZbozi].nazevZbozi, nazevZbozi);

        pocetZbozi++;
    }
    

    // zavreni souboru
    fclose(fr);

    // vypis do terminalu

    char nejdrNazev[20];
    float nejdrazsiZbozi = 0.0;
    int celkovaHmotnost = 0;
    float celkovaCena = 0.0;

    printf("+----------+---------------------+----------------+------+----------------+------------+\n");
    printf("| DODAVATEL|                NAZEV| HMOTNOST (1 ks)| POCET|     CENA (1 ks)| CENA CELKEM|\n");
    printf("+----------+---------------------+----------------+------+----------------+------------+\n");
    for (int i = 0; i < pocetZbozi; i++)
    {
        if (zbozi[i].cena > nejdrazsiZbozi)
        {
            nejdrazsiZbozi = zbozi[i].cena;
            strcpy(nejdrNazev, zbozi[i].nazevZbozi);
        }
        celkovaCena += zbozi[i].cena * zbozi[i].pocet;
        celkovaHmotnost += zbozi[i].hmotnost * zbozi[i].pocet;

        printf("|%10s|%21s|%14d g|%3d ks|        %s%0.2f kc|   %s%s%0.2f kc|\n",
        zbozi[i].dodavatel,
        zbozi[i].nazevZbozi,
        zbozi[i].hmotnost,
        zbozi[i].pocet,
        zbozi[i].cena <= 10 ? " " : "",
        zbozi[i].cena,
        (zbozi[i].cena * zbozi[i].pocet) <= 100 ? " " : "",
        (zbozi[i].cena * zbozi[i].pocet) <= 10 ? "" : "",
        zbozi[i].cena * zbozi[i].pocet);
        printf("+----------+---------------------+----------------+------+----------------+------------+\n");
    }
    printf("Cena celkem: %0.2f\n", celkovaCena);
    printf("Pocet polozek: %d\n", pocetZbozi);
    convertGtoKg(celkovaHmotnost);
    printf("Nejdrazsi zakoupene zbozi: %s za %0.2f\n", nejdrNazev, nejdrazsiZbozi);

    // otevreni vystupniho souboru pro zapis v HTML
    FILE *fw;
    fw = fopen(OUTPUT, "w");
    if (fw == NULL)
    {
        printf("Error opening file %s.\n", OUTPUT);
        return 1;
    }

    // Zápis hlavičky HTML souboru, vložení CSS stylu a začátek tabulky
    fprintf(fw, "<html><head><title>Vypis Zbozi</title>\n");
    fprintf(fw, "<style>\n");
    fprintf(fw, "table {\n");
    fprintf(fw, "    font-family: Arial, sans-serif;\n");
    fprintf(fw, "    border-collapse: collapse;\n");
    fprintf(fw, "    width: 100%%;\n");
    fprintf(fw, "}\n");
    fprintf(fw, "th, td {\n");
    fprintf(fw, "    border: 1px solid #dddddd;\n");
    fprintf(fw, "    text-align: left;\n");
    fprintf(fw, "    padding: 8px;\n");
    fprintf(fw, "}\n");
    fprintf(fw, "tr:nth-child(even) {\n");
    fprintf(fw, "    background-color: #f2f2f2;\n");
    fprintf(fw, "}\n");
    fprintf(fw, "th {\n");
    fprintf(fw, "    background-color: #4CAF50;\n");
    fprintf(fw, "    color: white;\n");
    fprintf(fw, "}\n");
    fprintf(fw, "</style>\n");
    fprintf(fw, "</head><body>\n");
    fprintf(fw, "<table>\n");
    fprintf(fw, "<tr><th>DODAVATEL</th><th>NAZEV</th><th>HMOTNOST (1 ks)</th><th>POCET</th><th>CENA (1 ks)</th></tr>\n");

    for (int i = 0; i < pocetZbozi; i++)
    {
        // Přidat položky pouze pro dodavatele, kteří nejsou "OP"
        if (strcmp(zbozi[i].dodavatel, "OP") != 0)
        {
            fprintf(fw, "<tr>");
            fprintf(fw, "<td>%s</td><td>%s</td><td>%d g</td><td>%d ks</td><td>%.2f kc</td>",
                    zbozi[i].dodavatel,
                    zbozi[i].nazevZbozi,
                    zbozi[i].hmotnost,
                    zbozi[i].pocet,
                    zbozi[i].cena);
            fprintf(fw, "</tr>\n");
        }
    }

    // Zápis patičky HTML souboru a uzavření tabulky
    fprintf(fw, "</table></body></html>\n");

    // zavreni vystupniho souboru
    fclose(fw);

    printf("Data byla zapsana do souboru %s.\n", OUTPUT);

    return 0;
}
