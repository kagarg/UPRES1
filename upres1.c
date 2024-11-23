#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void listDirectories(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    dir = opendir(path); 
//     Открытие и закрытие каталога выполняется системными вызовами opendir()
// и closedir(). При успешном открытии каталога системный вызов возвращает
// указатель на переменную типа DIR, являющуюся дескриптором каталога,
// определенную в файле <dirent.h> и используемую при чтении и записи в каталог.
// При неудачном вызове возвращается значение NULL.
    if (dir == NULL)
    {
        perror("Ошибка открытия каталога");
        return;
    }

    while ((entry = readdir(dir)) != NULL) //Функция readdir() возвращает указатель на следующую запись каталога в структуре dirent, прочитанную из потока каталога
    {
        if (entry->d_type == DT_DIR) // в поле d_type хранится тип, DT_DIR соответствует типу Директория
        {
            char fullpath[PATH_MAX];
            snprintf(fullpath, PATH_MAX, "%s/%s", path, entry->d_name); //записываем в fullpath путь проверяемой директории. Например var/log

            // Проверяем, не содержит ли каталог других каталогов
            if (stat(fullpath, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) // The following macros will test whether a file is of the specified type. The value m supplied to the macros is the value of st_mode from a stat structure. The macro evaluates to a non-zero value if the test is true, 0 if the test is false.
            { // иными словами S_ISDIR - это макрос которому на вход подается содержимое флага из поля st_mode, которое находится в statbuf. При успешном заполнении структоры функция stat возваращает 0
              //Функция stat() вносит в структуру, на которую указывает statbuf, информацию, содержащуюся в файле, связанном с указателем filename. Структура stat определена в sys\stat.h.
                DIR *subdir = opendir(fullpath); // делаем все то же самое для поддиректории
                if (subdir != NULL)
                {
                    struct dirent *subentry;
                    int contains_subdirs = 0;
                    while ((subentry = readdir(subdir)) != NULL) // Просматриваем содержимое поддиректории, проверяем, есть ли другие директории. Закрываем поддиректорию.
                    {
                        if (subentry->d_type == DT_DIR && strcmp(subentry->d_name, ".") != 0 && strcmp(subentry->d_name, "..") != 0) // Игнорируем текущий и родительский каталоги
                        {
                            contains_subdirs = 1;
                            break;
                        }
                    }
                    closedir(subdir);

                    // Если каталог не содержит других каталогов, выводим его имя
                    if (!contains_subdirs)
                    {
                        printf("%s/%s\n", path, entry->d_name);
                    }
                }
            }
        }
    }

    closedir(dir);
}

int main()
{
    const char *directory_path = "var";
    listDirectories(directory_path);

    return 0;
}
