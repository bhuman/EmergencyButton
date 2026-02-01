#!/bin/sh

. /usr/lib/libmodcgi.sh
[ -r /etc/options.cfg ] && . /etc/options.cfg

sec_begin "Starttyp"
cgi_print_radiogroup_service_starttype "enabled" "$BHUMAN_ENABLED" "" "" 1
sec_end

sec_begin "Emergencybutton-Server"
cat << EOF
<h2>Der Emergencybutton-Server sendet UDP broadcasts auf dem folgenden Port:</h2>
EOF
cgi_print_textline_p "emergencybuttonport" "$BHUMAN_EMERGENCYBUTTONPORT" 5 "Port: "

cat << EOF
<h2>in dem folgenden Intervall:</h2>
EOF
cgi_print_textline_p "emergencybuttoninterval" "$BHUMAN_EMERGENCYBUTTONINTERVAL" 5 "Intervall (in ms): "
sec_end
