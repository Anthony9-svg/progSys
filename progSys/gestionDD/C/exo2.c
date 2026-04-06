#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

int main()
{
    DIR* dir=NULL;
    struct dirent* ent;
    char* directory=malloc(100);
    struct stat st;
    
    while(1)
    {
        printf("Repertoire: ");
        scanf("%s",directory);
        if (strcmp(directory,"stop")==0)
        {
           break;
        }
        else if((dir=opendir(directory))==NULL)
        {
            printf("Erreur d'ouverture du repertoire\n");
        }
        printf("Repertoire: %s\n",directory);
        while((ent=readdir(dir))!=NULL)
        {
            char chemin[100];
            snprintf(chemin, sizeof(chemin), "%s/%s", directory, ent->d_name);
            if (stat(chemin,&st)!=0)
            {
                printf("Erreur d'accès au fichier %s\n",ent->d_name);
                printf("%s\n",chemin);
                continue;
            }
            if (S_ISREG(st.st_mode))
            { 
                if (st.st_size>1000)
                    printf("Fichier:%s,UID du proprietaire:%d,taille:%ld \n",ent->d_name,st.st_uid,st.st_size);
            }
        
        }
        printf("\n");
    }
    return 0;
}