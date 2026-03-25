#!/bin/bash

password="$1"

if [[ -z "$password" ]]; then
    echo "Error: debe proporcionar una contraseña."
    exit 1
fi

if [[ "$password" =~ [[:space:]] ]]; then
    echo "Error: la contraseña no puede contener espacios."
    exit 1
fi

if [[ ! "$password" =~ [A-Z] ]]; then
    echo "Error: la contraseña debe contener al menos una letra mayúscula."
    exit 1
fi

if [[ ! "$password" =~ [^a-zA-Z0-9] ]]; then
    echo "Error: la contraseña debe contener al menos un carácter especial."
    exit 1
fi

echo "Contraseña válida."
exit 0
