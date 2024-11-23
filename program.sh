#!/bin/bash
a="var"
# Проверяем, что передан аргумент
if [ -z "$a" ]
then
  echo "Usage: $0 directory_name"
  exit 1
fi

# Проверяем, является ли аргументом директорией
if [ ! -d "$a" ]
then
  echo "$a is not a directory"
  exit 1
fi

# Ищем все поддиректории в указанной директории и проверяем, не содержат ли они своих поддиректорий
echo "Каталоги в $a которые не содержат других каталогов:"
for dir in "$a"/*/
do
  if [[ -n $(find "$dir" -mindepth 1 -type d) ]]
  then
    continue
  else
    echo "${dir%/}"  # Удаляем завершающий слеш из имени директории перед выводом
  fi
done
