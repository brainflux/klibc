#!/usr/bin/perl
while ( defined($line = <STDIN>) ) {
    chomp $line;
    $line =~ s/\s*\#.*$//;	# Strip comments and trailing blanks

    if ( $line =~ /^\s*([^\(]+)\s+([^\s\(]+)\s*\(([^\(]*)\)\s*$/ ) {
	$type = $1;
	$name = $2;
	$argv = $3;

	@args = split(/\s*\,\s*/, $argv);
	@cargs = ();

	$i = 0;
	for $arg ( @args ) {
	    push(@cargs, "$arg a".$i++);
	}
	$nargs = $i;

	open(OUT, "> socketcalls/${name}.c")
	    or die "$0: Cannot open socketcalls/${name}.c\n";
	print OUT "#include \"socketcommon.h\"\n\n";

	print OUT "#ifdef __NR_$name\n\n";
	print OUT "_syscall", scalar(@args), "(", $type, ',', $name;
	$i = 0;
	foreach $arg ( @args ) {
	    print OUT ",", $arg, ",a",$i++;
	}
	print OUT ");\n";
	print OUT "\n#else\n\n";

	print OUT "$type $name (", join(', ', @cargs), ")\n";
	print OUT "{\n";
	print OUT "    unsigned long args[$nargs];\n";
	for ( $i = 0 ; $i < $nargs ; $i++ ) {
	    print OUT "    args[$i] = (unsigned long)a$i;\n";
	}
	print OUT "    return ($type) socketcall(SYS_\U${name}\E, args);\n";
	print OUT "}\n";
	print OUT "\n#endif\n";
	close(OUT);
    }
}
