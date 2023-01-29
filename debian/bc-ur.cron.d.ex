#
# Regular cron jobs for the bc-ur package.
#
0 4	* * *	root	[ -x /usr/bin/bc-ur_maintenance ] && /usr/bin/bc-ur_maintenance
