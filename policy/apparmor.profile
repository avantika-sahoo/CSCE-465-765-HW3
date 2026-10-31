#include <tunables/global>

profile hw3-parser "/home/mstar/csce465-agentsec/CSCE-465-765-HW3/legacy_log_parser" {
    # Read/map the executable and required runtime.
    "/home/mstar/csce465-agentsec/CSCE-465-765-HW3/legacy_log_parser" mr,
    /etc/ld.so.cache r,
    /{usr/,}lib/x86_64-linux-gnu/libc.so.6 mr,
    /{usr/,}lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 mr,
    /{usr/,}lib64/ld-linux-x86-64.so.2 mr,

    # Read only the selected course input.
    "/home/mstar/csce465-agentsec/CSCE-465-765-HW3/inbox/payload.bin" r,

    # Write only the expected marker.
    "/home/mstar/csce465-agentsec/CSCE-465-765-HW3/markers/ret2win.txt" w,

    # Block and log access to the fake protected file.
    audit deny /opt/agentsec/decoy.txt rwklx,

    # Block subsequent execution of shells and other programs.
    audit deny /** x,
}
