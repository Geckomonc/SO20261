#!/bin/bash

BASE_DIR="/opt/lab_usuarios"

echo "Iniciando configuración del laboratorio..."

chmod +x "$BASE_DIR/validate_password.sh"
chmod +x "$BASE_DIR/password_warning.sh"
chmod +x "$BASE_DIR/configure_pam.sh"
chmod +x "$BASE_DIR/setup.sh"
chmod +x "$BASE_DIR/labctl.sh"

cp "$BASE_DIR/password_warning.sh" /etc/profile.d/password_warning.sh
chmod +x /etc/profile.d/password_warning.sh

"$BASE_DIR/configure_pam.sh"

echo "Configuración finalizada correctamente."
