#!/bin/sh
/userfs/bin/tcapi set System_Entry upgrade_fw_status "NONE"
/userfs/bin/tcapi set System_Entry upgrade_fw "1"
/userfs/bin/tcapi commit System_Entry

