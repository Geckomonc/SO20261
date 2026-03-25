#!/bin/bash

DEFAULT_PASSWORD="Lab2026!"
ALLOWED_CMD="/opt/lab_usuarios/labctl.sh"

require_root() {
    if [[ "$EUID" -ne 0 ]]; then
        echo "Error: solo el superusuario puede ejecutar acciones administrativas."
        exit 1
    fi
}

validate_username() {
    local username="$1"
    if [[ -z "$username" ]]; then
        echo "Error: debe indicar un nombre de usuario."
        exit 1
    fi
}

list_users() {
    echo "Usuarios del sistema con UID >= 1000:"
    awk -F: '$3 >= 1000 && $1 != "nobody" {print $1}' /etc/passwd
}

create_user() {
    require_root
    validate_username "$1"
    local username="$1"

    if id "$username" &>/dev/null; then
        echo "Error: el usuario ya existe."
        exit 1
    fi

    useradd -m -s /bin/bash "$username"
    echo "${username}:${DEFAULT_PASSWORD}" | chpasswd

    chage -M 30 -W 10 -I 0 "$username"
    chage -d 0 "$username"

    echo "Usuario creado correctamente."
    echo "Usuario: $username"
    echo "Contraseña por defecto: $DEFAULT_PASSWORD"
    echo "Debe cambiar la contraseña en el primer inicio de sesión."
}

lock_user() {
    require_root
    validate_username "$1"
    passwd -l "$1"
    echo "Usuario bloqueado correctamente."
}

unlock_user() {
    require_root
    validate_username "$1"
    passwd -u "$1"
    echo "Usuario desbloqueado correctamente."
}

delete_user() {
    require_root
    validate_username "$1"
    userdel -r "$1"
    echo "Usuario eliminado correctamente."
}

user_status() {
    require_root
    validate_username "$1"
    chage -l "$1"
    passwd -S "$1"
}

grant_lab_sudo() {
    require_root
    validate_username "$1"
    local username="$1"
    local sudo_file="/etc/sudoers.d/lab_${username}"

    echo "${username} ALL=(ALL) NOPASSWD: ${ALLOWED_CMD}" > "$sudo_file"
    chmod 440 "$sudo_file"

    echo "Permiso sudo restringido otorgado a $username"
    echo "Solo podrá ejecutar: sudo ${ALLOWED_CMD}"
}

check_expired_users() {
    require_root
    echo "Revisando usuarios con contraseña vencida..."

    while IFS=: read -r user _ uid _ _ home shell; do
        if [[ "$uid" -ge 1000 && "$user" != "nobody" ]]; then
            expiry_info=$(chage -l "$user" 2>/dev/null | awk -F: '/Password expires/ {print $2}' | xargs)

            if [[ -n "$expiry_info" && "$expiry_info" != "never" ]]; then
                expiry_date=$(date -d "$expiry_info" +%s 2>/dev/null)
                today=$(date +%s)

                if [[ -n "$expiry_date" && "$today" -gt "$expiry_date" ]]; then
                    passwd -l "$user" >/dev/null 2>&1
                    echo "Usuario bloqueado por contraseña vencida: $user"
                fi
            fi
        fi
    done < /etc/passwd
}

show_help() {
    cat <<EOF
Uso: $0 <comando> [usuario]

Comandos disponibles:
  list-users           Listar usuarios del sistema
  create <usuario>     Crear usuario con contraseña por defecto
  lock <usuario>       Bloquear usuario
  unlock <usuario>     Desbloquear usuario
  delete <usuario>     Eliminar usuario
  status <usuario>     Ver estado del usuario
  grant-sudo <usuario> Dar sudo restringido solo al laboratorio
  check-expired        Bloquear usuarios con contraseña vencida
  help                 Mostrar ayuda
EOF
}

case "$1" in
    list-users)
        list_users
        ;;
    create)
        create_user "$2"
        ;;
    lock)
        lock_user "$2"
        ;;
    unlock)
        unlock_user "$2"
        ;;
    delete)
        delete_user "$2"
        ;;
    status)
        user_status "$2"
        ;;
    grant-sudo)
        grant_lab_sudo "$2"
        ;;
    check-expired)
        check_expired_users
        ;;
    help|--help|-h|"")
        show_help
        ;;
    *)
        echo "Comando no válido."
        show_help
        exit 1
        ;;
esac
