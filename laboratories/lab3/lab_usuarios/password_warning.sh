#!/bin/bash

days_left=$(chage -l "$USER" 2>/dev/null | awk -F: '/Password expires/ {print $2}' | xargs)

if [[ -z "$days_left" || "$days_left" == "never" ]]; then
    exit 0
fi

expire_date=$(date -d "$days_left" +%s 2>/dev/null)
today=$(date +%s)

if [[ -z "$expire_date" ]]; then
    exit 0
fi

diff_days=$(( (expire_date - today) / 86400 ))

if [[ $diff_days -le 10 && $diff_days -ge 0 ]]; then
    echo ""
    echo "-----------------//-------//------//---------"
    echo " ADVERTENCIA: tu password vence en $diff_days día(s)."
    echo " Debes cambiarla pronto para evitar el bloqueo."
    echo "-------//-----------------//-----//-----------"
    echo ""
fi
