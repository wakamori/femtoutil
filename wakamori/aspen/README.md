Aspen
===========

Aspen is an online KonohaScript editor.

Install
----------

Before using Aspen, you must configure Apache httpd.conf file.
If it is possible, use ScriptAlias and locate cgi-bin directory outside of
DocumentRoot directory.

Setting example (Mac OSX)
	<Directory "/path/to/aspen/cgi-bin/">
		AllowOverride None
		Options ExecCGI FollowSymLinks
		AddHandler text/plain .out .err .k
		Order allow,deny
		Allow from all
	</Directory>

How to use
----------

Others
-----------------

* FAQ
* Notes
* Misc
* Known issues
