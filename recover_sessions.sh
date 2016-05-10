#!/bin/bash

SNAPSHOT=/tmp/tmux.snapshot
TMUXBIN=$(which tmux)
Pgrep=$(which pgrep)

dump_tmux_sessions()
{
    ${TMUXBIN} list-windows -a -F "#{session_name} #{window_name} #{pane_current_path}" > ${SNAPSHOT}
}

recover_tmux_sessions()
{
    ${TMUXBIN} start-server
    local i=0
    while IFS=' ' read -r SessionName WindowName PanePath
    do
        if ${TMUXBIN} has-session -t "${SessionName}" 2>/dev/null; then
            ${TMUXBIN} new-window -d -t "${SessionName}" -n "${WindowName}"
        else
            ${TMUXBIN} new-session -d -s "${SessionName}" -n "${WindowName}"
        fi
        sleep 0.5
        ${TMUXBIN} send-keys -t "${SessionName}:${i}.0" "cd ${PanePath}" ENTER
        i=`expr ${i} + 1`
    done < ${SNAPSHOT}
}

if ${Pgrep} -l '\<tmux\>' >/dev/null 2>&1; then
    dump_tmux_sessions
else
    recover_tmux_sessions
fi
