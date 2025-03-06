#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <locale.h>
#include <getopt.h>

void list_directory(const char *dir_name, int show_links, int show_dirs, int show_files) {
    struct dirent *entry;
    struct stat file_stat;
    DIR *dir = opendir(dir_name);

    if (!dir) {
        perror("opendir");
        return;
    }

    // Сохранение всех записей в массив для последующей сортировки
    char *entries[4096];
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        char path[4096];
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

        // Пропустить специальные директории
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (stat(path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        int is_link = S_ISLNK(file_stat.st_mode);
        int is_dir = S_ISDIR(file_stat.st_mode);
        int is_file = S_ISREG(file_stat.st_mode);

        if ((show_links && is_link) || (show_dirs && is_dir) || (show_files && is_file) || (!show_links && !show_dirs && !show_files)) {
            entries[count] = malloc(strlen(path) + 1);  // Выделение памяти для строки
            if (entries[count]) {
                strcpy(entries[count], path);  // Копируем путь
                count++;
            }
        }

        if (is_dir) {
            list_directory(path, show_links, show_dirs, show_files);
        }
    }

    closedir(dir);

    // Сортировка для вывода
    if (count > 0) {
        // Сортируем массив записей
        qsort(entries, count, sizeof(char *), (int (*)(const void *, const void *)) strcoll);
        for (int i = 0; i < count; i++) {
            printf("%s\n", entries[i]);
            free(entries[i]);  // Освобождаем память
        }
    }
}

int main(int argc, char *argv[]) {
    const char *dir_name = "./";
    int show_links = 0, show_dirs = 0, show_files = 0;

    // Установить локаль для сортировки
    setlocale(LC_COLLATE, "");
    
    int opt;
    while ((opt = getopt(argc, argv, "ldfs")) != -1) {
        switch (opt) {
            case 'l':
                show_links = 1;
                break;
            case 'd':
                show_dirs = 1;
                break;
            case 'f':
                show_files = 1;
                break;
            case 's':
                // Флаг для сортировки уже используется
                break;
            default:
                fprintf(stderr, "Usage: %s [dir] [-l] [-d] [-f] [-s]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Проверка аргумента каталога
    if (optind < argc) {
        dir_name = argv[optind];
    }

    list_directory(dir_name, show_links, show_dirs, show_files);
    return 0;
}
