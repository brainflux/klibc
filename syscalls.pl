#!/usr/bin/perl
while ( defined($line = <STDIN>) ) {
    chomp $line;
    $line =~ s/\s*\#.*$//;	# Strip comments and trailing blanks

    if ( $line =~ /^\s*([^\(]+[^:A-Za-z0-9_])([A-Za-z0-9_]+)(|\:\:[A-Za-z0-9_]+)\s*\(([^\:\)]*)\)\s*$/ ) {
	$type = $1;
	$sname = $2;
	$fname = $3;
	$argv = $4;

	$type =~ s/\s*$//;

	if ( $fname eq '' ) {
	    $fname = $sname;
	} else {
	    $fname =~ s/^\:\://;
	}

	@args = split(/\s*\,\s*/, $argv);

	open(OUT, "> syscalls/${fname}.c")
	    or die "$0: Cannot open syscalls/${fname}.c\n";
	print OUT "#include \"syscommon.h\"\n\n";
	
	if ( $fname ne $sname ) {
	    print OUT "#undef __NR_${fname}\n";
	    print OUT "#define __NR_${fname} __NR_${sname}\n\n";
	}

	print OUT "_syscall", scalar(@args), "(", $type, ',', $fname;
	$i = 0;
	foreach $arg ( @args ) {
	    print OUT ",", $arg, ",a",$i++;
	}
	print OUT ");\n";
	close(OUT);
    }
}
