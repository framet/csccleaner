# csccleaner
Cloudstation Read-Only Flag problematic file cleaner

This application searches through the daemon.log from Cloudstation Client for "[ERROR] download-remote-handler.cpp(264): Worker (21623): Unable to handle locked/no permission file" and gives you the possibility to delete the file or rename ith with an underscore infront of the realname.

If a file could not be deleted it will be marked red. Carefull, maybe this file is already deleted (double entries).
If a file could not be renamed it will be marked blue. Carefull, maybe this file is already renamed (double entries).
