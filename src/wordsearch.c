#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

// Функция для поиска слова в строках файла
void search_in_file(const char *filepath, const char *word) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    char line[1024];
    int line_number = 0;
    
    // Чтение файла построчно
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        // Поиск слова в строке
        if (strstr(line, word) != NULL) {
            printf("%s:%d: %s", filepath, line_number, line);
        }
    }

    fclose(file);
}

// Рекурсивная функция для обхода директорий
void search_in_directory(const char *dirpath, const char *word) {
    DIR *dir = opendir(dirpath);
    if (dir == NULL) {
        perror("Ошибка открытия директории");
        return;
    }

    struct dirent *entry;
    
    // Чтение всех файлов и директорий в текущей директории
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем текущую и родительскую директорию
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        
        // Если это директория, рекурсивно вызываем поиск
        if (entry->d_type == DT_DIR) {
            search_in_directory(path, word);
        } 
        // Если это файл, ищем в нем слово
        else if (entry->d_type == DT_REG) {
            search_in_file(path, word);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *directory = "~/files"; // Директория по умолчанию
    const char *word = NULL;          // Слово для поиска

    // Обработка аргументов командной строки
    if (argc > 1) {
        directory = argv[1];
    }
    if (argc > 2) {
        word = argv[2];
    }

    // Если слово не передано, выводим ошибку
    if (word == NULL) {
        fprintf(stderr, "Не указано слово для поиска.\n");
        return 1;
    }

    // Расширяем путь до домашней директории (~)
    char expanded_dir[1024];
    snprintf(expanded_dir, sizeof(expanded_dir), "%s", directory);
    if (expanded_dir[0] == '~') {
        char *home = getenv("HOME");
        if (home != NULL) {
            memmove(expanded_dir + strlen(home), expanded_dir + 1, strlen(expanded_dir));
            memcpy(expanded_dir, home, strlen(home));
        }
    }

    // Запускаем рекурсивный поиск
    search_in_directory(expanded_dir, word);

    return 0;
}
