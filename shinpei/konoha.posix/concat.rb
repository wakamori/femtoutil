kfilename = "posix.k"
cfilename = "posix.c"
kfiles = []
cfiles = []
Dir::foreach('.') {|f|
	if /.k$/.match(f) && f != kfilename
		kfiles.push(f)
	elsif /.c$/.match(f) && f != cfilename 
		cfiles.push(f)
	end
}
kcontents = ''
ccontents = ''

kfiles.each {|f|
	buf = open(f)
	kcontents = kcontents + buf.read
	buf.close
}
kfile = File.open(kfilename, 'w')
kfile << kcontents
kfile.close

cfiles.each{|f|
	buf = open(f)
	ccontents = ccontents + buf.read
	buf.close
}
cfile = File.open(cfilename, 'w')
cfile << ccontents
cfile.close
