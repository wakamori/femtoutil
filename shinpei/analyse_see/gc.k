#!/usr/local/bin/konoha
/* -------------------------------------------
 * gc.k : 
 *  konoha gclog analyze program
 *  shinpei(c)2011
 * ------------------------------------------- */

include "laf.k"
/* ------------------------------------------- */
/* [analyzers] */
include "objlivetime.k";
include "classcount.k"

int main (String[] args)
{
	Laf laf = new Laf();
	if (laf.parseArguments(args)) {
		laf.prepare(new ObjectLivetimeStreamAnalyzer());
		laf.start();
	} 
	return 0;
}
