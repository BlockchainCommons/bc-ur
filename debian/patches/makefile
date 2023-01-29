Description: Fix build rules.
 .
 bc-ur (0.3.0-1) UNRELEASED; urgency=medium
 .
Author:Nicholas Ochiel <nochiel@users.noreply.github.com>

---
The information above should follow the Patch Tagging Guidelines, please
checkout http://dep.debian.net/deps/dep3/ to learn about the format. Here
are templates for supplementary fields that you might want to add:

Origin: <vendor|upstream|other>, <url of original patch>
Bug: <url in upstream bugtracker>
Bug-Debian: https://bugs.debian.org/<bugnumber>
Bug-Ubuntu: https://launchpad.net/bugs/<bugnumber>
Forwarded: <no|not-needed|url proving that it has been forwarded>
Reviewed-By: <name and email of someone who approved the patch>
Last-Update: 2023-01-29

--- bc-ur-0.3.0.orig/src/Makefile.in
+++ bc-ur-0.3.0/src/Makefile.in
@@ -29,7 +29,7 @@ RESET=`tput sgr0`
 
 COMPILER = g++
 CFLAGS += --debug -O0
-CXXFLAGS += -std=c++17 -stdlib=libc++ --debug -O0
+CXXFLAGS += -std=c++17 --debug -O0
 
 ARFLAGS = rcs
 
