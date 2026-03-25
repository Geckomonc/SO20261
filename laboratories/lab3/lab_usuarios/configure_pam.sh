#!/bin/bash

PAM_FILE="/etc/pam.d/common-password"
BACKUP_FILE="/etc/pam.d/common-password.bak"

cp "$PAM_FILE" "$BACKUP_FILE"

if grep -q "pam_pwquality.so" "$PAM_FILE"; then
    sed -i 's/^password.*pam_pwquality\.so.*/password requisite pam_pwquality.so retry=3 minlen=8 ucredit=-1 ocredit=-1 maxrepeat=2 difok=3 enforce_for_root/' "$PAM_FILE"
else
    sed -i '/^password.*pam_unix.so/i password requisite pam_pwquality.so retry=3 minlen=8 ucredit=-1 ocredit=-1 maxrepeat=2 difok=3 enforce_for_root' "$PAM_FILE"
fi

echo "Configuración PAM aplicada."
echo "Respaldo guardado en: $BACKUP_FILE"
