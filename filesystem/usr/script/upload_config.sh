#!/bin/sh
echo do upload config
/userfs/bin/tcapi set System_Entry upgrade_fw_status "NONE"
/userfs/bin/tcapi set System_Entry upgrade_fw "2"
/userfs/bin/tcapi commit System_Entry
