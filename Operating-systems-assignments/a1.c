#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define MAX_PATH_LEN 1024

typedef struct {
    unsigned char magic[2];
    unsigned short header_size;
    unsigned char version;
    unsigned char no_of_sections;
} SF_Header;

typedef struct {
    char sect_name[15];
    unsigned short sect_type;
    unsigned int sect_offset;
    unsigned int sect_size;
} Section_Header;

void list_directory(const char *path, int recursive, const char *name_filter, long min_size, int *found) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("ERROR\nInvalid directory path\n");
        return;
    }

    struct dirent *entry;
    struct stat statbuf;
    char full_path[MAX_PATH_LEN];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, MAX_PATH_LEN, "%s/%s", path, entry->d_name);

        if (lstat(full_path, &statbuf) == -1) {
            continue;
        }

        if (name_filter != NULL && strncmp(entry->d_name, name_filter, strlen(name_filter)) != 0) {
            continue;
        }

        if (min_size != -1) {
            if (!S_ISREG(statbuf.st_mode)) {
                continue;
            }
            if (statbuf.st_size <= min_size) {
                continue;
            }
        }

        printf("%s\n", full_path);
        (*found)++;

        if (recursive && S_ISDIR(statbuf.st_mode)) {
            list_directory(full_path,recursive,name_filter,min_size,found);

        }
    }

    closedir(dir);
}


void parse_sf_file(const char *file_path) {
    int fd=open(file_path,O_RDONLY);
    if(fd==-1) 
    {
        printf("ERROR\ninvalid file\n");
        return;
    }

    SF_Header header;
    if(read(fd,&header.magic,2)!=2 || read(fd,&header.header_size,2)!=2 || read(fd,&header.version,1)!=1 || read(fd,&header.no_of_sections,1)!=1)
     {
        printf("ERROR\ninvalid file\n");
        close(fd);
        return;
    }

    if(header.magic[0]!='P' || header.magic[1]!='f') 
    {
        printf("ERROR\nwrong magic\n");
        close(fd);
        return;
    }

    if(header.version<43 || header.version>162) {
        printf("ERROR\nwrong version\n");
        close(fd);
        return;
    }

    if((header.no_of_sections!=2) && (header.no_of_sections<4 || header.no_of_sections>16))
     {
        printf("ERROR\nwrong sect_nr\n");
        close(fd);
        return;
    }

    Section_Header *sections=malloc(header.no_of_sections*sizeof(Section_Header));
    if(sections==NULL) 
    {
        printf("ERROR\ninvalid file\n");
        close(fd);
        return;
    }

    for(int i=0;i<header.no_of_sections;i++) 
    {
        if (read(fd,sections[i].sect_name, 15)!=15 || read(fd,&sections[i].sect_type, 2)!=2 || read(fd,&sections[i].sect_offset, 4)!=4 || read(fd, &sections[i].sect_size,4)!=4) 
        {
            printf("ERROR\ninvalid file\n");
            free(sections);
            close(fd);
            return;
        }
    }
    for(int i=0;i<header.no_of_sections;i++)
     {
        if (sections[i].sect_type!= 35&&sections[i].sect_type!=17)
         {
            printf("ERROR\nwrong sect_types\n");
            free(sections);
            close(fd);
            return;
        }
    }

    printf("SUCCESS\n");
    printf("version=%d\n", header.version);
    printf("nr_sections=%d\n", header.no_of_sections);
    
    for(int i=0;i<header.no_of_sections;i++) 
    {
        printf("section%d: %s %d %d\n",i+1,sections[i].sect_name,sections[i].sect_type,sections[i].sect_size);
    }
    free(sections);
    close(fd);
}



int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("\nPrea putine argumente\n");
        return -1;
    }

    if (strcmp(argv[1], "variant") == 0) {
        printf("36611\n");
        return 0;
    }

    if (strcmp(argv[1], "list") == 0) {
        int recursive = 0;
        char *name_filter = NULL;
        long min_size = -1;
        char *path = NULL;
        int found = 0;

        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "recursive") == 0) {
                recursive = 1;
            } else if (strncmp(argv[i], "name_starts_with=", 17) == 0) {
                name_filter = argv[i] + 17;
            } else if (strncmp(argv[i], "size_greater=", 13) == 0) {
                min_size = strtol(argv[i] + 13, NULL, 10);
            } else if (strncmp(argv[i], "path=", 5) == 0) {
                path = argv[i] + 5;
            }
        }

        if (path == NULL) {
            printf("ERROR\nDirectory path invalid\n");
            return 1;
        }

        printf("SUCCESS\n");
        list_directory(path, recursive, name_filter, min_size, &found);

        if (found == 0) {
            printf("SUCCESS\n");
        }

        return 0;
    }
    else if (strcmp(argv[1], "parse") == 0) {
        char *path = NULL;
        
        for (int i = 2; i < argc; i++) {
            if (strncmp(argv[i], "path=", 5) == 0) {
                path = argv[i] + 5;
                break;
            }
        }

        if (path == NULL) {
            printf("ERROR\ninvalid file\n");
            return 1;
        }

        parse_sf_file(path);
        return 0;
    }
    else {
        fprintf(stderr, "Comanda invalida\n");
        return 1;
    }
}